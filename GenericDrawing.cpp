#include "GenericDrawing.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GenericDrawing::GenericDrawing():
    m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f))
{
    ShaderList_T shaderList;
    shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "GenericVertex.glsl"));
    shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "GenericFragment.glsl"));

    m_shaderProgram = createShaderProgram(shaderList);
    
    //done with the shaders, so delete them
    //do this here instead of in createShaderProgram so that a shader
    //can be reused without rebuilding it
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    m_vertexAttribute = glGetAttribLocation(m_shaderProgram,"vPosition");
    m_locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
    m_locationColor = glGetUniformLocation(m_shaderProgram,"color");
}

void GenericDrawing::Draw(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

    GLfloat glPositions[vertexCount][4];
   

    for (int i =0; i < vertexCount ; ++i)
    {
        glPositions[i][0] = vertices[i].x;
        glPositions[i][1] = vertices[i].y;
        glPositions[i][2] = 0.0f;
        glPositions[i][3] = 1.0f;
    }
    
    int drawType = GL_LINE_LOOP;
    if(vertexCount == 2)
    {
        drawType = GL_LINES;
    }
    
    glUseProgram(m_shaderProgram);
    
    glUniformMatrix4fv(m_locationMVP, 1, GL_FALSE, glm::value_ptr(m_projMat)); 
    glm::vec4 colorVec;
    glUniform4f(m_locationColor,color.r,color.g,color.b,1.0f);

    glEnableVertexAttribArray(m_vertexAttribute);
    glVertexAttribPointer(m_vertexAttribute,
                          4, //4 dimensions per vertex
                          GL_FLOAT,
                          GL_FALSE,
                          (0),
                          (GLfloat*) glPositions);
    
    glDrawArrays(drawType,0, vertexCount);
    glDisableVertexAttribArray(m_vertexAttribute);
    glUseProgram(0);
}



