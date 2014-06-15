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
#include "DebugDraw.h"

//function prototype
void setupDisplay(SDL_Window *&pWindow, SDL_GLContext& context, int screenx=1920, int screeny=1080, int flags=0);
void setupWalls(b2World& world);


int main(int argc, char *argv[])
{
    std::cout << "Num arguments: " << argc << std::endl;
    if(argc > 1)
    {
        std::cout << argv[1] << std::endl;
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
    DebugDraw debugDraw;
    debugDraw.SetFlags(0xFFFF);
    //initial display
    World.SetDebugDraw(&debugDraw);

    //Create ships, event handler, and controllers (wiimotes)
    Ships shipVector;
    Ship ship(World);
    shipVector.push_back(&ship);
//    Ship ship2(World);
//    shipVector.push_back(&ship2);
    EventHandler eventHandler(pWindow,World,shipVector);
    Wiimote player1Control(&eventHandler);
//    Wiimote player2Control(&eventHandler);

    //Put some boundries on the world
    setupWalls(World);

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
        
    context = SDL_GL_CreateContext(pWindow);
    //just use the max size for the GL Viewport
    glViewport(0,0,1920,1080);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



void setupWalls(b2World& world)
{
    b2BodyDef bodyDef;

    //Top
    bodyDef.position.Set(-10.0f, 9.5f);
    b2Body* topBody = world.CreateBody(&bodyDef);
    b2EdgeShape topEdge;
    topEdge.Set(b2Vec2(0.0, 0.0), b2Vec2(20.0,0.0));
    topBody->CreateFixture(&topEdge, 0.0f);
    
      
    //left
    bodyDef.position.Set(-9.5f, 10.0f);
    b2Body* leftBody = world.CreateBody(&bodyDef);
    b2EdgeShape leftEdge;
    leftEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f,-20.0f));
    leftBody->CreateFixture(&leftEdge, 0.0f);

    //bottom
    bodyDef.position.Set(-10.0f, -9.5f);
    b2Body* groundBody = world.CreateBody(&bodyDef);
    b2EdgeShape groundEdge;
    groundEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(20.0f,0.0f));
    groundBody->CreateFixture(&groundEdge, 0.0f);

    //right
    bodyDef.position.Set(9.5f, 10.0f);
    b2Body* rightBody = world.CreateBody(&bodyDef);
    b2EdgeShape rightEdge;
    rightEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f,-20.0f));
    rightBody->CreateFixture(&rightEdge, 0.0f);

}
