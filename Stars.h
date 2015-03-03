#ifndef STARS_H
#define STARS_H

#include "GLHeader.h"

class Stars
{
public:
    //Constructor
    //takes screen size to calculate the star density
    Stars(int screenx, int screeny);
    void Draw();
private:
    GLuint m_vertexBufferHandleArray[1];    
    static GLuint m_shaderProgram;

};

#endif
