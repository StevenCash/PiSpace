#ifndef BULLET_H
#define BULLET_H

#include "ShipIntf.h"
#include "DestroyableIntf.h"
#include "GLHeader.h"
#include <Box2D/Box2D.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::sca

#ifndef RAD2DEG
#define RAD2DEG(x) ((x) * 57.2957795f)
#endif

class Bullet : public DestroyableIntf
{
private:
    static GLuint m_vertexbuffer;
    static GLuint m_colorbuffer;
    static GLuint m_numVertices;

    b2Body *m_pBody;
    b2World& m_world;
    float32 m_radius;

    static const glm::mat4 m_projMat;
    static GLuint m_shaderProgram;

    ShipIntf* m_pShip;
    uint32 m_timestamp;

    //Container to hold the interface to pass to
    //Box2D as UserData, since it's stores it as a void
    //and will lose most of its vtable
    DestroyableIntfContainer m_destroyableContainer;

public:
    Bullet(b2World& world, ShipIntf *pShip, uint32 index);
    ~Bullet();

    void Fire(const b2Transform& xf);
    void Draw();
    
    //override of DestroyableIntf function
    void DestroyObject();
};

#endif
