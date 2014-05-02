
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h> 

#include <iostream>
#include <fstream>
#include <string>
#include "Ship.h"

#include <bluetooth/bluetooth.h>
#include <cwiid.h>

//function prototype
void setupDisplay(int screenx=1920, int screeny=1080, int flags=0);

/* Wiimote Callback */
cwiid_mesg_callback_t cwiid_callback;

//global variables to handle the SDL display
SDL_Window *pWindow = 0;
SDL_GLContext glContext;
bdaddr_t bdaddr;

//new event for SDL
Uint32 wiiButtonEvent;

//setup a constant for the equivalent of BD_ADDR_ANY
//because the one in bluetooth.h is stupid
const bdaddr_t kBdAddrAny = {{0, 0, 0, 0, 0, 0}};

cwiid_wiimote_t *wiimote = NULL;


int main(int argc, char *argv[])
{
    //bluetooth lookup stuff
//    bool reset_bdaddr = false;
//    if(bacmp(&bdaddr, &kBdAddrAny) == 0 )
//    {
//        reset_bdaddr = true;
//    }


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
    

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //initial display
    Ship ship;
    ship.Draw();
    SDL_GL_SwapWindow(pWindow);

    if (cwiid_set_mesg_callback(wiimote, &cwiid_callback))
    {
        std::cerr << "Error setting callback.  Exitting" << std::endl;
        cwiid_close(wiimote);
        exit(2);
    }
    std::cout << "Commanding reporting BTN" << std::endl;
    cwiid_command(wiimote, CWIID_CMD_RPT_MODE, CWIID_RPT_BTN);
    

    bool bQuit = false;
    SDL_Event event;

    //Set it up so it automatically quits after 10 seconds
    int timestamp = SDL_GetTicks();    
    while(!bQuit)
    {
        if(SDL_PollEvent(&event))
        {          
            if(event.type == wiiButtonEvent)
            {
                std::cout << "Button pushed." << std::endl;
            }
            else
            {
                switch(event.type)
                {
                case SDL_QUIT:
                    bQuit=true;
                    break;
                default:
                    break;
                }
            }
        }

        SDL_Delay(100);
        // SDL_Event dummy;
        // dummy.type = SDL_KEYDOWN;
        // dummy.key.keysym.sym = SDLK_LEFT;
        // SDL_PushEvent(&dummy);

        if((SDL_GetTicks() - timestamp) > 30000)
        {
            bQuit = true;
        }
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
    std::cout << "Message received" << std::endl;
    for (int i=0; i < mesg_count; ++i) 
    {
        switch (mesg_array[i].type) 
        {
	case CWIID_MESG_BTN:
            //create a new SDL event with the button state
//            cwiid_btn(&mesg_array[i].btn_mesg);
            std::cout << "Button pressed" << std::endl;
            SDL_Event event;
            SDL_zero(event);
            event.type = wiiButtonEvent;
            SDL_PushEvent(&event);
            break;
        default:
            //do nothing
            break;
        }
    }
}
