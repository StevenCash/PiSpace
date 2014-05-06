//SDL (2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h> 

#include <iostream>
#include <fstream>
#include <string>

//Wii stuff
#include <bluetooth/bluetooth.h>
#include <cwiid.h>

//Box 2D
#include <Box2D/Box2D.h>
#include "Ship.h"

#include "Ship.h"


//function prototype
void setupDisplay(int screenx=1920, int screeny=1080, int flags=0);
void setupWiiMote();

/* Wiimote Callback */
cwiid_mesg_callback_t cwiid_callback;

//global variables to handle the SDL display
SDL_Window *pWindow = 0;
SDL_GLContext glContext;
bdaddr_t bdaddr;

//new event for SDL
Uint32 wiiButtonEvent;
Uint32 wiiAccelEvent;

//setup a constant for the equivalent of BD_ADDR_ANY
//because the one in bluetooth.h is stupid
const bdaddr_t kBdAddrAny = {{0, 0, 0, 0, 0, 0}};

cwiid_wiimote_t *wiimote = NULL;


//World for use with Box2D with no gravity
b2World World(b2Vec2(0.0f,0.0f));


int main(int argc, char *argv[])
{
    //bluetooth lookup stuff
//    bool reset_bdaddr = false;
//    if(bacmp(&bdaddr, &kBdAddrAny) == 0 )
//    {
//        reset_bdaddr = true;
//    }



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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //initial display
    Ship ship(World);
    ship.Draw();
    SDL_GL_SwapWindow(pWindow);

    setupWiiMote();


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
                    ship.rotateRight();
                    break;
                }
                case SDLK_a:
                {
                    ship.rotateLeft();
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
                        ship.rotateLeft();
                    }
                    if(event.user.code & CWIID_BTN_DOWN)
                    {
                        ship.rotateRight();
                    }
                    if(event.user.code & CWIID_BTN_RIGHT)
                    {
                        ship.translateUp();
                    }
                    if(event.user.code & CWIID_BTN_LEFT)
                    {
                        ship.translateDown();
                    }
                    if(event.user.code & CWIID_BTN_HOME)
                    {
                        bQuit = true;
                    }
                }
                else if(event.user.type == wiiAccelEvent)
                {
                    if(event.user.code > 140)
                    {
                        ship.rotateRight();
                    }
                    else if (event.user.code < 120)
                    {
                        ship.rotateLeft();
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
        ship.Draw();
        SDL_GL_SwapWindow(pWindow);
        SDL_Delay(100);
        
    }
    
    
    if((SDL_GetTicks() - timestamp) > 30000)
    {
        bQuit = true;
    }




    //clean up everything set up by the display
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(pWindow);
    pWindow = 0;
    SDL_Quit();
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
void setupWiiMote()
{
    std::cout << "Put wiimote into discovery mode (press 1+2)" << std::endl;
    wiimote = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC);
    if(wiimote == NULL)
    {
        std::cerr << "No connection. Quitting" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Connected" << std::endl;
    }




/*
    int messageCount = 0;
    cwiid_mesg *mesg[100];
    timespec timeinfo;

    if(cwiid_get_mesg(wiimote, &messageCount, mesg, &timeinfo))
    {
        std::cout << "Error reading message" << std::endl;
    }
    else
    {
        std::cout << "Got a message!" << std::endl;
    }

    if(cwiid_get_mesg(wiimote, &messageCount, mesg, &timeinfo))
    {
        std::cout << "Error reading message" << std::endl;
    }
    else
    {
        std::cout << "Got a message!" << std::endl;
    }


    std::cout << "Turning on LED 1" << std::endl;
    cwiid_command(wiimote, CWIID_CMD_LED, CWIID_LED1_ON);
    SDL_Delay(1000);

    std::cout << "Turning off LED 1" << std::endl;
    cwiid_command(wiimote, CWIID_CMD_LED, 0);
    SDL_Delay(5000);

    
    
    cwiid_close(wiimote);
    std::cout << "exiting" << std::endl;
    exit(0);
*/
    if (cwiid_set_mesg_callback(wiimote, &cwiid_callback))
    {
        std::cerr << "Error setting callback.  Exitting" << std::endl;
        cwiid_close(wiimote);
        exit(2);
    }
    std::cout << "Commanding reporting BTN" << std::endl;
    cwiid_command(wiimote, CWIID_CMD_RPT_MODE, CWIID_RPT_BTN);


}
