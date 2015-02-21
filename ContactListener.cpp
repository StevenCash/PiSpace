#include "ContactListener.h"
#include <iostream>
#include "DestroyableIntf.h"
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* /* oldManifold*/)
{
    const b2Body* bodyA = contact->GetFixtureA()->GetBody();
    const b2Body* bodyB = contact->GetFixtureB()->GetBody();    



    if((bodyA->GetType() == b2_staticBody) ||
       (bodyB->GetType() == b2_staticBody))
    {
        std::cout << "Something hit a wall"<< std::endl;
    }
    else
    {
        DestroyableIntf* pDestroy =
            static_cast<DestroyableIntf*>(bodyA->GetUserData());

        if(pDestroy)
        {
            std::cout << "WTF" << std::endl;

        }

/*
  
        DestroyableIntf* pDestroy =
            static_cast<DestroyableIntf*>(bodyA->GetUserData());
        if(pDestroy)
        {
            pDestroy->Destroy();
        }

        pDestroy =
            static_cast<DestroyableIntf*>(bodyB->GetUserData());
        if(pDestroy)
        {
            pDestroy->Destroy();
        }
*/
    }        

}
