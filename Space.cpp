//SDL (2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h> 

#include <iostream>
#include <fstream>
#include <string>

//Wii stuff
#include "Wiimote.h"

//Box 2D
#include <Box2D/Box2D.h>
#include "Ship.h"
#include "EventHandler.h"

//function prototype
void setupDisplay(SDL_Window *&pWindow, SDL_GLContext& context, int screenx=1920, int screeny=1080, int flags=0);

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        std::string argument = argv[1];
        if(argument == "-debug")
        {
            //set up some paramters or something 
        }
    }

    

    
    //variables to handle the SDL display
    SDL_Window *pWindow = 0;
    SDL_GLContext glContext;
    setupDisplay(pWindow,glContext);
    
//World for use with Box2D with no gravity
//positive 10.0 is up
//negative 10.0 is down
//positive is right,
//negative is left
    b2World World(b2Vec2(0.0f,0.0f));

    //Create ships, event handler, and controllers (wiimotes)
    Ships shipVector;

    EventHandler eventHandler(pWindow,World,shipVector);
    std::vector<Wiimote*> wiimotes;
    for(int i =0; i < argc; ++i)
    {
        shipVector.push_back(new Ship(World));
        wiimotes.push_back(new Wiimote(&eventHandler));
    }
    //TBD cleanup

    eventHandler.EventLoop();

    //clean up everything set up by the display
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(pWindow);
    pWindow = 0;
    SDL_Quit();
    std::cout << "Finished" << std::endl;
}




//****************************************************************
//Basic SDL2 setup of a screen for OpenGL
void setupDisplay(SDL_Window*& pWindow, SDL_GLContext& context, int screenx, int screeny, int flags)
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
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_EGL, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );

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
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
    }
        
    context = SDL_GL_CreateContext(pWindow);
    //just use the max size for the GL Viewport
    glViewport(0,0,1920,1080);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
