
#include "EventHandler.h"

#include "GLHeader.h"
#include <iostream>
#include "Walls.h"

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
                            pShip = m_ships[0];
                            command |= SHIP_STOP;
                            break;
                        case SDL_SCANCODE_R: //Reset to 0 position
                            pShip = m_ships[0];
                            command |= SHIP_RESET;
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
                        case SDL_SCANCODE_E:
                            pShip = m_ships[0];
                            command |= SHIP_BOMB;
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
                        case SDL_SCANCODE_E:
                            pShip = m_ships[0];
                            command &= ~SHIP_BOMB;
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
                }                    
                break;
                default:
                    break;
            }
        }
        
        //Do the physics stepping
        m_world.Step(m_timeStep, m_velocityIterations, m_positionIterations);
        m_world.ClearForces();
        
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Draw objects that need to be drawn
        m_walls.Draw();
        ProcessEffects();
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

void EventHandler::ProcessEffects() const
{
    /*
    //Right now, this is for handling the "vortex" (later to be a deployable)
    //get all "vortexes"
    //for each vortex, go through the list of affected b2Body objects
    //(which I guess is every non-static body)
    b2Vec2 vortexPos = m_vortex.getCoordinates();
    //get the list of box2D bodies from the world.

    //Start with a Box2D type, to do the math, convert it to 
    //a GLM type to do the normalizing,
    //then back to Box2D to apply the force
    b2Body* pBody = m_world.GetBodyList();
    while(pBody)
    {
        if(pBody->IsActive())
        {
            b2Vec2 direction = (vortexPos - pBody->GetWorldCenter());
            glm::vec2 glmvec(direction.x, direction.y);
            glmvec = glm::normalize(glmvec);
            glmvec *= 2.0f;
            pBody->ApplyForceToCenter(b2Vec2(glmvec.x, glmvec.y),true);
        }
        pBody=pBody->GetNext();
    }
    */
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
