#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL.h>
#include <set>

#include "ControllerIntf.h"
#include "Vortex.h"
#include "ShipIntf.h"


class Walls;
class b2World;
class b2Body;


//Interface to receive events from some kind of controller object.
class EventHandler : public ControllerIntf
{
private:
    typedef std::set< std::pair<b2Body*, b2Body*> > CollisionSet;

    bool m_bQuit;
    SDL_Window* m_pWindow;
    Uint32 m_controlEvent;
    SDL_Event m_templateEvent;
    Ships& m_ships;

//Box2D  stuff
    float32 m_timeStep;
    int32 m_velocityIterations;
    int32 m_positionIterations;
    b2World& m_world;
    Walls& m_walls;

    void ProcessShips() const;
    void ProcessEffects();
    uint32 KeyUp(SDL_Scancode scankeycode) const;
    uint32 KeyDown(SDL_Scancode scankeycode) const;

    //set of all unique collision
    CollisionSet m_collisionSet;
    
public:
    EventHandler(SDL_Window *pWindow,b2World& world,Ships& ships, Walls& walls);

//Main Event Loop
    void EventLoop();

//Destructor
    virtual ~EventHandler();
    
//Accessor functions to add events to the event handler
    virtual void ButtonHome(int controllerIndex);
    virtual void ButtonPushed(int controller, int buttons);

    void CollisionHappened(b2Body* bodyA, b2Body* b2BodyB);
    
};



#endif
