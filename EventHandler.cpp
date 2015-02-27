#include <typeinfo>
#include "GLHeader.h"
#include <iostream>
#include <Box2D/Box2D.h>
#include "EventHandler.h"
#include "Walls.h"
#include "DestroyableIntf.h"
#include "TraceLogger.h"

EventHandler::EventHandler(SDL_Window *pWindow, b2World& world, Ships& ships, Walls& walls):
    m_bQuit(false),
    m_pWindow(pWindow),
    m_ships(ships),
    m_timeStep(1.0f / 60.0f),
    m_velocityIterations(6),
    m_positionIterations(2),
    m_world(world),
    m_walls(walls)
{
    //Setup a new SDL event to handle controls from whatever source
    //currently, WiiMote
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
                            break;
                        case SDL_SCANCODE_Q: //Stop the ship exactly where it is,
                        case SDL_SCANCODE_R: //Reset to 0 position
                        case SDL_SCANCODE_A: //left
                        case SDL_SCANCODE_D: //right
                        case SDL_SCANCODE_W: //up
                        case SDL_SCANCODE_S: //shoot
                        case SDL_SCANCODE_E: //bomb
                            pShip=m_ships[0];
                            break;

                        case SDL_SCANCODE_U: //Stop the ship exactly where it is,
                        case SDL_SCANCODE_P: //Reset to 0 position
                        case SDL_SCANCODE_J: //left
                        case SDL_SCANCODE_L: //right
                        case SDL_SCANCODE_I: //up
                        case SDL_SCANCODE_K: //shoot
                        case SDL_SCANCODE_O: //bomb
                            if(m_ships.size() > 1)
                            {
                                pShip = m_ships[1];
                            }
                            break;
                        default:
                            break;
                    };
                    if(pShip)
                    {
                        command=KeyDown(event.key.keysym.scancode);
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
                        case SDL_SCANCODE_Q: //Stop the ship exactly where it is,
                        case SDL_SCANCODE_R: //Reset to 0 position
                        case SDL_SCANCODE_A: //left
                        case SDL_SCANCODE_D: //right
                        case SDL_SCANCODE_W: //up
                        case SDL_SCANCODE_S: //shoot
                        case SDL_SCANCODE_E: //bomb
                            pShip=m_ships[0];
                            break;
                        case SDL_SCANCODE_U: //Stop the ship exactly where it is,
                        case SDL_SCANCODE_P: //Reset to 0 position
                        case SDL_SCANCODE_J: //left
                        case SDL_SCANCODE_L: //right
                        case SDL_SCANCODE_I: //up
                        case SDL_SCANCODE_K: //shoot
                        case SDL_SCANCODE_O: //bomb
                            if(m_ships.size() > 1)
                            {
                                pShip = m_ships[1];
                            }
                            break;
                        default:
                            break;
                    };
                    if(pShip)
                    {
                        command=KeyUp(event.key.keysym.scancode);
                        pShip->ProcessInput(command);
                    }   

                }
                break;
                case SDL_USEREVENT:
                {
                    //All of the user event types should have a ship pointer
                    ShipIntf *pShip = reinterpret_cast<ShipIntf*>(event.user.data1);
                    pShip->ProcessInput(event.user.code);
                }                    
                break;
                default:
                    break;
            }
        }
        
        //Do the physics stepping
        m_world.Step(m_timeStep, m_velocityIterations, m_positionIterations);
        m_world.ClearForces();

        ProcessEffects();
        
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Draw objects that need to be drawn
        m_walls.Draw();

        //Ships store/draw their own bullets
        ProcessShips();
        
        //Display the back buffer
        SDL_GL_SwapWindow(m_pWindow);
        
        //Sleep to not eat the machine
        SDL_Delay(60);
    }

}

//Destructor
EventHandler::~EventHandler()
{
};
    

/*******************************************/
void EventHandler::ProcessShips() const
{
    for(Ships::const_iterator iter = m_ships.begin(), endIter = m_ships.end();
        iter != endIter;
        ++iter)
    {
        (*iter)->DoCommands();
        (*iter)->Draw();
    }
        
}            


