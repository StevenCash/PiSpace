
#include "EventHandler.h"


#include <SDL2/SDL_opengles2.h> 



EventHandler::EventHandler(SDL_Window *pWindow, b2World& world, Ships& ships):
    m_bQuit(false),
    m_pWindow(pWindow),
    m_ships(ships),
    m_timeStep(1.0f / 60.0f),
    m_velocityIterations(6),
    m_positionIterations(2),
    m_world(world)

{
    m_controlEvent = SDL_RegisterEvents(1);
    SDL_zero(m_templateEvent);
    m_templateEvent.type = SDL_USEREVENT; 
    m_templateEvent.user.type = m_controlEvent;
}

//Main Event Loop
void EventHandler::EventLoop()
{
    SDL_Event event;

    while(!m_bQuit)
    {
        if(SDL_PollEvent(&event))
        {          
            switch(event.type)
            {
            case SDL_QUIT:
                m_bQuit=true;
                break;
            case SDL_USEREVENT:
            {
                //All of the user event types should have a ship pointer
                ShipIntf *pShip = reinterpret_cast<ShipIntf*>(event.user.data1);
                pShip->ProcessInput(event.user.code);
                break;
            }                    
            default:
                break;
            }
        }
        
        m_world.Step(m_timeStep, m_velocityIterations, m_positionIterations);

        DrawShips();
        SDL_Delay(100);
    }

}

//Destructor
EventHandler::~EventHandler()
{
};
    

void EventHandler::Button_Home(int  /* controllerIndex */)
{
    m_templateEvent.type = SDL_QUIT;
    SDL_PushEvent(&m_templateEvent);
}



void EventHandler::DrawShips() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DEBUG
//    m_world.DrawDebugData();
#endif

    for(Ships::const_iterator iter = m_ships.begin(), endIter = m_ships.end();
        iter != endIter;
        ++iter)
    {
        (*iter)->DoCommands();
        (*iter)->Draw();
    }
    SDL_GL_SwapWindow(m_pWindow);
        
}            


    
void EventHandler::ButtonPushed(int controller, int buttons)
{
    m_templateEvent.user.code = buttons;
    m_templateEvent.user.data1 = m_ships[controller];
    SDL_PushEvent(&m_templateEvent);
}
