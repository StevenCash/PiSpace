#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "GLHeader.h"
#include <Box2D/Box2D.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::sca

class Explosion
{
public:
    explicit Explosion();
    void Draw();
    ~Explosion();
    void setActive(bool active);
    bool getActive() const;
private:
    const uint32 m_kLifetime;
    GLuint m_shaderProgram;
    GLuint m_explosionBuffer;
    GLuint m_explosionColorBuffer;
    static const glm::mat4 m_projMat;
    uint32 m_timestamp;
    bool m_bActive;
};

#endif