void EventHandler::ProcessEffects()
{

    //Handle all the collisions that occurred
    if(!m_collisionSet.empty())
    {
        CollisionSet::iterator iter = m_collisionSet.begin();
        for(;iter != m_collisionSet.end(); ++iter)
        {
            std::pair<b2Body*, b2Body*> collisionPair =
                (*iter);
            void *pUserData = collisionPair.first->GetUserData();
            if(pUserData)
            {
                static_cast<DestroyableIntfContainer*>(pUserData)->pDestroyable->DestroyObject();
            }
            pUserData = collisionPair.second->GetUserData();
            if(pUserData)
            {
                static_cast<DestroyableIntfContainer*>(pUserData)->pDestroyable->DestroyObject();
            }
            
        }
        m_collisionSet.clear();
    }
}


/*******************************************/
//Controls
void EventHandler::ButtonHome(int  /* controllerIndex */)
{
    m_templateEvent.type = SDL_QUIT;
    SDL_PushEvent(&m_templateEvent);
}

    
void EventHandler::ButtonPushed(int controller, int buttons)
{
    m_templateEvent.user.code = buttons;
    m_templateEvent.user.data1 = m_ships[controller];
    SDL_PushEvent(&m_templateEvent);
}

uint32 EventHandler::KeyDown(SDL_Scancode scankeycode) const
{

    uint32 command = 0;
    switch(scankeycode)
    {
        case SDL_SCANCODE_Q: //Stop the ship exactly where it is,
        case SDL_SCANCODE_U: //Stop the ship exactly where it is,
            command |= SHIP_STOP;
            break;
        case SDL_SCANCODE_R: //Reset to 0 position
        case SDL_SCANCODE_P: //Reset to 0 position
            command |= SHIP_RESET;
            break;
        case SDL_SCANCODE_A: //left
        case SDL_SCANCODE_J: //left
            command |= SHIP_CCW;
            break;
        case SDL_SCANCODE_D: //right
        case SDL_SCANCODE_L: //right
            command |= SHIP_CW;
            break;
        case SDL_SCANCODE_W: //up
        case SDL_SCANCODE_I: //up
            command |= SHIP_FORWARD;
            break;
        case SDL_SCANCODE_S: //shoot
        case SDL_SCANCODE_K: //shoot
            command |= SHIP_SHOOT;
            break;
        case SDL_SCANCODE_E: //bomb
        case SDL_SCANCODE_O: //bomb
            command |= SHIP_BOMB;
            break;
        default:
            //do nothing
            break;
    }
    return command;
}


uint32 EventHandler::KeyUp(SDL_Scancode scankeycode) const
{
    uint32 command = 0;
    switch(scankeycode)
    {
        case SDL_SCANCODE_Q: //Stop the ship exactly where it is,
        case SDL_SCANCODE_U: //Stop the ship exactly where it is,
            command &= ~SHIP_STOP;
            break;
        case SDL_SCANCODE_R: //Reset to 0 position
        case SDL_SCANCODE_P: //Reset to 0 position
            command &= ~SHIP_RESET;
            break;
        case SDL_SCANCODE_A: //left
        case SDL_SCANCODE_J: //left
            command &= ~SHIP_CCW;
            break;
        case SDL_SCANCODE_D: //right
        case SDL_SCANCODE_L: //right
            command &= ~SHIP_CW;
            break;
        case SDL_SCANCODE_W: //up
        case SDL_SCANCODE_I: //up
            command &= ~SHIP_FORWARD;
            break;
        case SDL_SCANCODE_S: //shoot
        case SDL_SCANCODE_K: //shoot
            command &= ~SHIP_SHOOT;
            break;
        case SDL_SCANCODE_E: //bomb
        case SDL_SCANCODE_O: //bomb
            command &= ~SHIP_BOMB;
            break;
        default:
            //do nothing
            break;
    };
    return command;
}


//Put anything that collided into a set
void EventHandler::CollisionHappened(b2Body* bodyA, b2Body* bodyB)
{
    //order the pairs that go in so that there can be no duplicates
    //with just the order changed.
    if(bodyA < bodyB)
    {
        m_collisionSet.insert(std::pair<b2Body*,b2Body*>(bodyA,bodyB));
    }
    else
    {
        m_collisionSet.insert(std::pair<b2Body*,b2Body*>(bodyB,bodyA));
    }
}
