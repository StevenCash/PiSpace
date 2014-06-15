#ifndef GENERIC_DRAWING
#define GENERIC_DRAWING

#include <SDL2/SDL_opengles2.h> 
#include <Box2D/Box2D.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class GenericDrawing
{
public:
    GenericDrawing();
    void Draw(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
private:
    GLuint m_shaderProgram;
    GLuint m_vertexAttribute;
    GLuint m_locationMVP;
    GLuint m_locationColor;
    const glm::mat4 m_projMat;
};

#endif
