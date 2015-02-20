#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H


#include "ControllerIntf.h"

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

#include "Vortex.h"

#include "ShipIntf.h"
class Walls;

//Interface to receive events from some kind of controller object.
class EventHandler : public ControllerIntf
{
private:
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
    void ProcessEffects() const;
public:
    EventHandler(SDL_Window *pWindow,b2World& world,Ships& ships, Walls& walls);

    //Main Event Loop
    void EventLoop();

    //Destructor
    virtual ~EventHandler();
    
    //Accessor functions to add events to the event handler
    virtual void ButtonHome(int controllerIndex);
    virtual void ButtonPushed(int controller, int buttons);

    
};



#endif
