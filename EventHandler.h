#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H


#include "WiimoteIntf.h"

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

#include "ShipIntf.h"
#include "Walls.h"

//Interface to receive events from some kind of controller object.
class EventHandler : public WiimoteIntf
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
    Walls m_walls;
    void DrawShips() const;
public:
    EventHandler(SDL_Window *pWindow,b2World& world,Ships& ships);

    //Main Event Loop
    void EventLoop();

    //Destructor
    virtual ~EventHandler();
    
    //Accessor functions to add events to the event handler
    virtual void Button_Home(int controllerIndex);
    virtual void ButtonPushed(int controller, int buttons);

    
};



#endif
