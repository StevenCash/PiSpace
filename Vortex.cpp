#include "Vortex.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <iostream>
const glm::mat4 Vortex::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));

Vortex::Vortex(b2World& world)
    : m_kLifetime(10000),
      m_world(world),
      m_shaderProgram(0),
      m_vortexBuffer(0),
      m_timestamp(0),
      m_coords(0.0f,0.0f),
      m_bActive(false)
{
        //Init the shader program to use
    if(!m_shaderProgram)
    {
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "VortexVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "VortexFragment.glsl"));
        m_shaderProgram = createShaderProgram(shaderList);
        
        GLfloat vertices[] =
        {
            -0.10f, 0.06f,
            -0.10f, -0.06f,

             0.10f,  0.06f,
             0.10f, -0.06f,

            -0.06f, 0.10f,
             0.06f, 0.10f,

            -0.06f, -0.10f,
             0.06f, -0.10f,
        };
        
        glGenBuffers(1, &m_vortexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,m_vortexBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
}

void Vortex::Draw()
{
    if(m_bActive)
    {
        uint32 curTime = SDL_GetTicks();
        uint32 aliveTime = curTime - m_timestamp;
        if(curTime - m_timestamp > m_kLifetime)
        {
            m_bActive = false;
            return;
        }

        b2Body* pBody = m_world.GetBodyList();
        while(pBody)
        {

            if(pBody->IsActive())
            {
                b2Vec2 direction = (m_coords - pBody->GetWorldCenter());
                //don't do anything if the object is right on top
                //of the vortex
                if(direction.x != 0 && direction.y != 0)
                {
                    glm::vec2 glmvec(direction.x, direction.y);
                    
                    glmvec = glm::normalize(glmvec);
                    glmvec *= 2.0f;
                    pBody->ApplyForceToCenter(b2Vec2(glmvec.x, glmvec.y),true);
                }
            }

            pBody=pBody->GetNext();
        }
        
        float angle = 30.0f * (float(aliveTime)/1000.0f);

        if(angle > 360)
        {
            m_timestamp=SDL_GetTicks();
        }
        glUseProgram(m_shaderProgram);
        //setup the vertices!
        GLuint positionAttribute = glGetAttribLocation(m_shaderProgram,"aPosition");
        glEnableVertexAttribArray(positionAttribute);
        glBindBuffer ( GL_ARRAY_BUFFER , m_vortexBuffer ) ;
        glVertexAttribPointer (
            positionAttribute , // attribute 0.
            2 , // size
            GL_FLOAT , // type
            GL_FALSE , // normalized ?
            0 , // stride
            ( void * ) 0 // array buffer offset
            ) ;

        GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
        GLuint locationFade = glGetUniformLocation(m_shaderProgram,"uFade");
        
        GLfloat fade=1.0f - (float(aliveTime)/float(m_kLifetime));

        glUniform1f(locationFade, fade);
    
        glm::mat4 modelMatrix1(1.0f);
        modelMatrix1 =
            glm::translate(modelMatrix1,glm::vec3(m_coords.x,m_coords.y,0.0f));
        modelMatrix1=
            glm::rotate(modelMatrix1, angle ,glm::vec3(0.0f,0.0f,1.0f));
        modelMatrix1 = m_projMat * modelMatrix1;
        glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(modelMatrix1));

        glDrawArrays ( GL_LINES , 0 , 8 ); 

        glm::mat4 modelMatrix2(1.0f);
        modelMatrix2 =
            glm::translate(modelMatrix2,glm::vec3(m_coords.x,m_coords.y,0.0f));
        modelMatrix2 =
            glm::rotate(modelMatrix2, angle ,glm::vec3(0.0f,0.0f,-1.0f));
        modelMatrix2=
            glm::scale(modelMatrix2, glm::vec3(5.0f,5.0f,0.0f));
        modelMatrix2 = m_projMat * modelMatrix2;
        glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(modelMatrix2));
        glDrawArrays ( GL_LINES , 0 , 8 ); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}


Vortex::~Vortex()
{
    glDeleteBuffers(1,&m_vortexBuffer);
}

void Vortex::activate(const b2Vec2& coords) 
{
    m_coords=coords;
    m_timestamp=SDL_GetTicks();
    setActive(true);
}

b2Vec2 Vortex::getCoordinates() const
{
    return m_coords;
}

void Vortex::setActive(bool active)
{
    m_bActive = active;
}

bool Vortex::getActive() const
{
    return m_bActive;
}
