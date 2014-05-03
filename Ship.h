#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL_opengles2.h> 


class Ship
{
    struct TempStruct
    {
        GLfloat vertices[4];
        GLfloat colors[4];
    };

public:
    //Default constructor
    Ship();
    ~Ship();
    //The function that actually puts the object on the screen
    void Draw();
    void rotateRight();
    void rotateLeft();
private:
    //only using 1 vertex buffer for this object
    GLuint m_vertexBufferHandleArray[1];    
    GLuint m_colorBufferHandleArray[1];    

    GLuint m_shaderProgram;
    GLuint m_indexBuffer;
    GLuint m_numIndices;
    GLfloat m_angle;
};

#endif //SHIP_H
