#include "Bullet.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <math.h>
#include <iostream>

//const glm::mat4 Bullet::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));
const glm::mat4 Bullet::m_projMat(glm::ortho(-1.0f,1.0f,-1.0f,1.0f));
GLuint Bullet::m_shaderProgram = 0;

Bullet::Bullet(b2World& world):
  m_pBody(0),
  m_world(world),
  m_radius(5.0f),
  m_rotateAxis(glm::vec3(0.0f,0.0f,1.0f))
{
#ifdef DAMMIT
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

  b2Vec2 bodyPosition = m_pBody->GetWorldCenter();
#endif

  int numTangents = 16;
  int numFields = 4 * (numTangents+1+1);  //plus centerpoint and the final connection point

  //Setup the data for OpenGL rendering
  GLfloat vertices[numFields]; //one center point, 16 on the circle

  vertices[0] = 0.0;//bodyPosition.x;
  vertices[1] = 0.0;//bodyPosition.y;
  vertices[2] = 0.0f;
  vertices[3] = 1.0f;
  
  float angleBase = 2.0f * 3.14159265f / numTangents;

  for(int j = 0; j < numTangents; ++j)
  {
      int index = (j+1)*4;
      vertices[index] = m_radius * sinf(angleBase * j) + vertices[0];
      vertices[index+1] = m_radius * cosf(angleBase * j) + vertices[1];
      vertices[index+2] = 0.0f;
      vertices[index+3] = 1.0f;
  }

  //make the last point on the circle match up with the first point
  int index = numTangents * 4;
  vertices[index] = vertices[4];
  vertices[index+1] = vertices[5];
  vertices[index+2] = vertices[6];
  vertices[index+3] = vertices[7];
  

    
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
  GLubyte tmpIndices[numTangents+2];
  for(int i=0;i < numTangents+2; ++i)
  {
      tmpIndices[i] = i;
  }
  
  //store the number of indices in the index buffer
  m_numIndices = numTangents+2;
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
#ifdef DAMMIT
  m_world.DestroyBody(m_pBody);
#endif
}


void Bullet::Fire(const b2Transform& xf)
{
    //get a direction and a position to start from
    //This should probably be based on the transform
    //from the ship
    b2Vec2 pos  = b2Mul(xf,b2Vec2(0.0f, 0.5f));
#ifdef DAMMIT
    m_pBody->SetTransform(pos,xf.q.GetAngle());
#endif
    //Get/Set the starting timestamp
    m_timestamp = SDL_GetTicks();
    SetActive();
}

void Bullet::Update()
{
#ifdef DAMMIT
    if(m_pBody->IsActive())
    {
        if(m_timestamp - SDL_GetTicks() <= 0)
	{
            SetInactive();
	}
    }
#endif
}

void Bullet::SetActive()
{
#ifdef DAMMIT
    m_pBody->SetActive(true);
#endif
}
void Bullet::SetInactive()
{
#ifdef DAMMIT
  m_pBody->SetActive(false);
#endif
}



void Bullet::Draw()
{
#ifdef DAMMIT
    if(m_pBody->IsActive())
#endif
    {
        glUseProgram(m_shaderProgram);
     
        //get a handle for the rotation uniform;
        GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
        glm::mat4 modelMatrix(1.0f);
        //glm translation vector
#ifdef DAMMIT

        b2Vec2 position = m_pBody->GetWorldCenter();
     
        //box uses radians, GLM uses degrees
        float angleDeg = RAD2DEG(m_pBody->GetAngle());
#endif
        //Translate then Rotate
        modelMatrix = 
//            glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f)); 
            glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); 
//        modelMatrix = 
//            glm::rotate(modelMatrix, angleDeg , m_rotateAxis);    
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
//        glVertexAttribPointer(vertexAttribute, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(vertexAttribute, 4*sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, 0);

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
