#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <iostream>

#include "ConnectionThread.h"
#include "Ships.h"

Ships *ConnectionThread::m_pShips = 0;

int ConnectionThread::ConnectionThreadMain(void * /*ptr*/)
{
    for(int i= 0; i < 10; ++i)
    {
        std::cout << i << std::endl;
        SDL_Delay(1000);
    }
    return 0;
}

ConnectionThread::ConnectionThread(Ships* pShips):
    m_pThread(0)
{
    m_pShips=pShips;
}

void ConnectionThread::Activate()
{
    m_pThread = SDL_CreateThread(
        ConnectionThread::ConnectionThreadMain,
        "ConnectionThread",
        0);
    if(!m_pThread)
    {
        std::cerr << "Unable to create thread" << std::endl;
    }
}
