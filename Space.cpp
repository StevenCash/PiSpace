//SDL (2)
#include "GLHeader.h"
#include <SDL2/SDL.h>

#include <iostream>
#include <fstream>
#include <string>

#include "TraceLogger.h"

//Box 2D
#include <Box2D/Box2D.h>

//Wii stuff remote
#ifdef RPI
#include "Wiimote.h"
#endif

#include "Ship.h"
#include "EventHandler.h"
#include "Walls.h"


//function prototype
void setupDisplay(SDL_Window *&pWindow, SDL_GLContext& context, int screenx=640, int screeny=480);



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

    setupDisplay(pWindow,glContext,640,480);

//World for use with Box2D with no gravity
//positive 10.0 is up
//negative 10.0 is down
//positive is right,
//negative is left
    b2World World(b2Vec2(0.0f,0.0f));

    //Create ships, event handler, and controllers (wiimotes)
    Ships shipVector;

    //Put some boundries on the world
    Walls myWalls(World);

#ifdef RPI
    std::vector<Wiimote*> wiimotes;
#endif

    //Instantiate main event handler/control loop object
    EventHandler eventHandler(pWindow,World,shipVector, myWalls);

    for(int i =0; i < argc; ++i)
    {
        shipVector.push_back(new Ship(World));
#ifdef RPI
        wiimotes.push_back(new Wiimote(&eventHandler));
#endif
    }


    //Run the main loop
    eventHandler.EventLoop();

    //clean up everything set up by the display
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(pWindow);
    pWindow = 0;
    SDL_Quit();

    return 0;
}




//****************************************************************
//Basic SDL2 setup of a screen for OpenGL
void setupDisplay(SDL_Window*& pWindow, SDL_GLContext& context, int screenx, int screeny)
{
    if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 ) {
        std::cerr << "Unable to initialize SDL " << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return ;
    }
#ifdef WINDOWS
    uint32 FLAGS = SDL_WINDOW_OPENGL;
#else
    uint32 FLAGS = SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN;
#endif
    
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_EGL , 1 ) ;
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION , 2 );
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION , 0 ) ;
    SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER , 1 ) ;
 //   SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE , 24 ) ;
    if ( ( pWindow = SDL_CreateWindow ( "PiSpace " ,
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            screenx , screeny,
            FLAGS ) ) == NULL ) {
        std::cerr << "Unable to open window" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return ;
    }
    SDL_DisableScreenSaver();
    
    context = SDL_GL_CreateContext ( pWindow ) ;
    SDL_GL_SetSwapInterval ( 1 ) ;

#ifdef WINDOWS
    glewExperimental = true;
    GLenum err = glewInit () ;
    if(err != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW" << std::endl;
        return;
    }
#endif
    glViewport(0,0,screenx,screeny);

//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

