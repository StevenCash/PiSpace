#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <SDL2/SDL_thread.h>

class SDL_Thread;
class Ships;

class ConnectionThread
{

public:
    explicit ConnectionThread(Ships* pShips);
    void Activate();
private:
    static int ConnectionThreadMain(void *ptr);
    static Ships* m_pShips;
    SDL_Thread *m_pThread;
};
#endif
