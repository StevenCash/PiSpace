#ifndef VORTEX_H
#define VORTEX_H

#include <Box2D/Box2D.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::sca
#include <SDL2/SDL_opengles2.h> 

class Vortex
{
public:
    explicit Vortex(b2World& world);
    void Draw();
    ~Vortex();

private:
    b2World& m_world;
    GLuint m_shaderProgram;
    GLuint m_vortexBuffer;
    static const glm::mat4 m_projMat;
    uint32 m_timestamp;
};

#endif
