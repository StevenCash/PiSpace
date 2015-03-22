#include "Star.h"
#include <SDL2/SDL.h>
#include "ShaderUtil.h"
#include <bits/stl_algo.h> //for_each
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const glm::mat4 Star::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));
GLuint Star::m_shaderProgram = 0;

Star::Star(float minX, float maxX, float minY, float maxY):
    m_vertexbuffer(0),
    m_colorbuffer(0),
    m_numVertices(0),
    m_radius(0.5f)
{

    const float fRAND_MAX=float(RAND_MAX);
    m_radius = rand()/fRAND_MAX * 0.5f;


    GLfloat centerx = (rand() / fRAND_MAX) * abs(minX - maxX) + minX ;
    GLfloat centery = (rand() / fRAND_MAX) * abs(minY - maxY) + minY ;

    //Init the shader program to use
    if(!m_shaderProgram)
    {
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "StarVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "StarFragment.glsl"));
        m_shaderProgram = createShaderProgram(shaderList);

        //done with the shaders, so delete them
        //do this here instead of in createShaderProgram so that a shader
        //can be reused without rebuilding it
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    }
    
    if(m_shaderProgram)
    {
         //Setup Position ATTRIBUTES

        GLuint numOutsideVertices=5;
        m_numVertices=numOutsideVertices+2;
        GLuint numValues=m_numVertices*3;
        int i=0;
        GLfloat f[numValues];
        f[0] = centerx;
        f[1] = centery;
        f[2] = 0.0f;

        float increment = (3.14159265 *2)/numOutsideVertices;
        for(float angle=(3.14159265 * 2.0); angle > 0.0f; angle-=increment)
        {
            i+=3;
            f[i]=centerx+sin(angle)*m_radius; 
            f[i+1]=centery+cos(angle)*m_radius;
            f[i+2]=0.0f;
        }
        f[numValues-3] = f[3];
        f[numValues-2] = f[4];
        f[numValues-1] = f[5];

        glGenBuffers ( 1 , &m_vertexbuffer ) ;
        glBindBuffer ( GL_ARRAY_BUFFER , m_vertexbuffer ) ;
        glBufferData ( GL_ARRAY_BUFFER , sizeof ( f ) , f , GL_STATIC_DRAW ) ;
        glBindBuffer(GL_ARRAY_BUFFER,0);


        //Setup COLOR attributes
        GLfloat colorArray[numValues];

        colorArray[0]=1.0f;
        colorArray[1]=1.0f;
        colorArray[2]=1.0f;
        for(GLuint x=3; x < numValues; x+=3)
        {
            colorArray[x]= 0.0f;
            colorArray[x+1]= 0.0f;
            colorArray[x+2]= 0.0f;
        }
        glGenBuffers(1,&m_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorArray), colorArray, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
}

void Star::Draw()
{
    glUseProgram ( m_shaderProgram ) ;
    
    //setup the vertices!
    GLuint positionAttribute = glGetAttribLocation(m_shaderProgram,"aPosition");
    glEnableVertexAttribArray ( positionAttribute ) ;
    glBindBuffer ( GL_ARRAY_BUFFER , m_vertexbuffer ) ;
    glVertexAttribPointer (
        positionAttribute , // attribute 0.
        3 , // size
        GL_FLOAT , // type
        GL_FALSE , // normalized ?
        0 , // stride
        ( void * ) 0 // array buffer offset
        ) ;
    
    //Setup the color(s)!
    glBindBuffer ( GL_ARRAY_BUFFER , m_colorbuffer ) ;
    GLuint colorAttribute = glGetAttribLocation(m_shaderProgram, "aColor");
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer (
        colorAttribute , // attribute 1. 
        3 , // size 
        GL_FLOAT , // type
        GL_FALSE , // normalized ?
        0 , // stride
        ( void * ) 0 // array buffer offset
        ) ;
    
    glm::mat4 modelMatrix(1.0f);    
    GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");

    glm::mat4 mvpMat = m_projMat * modelMatrix;
    glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(mvpMat));
    
    // Draw the octogon!
    glDrawArrays ( GL_TRIANGLE_FAN , 0 , m_numVertices ); 
    glDisableVertexAttribArray ( positionAttribute ) ;
    glDisableVertexAttribArray ( colorAttribute ) ;

}
