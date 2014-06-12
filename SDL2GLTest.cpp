//SDL (2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h> 

#include <iostream>
#include <fstream>
#include <string>

//Wii stuff
#include <bluetooth/bluetooth.h>
#include <cwiid.h>
#include "Wiimote.h"

//Box 2D
#include <Box2D/Box2D.h>
#include "Ship.h"
#include "DebugDraw.h"

//function prototype
void setupDisplay(int screenx=1920, int screeny=1080, int flags=0);
void setupWiimote(int index);
void setupWalls();

/* Wiimote Callback */
cwiid_mesg_callback_t cwiid_callback;

//global variables to handle the SDL display
SDL_Window *pWindow = 0;
SDL_GLContext glContext;


//new event for SDL
Uint32 wiiButtonEvent;
Uint32 wiiAccelEvent;

//setup a constant for the equivalent of BD_ADDR_ANY
//because the one in bluetooth.h is stupid
const bdaddr_t kBdAddrAny = {{0, 0, 0, 0, 0, 0}};

cwiid_wiimote_t *wiimote[2] = {NULL,NULL};


//World for use with Box2D with no gravity
//positive 10.0 is up
//negative 10.0 is down
//positive is right,
//negative is left
b2World World(b2Vec2(0.0f,0.0f));


int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        std::cout << argv[1] << std::endl;
        std::string argument = argv[1];
        if(argument == "-debug")
        {
            //set up some paramters or something 
        }
    }

    setupDisplay();

    wiiButtonEvent = SDL_RegisterEvents(1);
    wiiAccelEvent = SDL_RegisterEvents(1);

    DebugDraw debugDraw;
    debugDraw.SetFlags(0xFFFF);
    //initial display
    World.SetDebugDraw(&debugDraw);
/*NOSHIP*/    Ship ship(World);
#ifndef DEBUG
    ship.Draw();
#endif

    SDL_GL_SwapWindow(pWindow);

//    setupWiimote(0);
//    if(argc > 1)
//    {
//        setupWiimote(1);
//    }

    Wiimote foo;

    setupWalls();

    //Box timing stuff
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    bool bQuit = false;
    SDL_Event event;

    //Set it up so it automatically quits after 10 seconds
    int timestamp = SDL_GetTicks();    
    while(!bQuit)
    {
        if(SDL_PollEvent(&event))
        {          
            switch(event.type)
            {
            case SDL_QUIT:
                bQuit=true;
                break;
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_q:
                {
                    bQuit = true;
                    break;
                }
                case SDLK_d:
                {
                    /*NOSHIP*/ ship.rotateRight();
                    break;
                }
                case SDLK_a:
                {
                    /*NOSHIP*/ ship.rotateLeft();
                    break;
                }
                case SDLK_w:
                    break;
                case SDLK_s:
                    break;
                default:
                    //do nothing
                    break;
                }
                break;
            }

            case SDL_USEREVENT:
            {
                if(event.user.type == wiiButtonEvent)
                {
                    if(event.user.code & CWIID_BTN_UP)
                    {
                        /*NOSHIP*/ ship.translateLeft();
                    }
                    if(event.user.code & CWIID_BTN_DOWN)
                    {
                        /*NOSHIP*/ ship.translateRight();
                    }
                    if(event.user.code & CWIID_BTN_RIGHT)
                    {
                        /*NOSHIP*/ ship.translateUp();
                    }
                    if(event.user.code & CWIID_BTN_LEFT)
                    {
                        /*NOSHIP*/ ship.translateDown();
                    }
                    if(event.user.code & CWIID_BTN_HOME)
                    {
                        bQuit = true;
                    }
                    if(event.user.code & CWIID_BTN_1)
                    {
                        /*NOSHIP*/ ship.rotateRight();                    }
                    if(event.user.code & CWIID_BTN_2)
                    {
                        /*NOSHIP*/ ship.rotateLeft();
                    }
                    if(event.user.code & CWIID_BTN_PLUS)
                    {
                        /*NOSHIP*/ ship.stop();
                    }

                }
                else if(event.user.type == wiiAccelEvent)
                {
                    if(event.user.code > 140)
                    {
                        /*NOSHIP*/ ship.rotateRight();
                    }
                    else if (event.user.code < 120)
                    {
                        /*NOSHIP*/ ship.rotateLeft();
                    }
                }
                break;
            }                    
            default:
                break;
            }
        }
        World.Step(timeStep, velocityIterations, positionIterations);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*NOSHIP*/ ship.Draw();
#ifdef DEBUG
        World.DrawDebugData();
#endif
        SDL_GL_SwapWindow(pWindow);
        SDL_Delay(100);
        
    }
    


    //clean up everything set up by the display
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(pWindow);
    pWindow = 0;
    cwiid_close(wiimote[0]);
    SDL_Quit();
    std::cout << "Finished" << std::endl;
}




