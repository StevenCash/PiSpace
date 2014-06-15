#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H


#include "EventHandlerIntf.h"

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

#include "ShipIntf.h"

//Interface to receive events from some kind of controller object.
class EventHandler : public EventHandlerIntf
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

    enum
    {
        CONTROL_TURN_CW,
        CONTROL_TURN_CCW,
        CONTROL_FORWARD,
        CONTROL_FIRE_1,
        CONTROL_FIRE_2        
    };

    void DrawShips() const;
public:
    EventHandler(SDL_Window *pWindow,b2World& world,Ships& ships);

    //Main Event Loop
    void EventLoop();

    //Destructor
    virtual ~EventHandler();
    
    //Accessor functions to add events to the event handler
    //Currently, these map to Wiimote buttons
    //Should really be functionality based
    virtual void TurnCW(int controllerIndex);
    virtual void TurnCCW(int controllerIndex);
    virtual void Forward(int controllerIndex);
    virtual void Fire_1(int controllerIndex);
    virtual void Fire_2(int controllerIndex);
    virtual void Quit();
    
};



#endif
