#ifndef VORTEX_H
#define VORTEX_H

#include "GLHeader.h"
#include <Box2D/Box2D.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::sca

class Vortex
{
public:
    explicit Vortex(b2World& world);
    void Draw();
    ~Vortex();
    void activate(const b2Vec2& coords);
    b2Vec2 getCoordinates() const;
    void setActive(bool active);
    bool getActive() const;
private:
    const uint32 m_kLifetime;
    b2World& m_world;
    GLuint m_shaderProgram;
    GLuint m_vortexBuffer;
    static const glm::mat4 m_projMat;
    uint32 m_timestamp;
    b2Vec2 m_coords;
    bool m_bActive;
};

#endif
