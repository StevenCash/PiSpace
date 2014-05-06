#include <algorithm>
#include <iostream>

#include "Ship.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

Ship::Ship(b2World& world):
    m_angle(0.0f),
    m_yOffset(0.0f),
    m_xOffset(0.0f),
    m_forward(0.0,-1.0f),
    m_worldRef(world)

{

    //Set up the object for Box2D
    m_bodyDef.type = b2_dynamicBody;
    m_bodyDef.position.Set(5.0f,5.0f); //trying to start at the center
    m_pBody = m_worldRef.CreateBody(&m_bodyDef);
    b2PolygonShape shipShape;

    //define array of vertices for the ship shape
    b2Vec2 boxShipVertices[4];

    //Vertices definitions for use by both OpenGL and Box2D
    TempStruct shipVertices[] =
        {
            {{ 0.0f, 0.3f, 0.0f,1.0f}, //0
             {1.0, 0.0, 0.0, 1.0}},
            {{-0.3f, -0.1f, 0.0f,1.0f}, //1
             {0.0, 1.0, 0.0, 1.0}},
            {{0.0f, 0.0f, 0.0f,1.0f}, //2
             {0.0, 1.0, 0.0, 1.0}},
            {{0.3f, -0.1f, 0.0f,1.0f}, //3
             {0.0, 1.0, 0.0, 1.0}}
        };

    for(int index=0; index < 4; ++index)
    {
        boxShipVertices[index].x = GL2BOX(shipVertices[index].vertices[0]);
        boxShipVertices[index].y = GL2BOX(shipVertices[index].vertices[1]);
    }

    shipShape.Set(boxShipVertices, 4);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shipShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    m_pBody->CreateFixture(&fixtureDef);


    //TBD try/catch
    //Load and compile the shaders
    ShaderList_T shaderList;
    shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "ShipVertex.glsl"));
    shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "ShipFragment.glsl"));

    m_shaderProgram = createShaderProgram(shaderList);
    
    //done with the shaders, so delete them
    //do this here instead of in createShaderProgram so that a shader
    //can be reused without rebuilding it
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
 
    //Setup the vertex buffers.  Should this be its own function?
    //glGenBuffers(Number of buffers to allocate, array to put buffer handlers into);
    glGenBuffers(1, m_vertexBufferHandleArray);
    //select the current buffer to have opengl use
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandleArray[0]);
    //copy the data into it
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipVertices), shipVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //array to give the order of the vertices to draw
    GLubyte tmpIndices[] =
    {
        //front
        0,1,2,
        2,3,0

    };
    
    //store the number of indices in the index buffer
    //this only works because they're GLubytes
    m_numIndices = sizeof(tmpIndices);
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices, tmpIndices, GL_STATIC_DRAW);

    //release the buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

//Main drawing function for this Ship
void Ship::Draw()
{
     glUseProgram(m_shaderProgram);

     //get a handle for the rotation uniform;
     GLuint rotation = glGetUniformLocation(m_shaderProgram,"rotation");
     glm::mat4 modelMatrix(1.0f);
     
     //Rotate
//     modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2));
     modelMatrix = glm::rotate(modelMatrix, (float)RAD2DEG(m_pBody->GetAngle()),glm::vec3(0.0f,0.0f,-1.0f));       
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(m_xOffset, m_yOffset, 0.0f)); 
     glUniformMatrix4fv(rotation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    //get a handle to the vPosition attribute of the shader
    //this can/should be done right after linking the shader and
    //stored in a member variable
    //Should probably do that with all the shader program handles
    GLuint vertexAttribute = glGetAttribLocation(m_shaderProgram,"vPosition");
    //bind the buffer that we're going to get the attributes from
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandleArray[0]);

    glEnableVertexAttribArray(vertexAttribute);
    glVertexAttribPointer(vertexAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(TempStruct), 0);

 
    GLuint colorAttribute = glGetAttribLocation(m_shaderProgram, "vColor");
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(TempStruct), (void*)(sizeof(GLfloat)*4));
    glEnableVertexAttribArray(colorAttribute);


   //must bind this before glDrawElements
    //or give glDrawElements an array of indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glDrawElements(GL_TRIANGLES,m_numIndices,GL_UNSIGNED_BYTE,0);



    glDisableVertexAttribArray(vertexAttribute);
//    glDisableVertexAttribArray(colorAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

}


//Destructor
Ship::~Ship()
{
}

void Ship::rotateRight()
{
    m_pBody->SetAngularVelocity(1.0f);
    std::cout << "AV = " << m_pBody->GetAngularVelocity() << std::endl;
}

void Ship::rotateLeft()
{
    m_pBody->SetAngularVelocity(-1.0f);
    std::cout << "AV = " << m_pBody->GetAngularVelocity() << std::endl;
}

void Ship::translateUp()
{
    m_yOffset += 0.1f;
}

void Ship::translateDown()
{
    m_yOffset -= 0.1f;
}
