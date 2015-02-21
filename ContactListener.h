#ifndef CONTACTLISTENER
#define CONTACTLISTENER

#include <Box2D/Box2D.h>

class ContactListener : public b2ContactListener
{
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif
