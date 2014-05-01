
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h> 

#include <iostream>
#include <fstream>
#include <string>
#include "Ship.h"

//function prototype
void setupDisplay(int screenx=1920, int screeny=1080, int flags=0);

//global variables to handle the SDL display
SDL_Window *pWindow = 0;
SDL_GLContext glContext;

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
            default:
                break;
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



