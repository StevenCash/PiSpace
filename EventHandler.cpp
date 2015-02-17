
#include "EventHandler.h"


#include <SDL2/SDL_opengles2.h> 
#include <iostream>

EventHandler::EventHandler(SDL_Window *pWindow, b2World& world, Ships& ships):
    m_bQuit(false),
    m_pWindow(pWindow),
    m_ships(ships),
    m_timeStep(1.0f / 60.0f),
    m_velocityIterations(6),
    m_positionIterations(2),
    m_world(world),
    m_walls(world)

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
                case SDL_KEYDOWN:
                {
                    ShipIntf *pShip = 0;
                    uint32 command = 0;
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_X:
                            m_bQuit=true;
                            std::cerr << "X pressed" << std::endl;
                            break;
                        case SDL_SCANCODE_A: //left
                            pShip = m_ships[0];
                            command |= SHIP_CCW;
                            break;
                        case SDL_SCANCODE_D: //right
                            pShip = m_ships[0];
                            command |= SHIP_CW;
                            break;
                        case SDL_SCANCODE_W: //up
                            pShip = m_ships[0];
                            command |= SHIP_FORWARD;
                            break;
                        case SDL_SCANCODE_S: //shoot
                            pShip = m_ships[0];
                            command |= SHIP_SHOOT;
                            break;
                        case SDL_SCANCODE_K:
                            if(m_ships.size() > 1)
                            {
                                pShip = m_ships[1];
                                command |= SHIP_SHOOT;
                            }
                            break;
                        default:
                            break;
                    };
                    if(pShip)
                    {
                        pShip->ProcessInput(command);
                    }   
                }
                break;
                case SDL_KEYUP:
                {
                    ShipIntf *pShip = 0;
                    uint32 command = 0;
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_A: //left
                            pShip = m_ships[0];
                            command &= ~SHIP_CCW;
                            break;
                        case SDL_SCANCODE_D: //right
                            pShip = m_ships[0];
                            command &= ~SHIP_CW;
                            break;
                        case SDL_SCANCODE_W: //up
                            pShip = m_ships[0];
                            command &= ~SHIP_FORWARD;
                            break;
                        case SDL_SCANCODE_S: //shoot
                            pShip = m_ships[0];
                            command &= ~SHIP_SHOOT;
                            break;
                        case SDL_SCANCODE_K:
                            if(m_ships.size()  > 1)
                            {
                                pShip = m_ships[1];
                                command &= ~SHIP_SHOOT;
                            }
                            break;
                        default:
                            break;
                    };
                    if(pShip)
                    {
                        pShip->ProcessInput(command);
                    }   

                }
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_walls.Draw();
        DrawShips();
        SDL_GL_SwapWindow(m_pWindow);
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

    for(Ships::const_iterator iter = m_ships.begin(), endIter = m_ships.end();
        iter != endIter;
        ++iter)
    {
        (*iter)->DoCommands();
        (*iter)->Draw();
    }
        
}            


    
void EventHandler::ButtonPushed(int controller, int buttons)
{
    m_templateEvent.user.code = buttons;
    m_templateEvent.user.data1 = m_ships[controller];
    SDL_PushEvent(&m_templateEvent);
}
