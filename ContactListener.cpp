#include "ContactListener.h"
#include <iostream>
#include "DestroyableIntf.h"
#include "EventHandler.h"

ContactListener::ContactListener(EventHandler& eventHandler)
    :m_eventHandler(eventHandler)
{
}


void ContactListener::BeginContact(b2Contact* contact)
{
/*
     b2Body* bodyA = contact->GetFixtureA()->GetBody();
     b2Body* bodyB = contact->GetFixtureB()->GetBody();    

    //Do nothing
    if((bodyA->GetType() != b2_staticBody) &&
       (bodyB->GetType() != b2_staticBody))
    {
        void* pTempA =
            bodyA->GetUserData();
        void* pTempB =
            bodyB->GetUserData();
        if(pTempA != pTempB)
        {
            m_eventHandler.CollisionHappened(bodyA,bodyB);
        }
        
    }
*/
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* /*oldManifold*/)
{
     b2Body* bodyA = contact->GetFixtureA()->GetBody();
     b2Body* bodyB = contact->GetFixtureB()->GetBody();    

    //Do nothing
    if((bodyA->GetType() != b2_staticBody) &&
       (bodyB->GetType() != b2_staticBody))
    {
        void* pTempA =
            bodyA->GetUserData();
        void* pTempB =
            bodyB->GetUserData();
        if(pTempA != pTempB)
        {
            m_eventHandler.CollisionHappened(bodyA,bodyB);
        }
        
    }
}