//****************************************************************
//Basic SDL2 setup of a screen for OpenGL
void setupDisplay(int screenx, int screeny, int flags)
{
    int FLAGS = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
    if (flags != 0)
    {
        FLAGS=flags;
    }

    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        std::cout << "Unable to init SDL" << std::endl;
        exit(1);
    }

    SDL_DisableScreenSaver();
    pWindow = SDL_CreateWindow(
        "SDL2 OpenGL Test program", 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenx,
        screeny,
        FLAGS);

    if(!pWindow)
    {
        std::cerr << "Unable to create SDL window" << std::endl;
        SDL_Quit();
    }
        
    glContext = SDL_GL_CreateContext(pWindow);
    //just use the max size for the GL Viewport
    glViewport(0,0,1920,1080);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



void cwiid_callback(
    cwiid_wiimote_t *wiimote, 
    int mesg_count,
    union cwiid_mesg mesg_array[], 
    struct timespec *timestamp)
{
    for (int i=0; i < mesg_count; ++i) 
    {
        switch (mesg_array[i].type) 
        {
	case CWIID_MESG_BTN:
        {
            //create a new SDL event with the button state
            SDL_Event event;
            SDL_zero(event);
            event.type = SDL_USEREVENT; 
            event.user.type = wiiButtonEvent;
            event.user.code = mesg_array[i].btn_mesg.buttons;
            SDL_PushEvent(&event);
            break;
        }
        case CWIID_MESG_ACC:
        {
#ifdef WII_ACC
            //Y Axis is good for holding the remote sideways and rotating
            std::cout << (int) mesg_array[i].acc_mesg.acc[CWIID_Y] << std::endl;
            //Seems to rang from 105 to about 155 (as int)

            //only add event if there isn't currently an accel event on the
            //queue
            
            std::cout << (int) mesg_array[i].acc_mesg.acc[CWIID_Y] << std::endl;
            SDL_Event event;
            SDL_zero(event);
            event.type = SDL_USEREVENT; 
            event.user.type = wiiAccelEvent;
            event.user.code = mesg_array[i].acc_mesg.acc[CWIID_Y];
            SDL_PushEvent(&event);
#endif
            break;
        }
        default:
            //do nothing
            break;
        }
    }
}


/********************/
void setupWiimote(int index)
{
    //Use new Wiimote object to connect to wiimotes

    {
        
        Wiimote foo;
//        Wiimote bar;
        SDL_Delay(30000);
    }


    bdaddr_t bdaddr = kBdAddrAny;

    std::cout << "Put wiimote into discovery mode (press 1+2)" << std::endl;
    wiimote[index] = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC);
    if(wiimote[index] == NULL)
    {
        std::cerr << "No connection. Quitting" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Connected" << std::endl;
        cwiid_command(wiimote[index], CWIID_CMD_LED, CWIID_LED1_ON);
    }


    if (cwiid_set_mesg_callback(wiimote[index], &cwiid_callback))
    {
        std::cerr << "Error setting callback.  Exitting" << std::endl;
        cwiid_close(wiimote[index]);
        exit(2);
    }

    std::cout << "WiiMote " << index << " connected" << std::endl;
    cwiid_command(wiimote[index], CWIID_CMD_RPT_MODE, CWIID_RPT_BTN);
}


void setupWalls()
{
    b2BodyDef bodyDef;

    //Top
    bodyDef.position.Set(-10.0f, 9.5f);
    b2Body* topBody = World.CreateBody(&bodyDef);
    b2EdgeShape topEdge;
    topEdge.Set(b2Vec2(0.0, 0.0), b2Vec2(20.0,0.0));
    topBody->CreateFixture(&topEdge, 0.0f);
    
      
    //left
    bodyDef.position.Set(-9.5f, 10.0f);
    b2Body* leftBody = World.CreateBody(&bodyDef);
    b2EdgeShape leftEdge;
    leftEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f,-20.0f));
    leftBody->CreateFixture(&leftEdge, 0.0f);

    //bottom
    bodyDef.position.Set(-10.0f, -9.5f);
    b2Body* groundBody = World.CreateBody(&bodyDef);
    b2EdgeShape groundEdge;
    groundEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(20.0f,0.0f));
    groundBody->CreateFixture(&groundEdge, 0.0f);

    //right
    bodyDef.position.Set(9.5f, 10.0f);
    b2Body* rightBody = World.CreateBody(&bodyDef);
    b2EdgeShape rightEdge;
    rightEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f,-20.0f));
    rightBody->CreateFixture(&rightEdge, 0.0f);

}
