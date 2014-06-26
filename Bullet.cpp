#include "Bullet.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <math.h>
#include <iostream>

const glm::mat4 Bullet::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));
GLuint Bullet::m_shaderProgram = 0;

Bullet::Bullet(b2World& world):
  m_pBody(0),
  m_world(world),
  m_radius(0.1f)
{
  b2BodyDef bulletBodyDef;
  bulletBodyDef.bullet = true;
  bulletBodyDef.active = false;
  bulletBodyDef.type = b2_dynamicBody;

  m_pBody = m_world.CreateBody(&bulletBodyDef);

  b2CircleShape bulletShape;
  bulletShape.m_radius = m_radius;

  b2FixtureDef bulletFixture;
  bulletFixture.restitution = 0.3f;
  bulletFixture.shape = &bulletShape;

  m_pBody->CreateFixture(&bulletFixture);

  int numTangents = 17;
  int numFields = 4 * numTangents;
  //Setup the data for OpenGL rendering
  GLfloat vertices[numFields]; //one center point, 16 on the circle
  
  b2Vec2 bodyPosition = m_pBody->GetWorldCenter();
  vertices[0] = bodyPosition.x;
  vertices[1] = bodyPosition.y;
  vertices[2] = 0.0f;
  vertices[3] = 1.0f;
  
//  6.28/16 *i/4
  for(int i = 4; i < numFields; i+=4)
  {
      vertices[i] = m_radius * sinf(6.28f/16.0f*i/4.0f) + bodyPosition.x;
      vertices[i+1] = m_radius * cosf(6.28f/16.0f*i/4.0f) + bodyPosition.y;
      vertices[i+2] = 0.0f;
      vertices[i+3] = 1.0f;
  }

  if(!m_shaderProgram)
  {
      //TBD try/catch
      //Load and compile the shaders
      ShaderList_T shaderList;
      shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "BulletVertex.glsl"));
      shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "BulletFragment.glsl"));
      
      m_shaderProgram = createShaderProgram(shaderList);
      
      //done with the shaders, so delete them
      //do this here instead of in createShaderProgram so that a shader
      //can be reused without rebuilding it
      std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
  }

  //Setup the vertex buffers.  Should this be its own function?
  //glGenBuffers(Number of buffers to allocate, array to put buffer handlers into);
  glGenBuffers(1, m_vertexBufferHandleArray);
  //select the current buffer to have opengl use
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandleArray[0]);
  //copy the data into it
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  //array to give the order of the vertices to draw
  GLubyte tmpIndices[] =
      {
          //front
          0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
          
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


Bullet::~Bullet()
{
  m_world.DestroyBody(m_pBody);
}


void Bullet::Fire(const b2Transform& xf)
{
    //get a direction and a position to start from
    //This should probably be based on the transform
    //from the ship
    b2Vec2 pos  = b2Mul(xf,b2Vec2(0.0f, 0.5f));
    m_pBody->SetTransform(pos,xf.q.GetAngle());
    //Get/Set the starting timestamp
    m_timestamp = SDL_GetTicks();
    SetActive();
}

void Bullet::Update()
{
    if(m_pBody->IsActive())
    {
        if(m_timestamp - SDL_GetTicks() <= 0)
	{
            SetInactive();
	}
    }
}

void Bullet::SetActive()
{
    m_pBody->SetActive(true);
}
void Bullet::SetInactive()
{
  m_pBody->SetActive(false);
}



void Bullet::Draw()
{
    if(m_pBody->IsActive())
    {
        glUseProgram(m_shaderProgram);
     
        //get a handle for the rotation uniform;
        GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
        glm::mat4 modelMatrix(1.0f);

        //glm translation vector
        b2Vec2 position = m_pBody->GetWorldCenter();
     
        //box uses radians, GLM uses degrees
        float angleDeg = RAD2DEG(m_pBody->GetAngle());

        //Translate then Rotate
        modelMatrix = 
            glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f)); 
        modelMatrix = 
            glm::rotate(modelMatrix, angleDeg , m_rotateAxis);    

        //skipping the view matrix, because we don't need it
        glm::mat4 mvpMat = m_projMat * modelMatrix;
     
        glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(mvpMat));
        //get a handle to the vPosition attribute of the shader
        //this can/should be done right after linking the shader and
        //stored in a member variable
        //Should probably do that with all the shader program handles
        GLuint vertexAttribute = glGetAttribLocation(m_shaderProgram,"vPosition");
        //bind the buffer that we're going to get the attributes from
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandleArray[0]);

        glEnableVertexAttribArray(vertexAttribute);
        glVertexAttribPointer(vertexAttribute, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, 0);
 
        //must bind this before glDrawElements
        //or give glDrawElements an array of indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glDrawElements(GL_TRIANGLE_FAN,m_numIndices,GL_UNSIGNED_BYTE,0);

        glDisableVertexAttribArray(vertexAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }
}
