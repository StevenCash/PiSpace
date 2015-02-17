#include "Vortex.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

const glm::mat4 Vortex::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));

Vortex::Vortex(b2World& world)
    :m_world(world),
     m_shaderProgram(0),
     m_vortexBuffer(0),
     m_timestamp(SDL_GetTicks())
{
        //Init the shader program to use
    if(!m_shaderProgram)
    {
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "WallVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "WallFragment.glsl"));
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
    float angle = 20.0f * ((SDL_GetTicks() - m_timestamp)/1000.0f);
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

    glm::mat4 modelMatrix1(1.0f);
    modelMatrix1=
        glm::rotate(modelMatrix1, angle ,glm::vec3(0.0f,0.0f,1.0f));
    modelMatrix1 = m_projMat * modelMatrix1;
    GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
    glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(modelMatrix1));
    glDrawArrays ( GL_LINES , 0 , 8 ); 

    glm::mat4 modelMatrix2(1.0f);
    modelMatrix2=
        glm::scale(modelMatrix2, glm::vec3(2.0f,2.0f,0.0f));
    modelMatrix2 =
        glm::rotate(modelMatrix2, angle ,glm::vec3(0.0f,0.0f,-1.0f));
    modelMatrix2 = m_projMat * modelMatrix2;
    glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(modelMatrix2));
    glDrawArrays ( GL_LINES , 0 , 8 ); 
    


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Vortex::~Vortex()
{
    glDeleteBuffers(1,&m_vortexBuffer);
}
