#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL_opengles2.h> 
#include <Box2D/Box2D.h>

class Bullet
{
private:
    //reference to the world to use for Box
    b2World& m_worldRef;

    //should destroy this body in the destructor
    b2Body *m_pBody;

public:
    Bullet(b2World& world);
    ~Bullet();
};

#endif
