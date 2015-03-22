#ifndef STAR_H
#define STAR_H

#include "GLHeader.h"
#include <glm/glm.hpp>

class Star
{
public:
//Constructor
//takes screen size to calculate the star density
    explicit Star(float minX, float maxX, float minY, float maxY);
    void Draw();
private:
    static const glm::mat4 m_projMat;
    static GLuint m_shaderProgram;
    GLuint m_vertexbuffer;  
    GLuint m_colorbuffer;
    int m_numVertices;
    GLfloat m_radius;


};

#endif
