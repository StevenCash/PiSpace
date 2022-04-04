#include "Explosion.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <list>
#include <iostream>

const glm::mat4 Explosion::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));

Explosion::Explosion(int index)
    : m_kLifetime(3000),
      m_shaderProgram(0),
      m_explosionBuffer(0),
      m_explosionColorBuffer(0),
      m_timestamp(0),
      m_bActive(false),
      m_offset(5.0f,0.0f)
{
        //Init the shader program to use
    if(!m_shaderProgram)
    {
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "ExplosionVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "ExplosionFragment.glsl"));
        m_shaderProgram = createShaderProgram(shaderList);

        GLfloat red = 0.0f, green=0.0f, blue=0.0f;
        switch(index)
        {
            case 0:
            {
                blue = 1.0f;
                green = 0.0f;
                red = 0.7f;
                break;
            }
            case 1:
            {
                red = 1.0f;
                break;
            }
            case 2:
            {
                green = 1.0f;
                break;
            }
            case 3:
            {
                blue = 1.0f;
                break;
            }
            default:
            {
                std::cerr << "Too many ships." << std::endl;
                break;
                exit(1);
            }
        }


        //Setup colors ********************************
        glm::vec3 colors[] =
        {
            glm::vec3(red,green,blue),
            glm::vec3(1.0f,1.0f,1.0f),
            glm::vec3(red,green,blue),
            glm::vec3(red,green,blue),
            glm::vec3(1.0f,1.0f,1.0f),
            glm::vec3(1.0f,1.0f,1.0f)
        };
        std::list<glm::vec3> colorList;
        //making 8 copies
        for(int j=0; j < 8; ++j)
        {
            for(int i=0; i <6; ++i)
            {
                colorList.push_back(colors[i]);
            }
        }
        std::list<GLfloat> colorListFloat;
        for(std::list<glm::vec3>::const_iterator iter = colorList.begin();
            iter != colorList.end();
            ++iter)
        {
            colorListFloat.push_back(iter->x);
            colorListFloat.push_back(iter->y);
            colorListFloat.push_back(iter->z);
        }

//Changed by SC
		//GLfloat colorArray[colorListFloat.size()];
        GLfloat colorArray[144];
		int colorIndex = 0;
        for(std::list<GLfloat>::const_iterator iter = colorListFloat.begin();
            iter != colorListFloat.end();
            ++iter)
        {
            colorArray[colorIndex] = *iter;
            ++colorIndex;
        }

        glGenBuffers(1, &m_explosionColorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,m_explosionColorBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(colorArray),colorArray,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);





        //Setup vertices *******************************
        glm::vec4 vectors[] = { 
            glm::vec4(-0.025f, 0.05f, 0.0f,1.0f),  //white
            glm::vec4(-0.025f, 0.0f, 0.0f,1.0f), //color
            glm::vec4(0.025f, 0.05f, 0.0f,1.0f), //white
            glm::vec4(0.025f, 0.05f, 0.0f,1.0f), //white
            glm::vec4(-0.025f, 0.0f, 0.0f,1.0f),//color
            glm::vec4(0.025f, 0.0f, 0.0f,1.0f) //color
        };

        std::list<glm::vec4> vectorList;

        //making 8 copies
        for(int j =0; j < 8; ++j)
        {
            for(int i=0; i < 6; ++i)
            {
                vectorList.push_back(vectors[i]);
            }
        }
        //translate
        glm::mat4 transMatrix(1.0f);
        transMatrix = glm::translate(transMatrix,glm::vec3(0.0f,0.4f,0.0f));
        for(std::list<glm::vec4>::iterator veciter =  vectorList.begin();
            veciter!=vectorList.end();
            ++veciter)
        {
            *veciter = transMatrix * (*veciter);
        }
        
        
        //rotate
        glm::mat4 rotMatrix(1.0f);
        int processedVertecies = 0;
        for(std::list<glm::vec4>::iterator veciter =  vectorList.begin();
            veciter!=vectorList.end() ;
            ++veciter, ++processedVertecies)
        {
            if(!(processedVertecies % 6))
            {
                rotMatrix = glm::rotate(rotMatrix, 45.0f,glm::vec3(0.0f,0.0f,1.0f));
            }
            *veciter = rotMatrix * (*veciter);
        }
        
        //convert to an array
        std::list<GLfloat> vertexList;
        for(std::list<glm::vec4>::iterator veciter =  vectorList.begin();
            veciter!=vectorList.end();
            ++veciter)
        {
            vertexList.push_back((*veciter).x);
            vertexList.push_back((*veciter).y);
            vertexList.push_back((*veciter).z);
        }

// Changed by SC
        //GLfloat vertices[vertexList.size()];
        GLfloat vertices[144];
		int tempIndex = 0;
        for(std::list<GLfloat>::const_iterator iter =  vertexList.begin();
            iter != vertexList.end();
            ++iter)
        {
            vertices[tempIndex] = *iter;
            ++tempIndex;
        }

        //Store it for OpenGL
        glGenBuffers(1, &m_explosionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,m_explosionBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }




}

void Explosion::Draw()
{
    if(m_bActive)
    {
        uint32 curTime = SDL_GetTicks();
        uint32 aliveTime = curTime - m_timestamp;
        if(aliveTime > m_kLifetime)
        {
            m_bActive = false;
            return;
        }

        glUseProgram(m_shaderProgram);
        //setup the vertices!
        GLuint positionAttribute = glGetAttribLocation(m_shaderProgram,"aPosition");
        glEnableVertexAttribArray(positionAttribute);
        glBindBuffer ( GL_ARRAY_BUFFER , m_explosionBuffer ) ;
        glVertexAttribPointer (
            positionAttribute , // attribute 0.
            3 , // size
            GL_FLOAT , // type
            GL_FALSE , // normalized ?
            0 , // stride
            ( void * ) 0 // array buffer offset
            ) ;


        GLuint colorAttribute = glGetAttribLocation(m_shaderProgram,"aColor");
        glEnableVertexAttribArray(colorAttribute);
        glBindBuffer ( GL_ARRAY_BUFFER , m_explosionColorBuffer ) ;
        glVertexAttribPointer (
            colorAttribute , // attribute 1.
            3 , // size
            GL_FLOAT , // type
            GL_FALSE , // normalized ?
            0 , // stride
            ( void * ) 0 // array buffer offset
            ) ;

        GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
        GLuint locationTime = glGetUniformLocation(m_shaderProgram,"uTime");
        GLuint locationOffset = glGetUniformLocation(m_shaderProgram,"uOffset");

//        0-1 over m_kLifetime seconds
        GLfloat offset=(float(aliveTime)/float(m_kLifetime));
        glUniform1f(locationTime, offset);

        glUniform2fv(locationOffset,1,glm::value_ptr(m_offset));

        glm::mat4 modelMatrix1(1.0f);
        modelMatrix1 = m_projMat * modelMatrix1;
        glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(modelMatrix1));

        glDrawArrays ( GL_TRIANGLES , 0 , 48 ); 

    }
}


Explosion::~Explosion()
{
    glDeleteBuffers(1,&m_explosionBuffer);
}


void Explosion::setActive(bool active)
{
    m_bActive = active;
    m_timestamp=SDL_GetTicks();
}

bool Explosion::getActive() const
{
    return m_bActive;
}

void Explosion::setPosition(const b2Transform& xf)
{
    m_offset.x = xf.p.x;
    m_offset.y = xf.p.y;
}
