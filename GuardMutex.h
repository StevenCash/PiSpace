#ifndef GUARDMUTEX_H
#define GUARDMUTEX_H

#include <SDL.h>

class GuardMutex
{
public:
    explicit GuardMutex(SDL_mutex *pMutex):
        m_pMutex(pMutex)
        {
            SDL_LockMutex(m_pMutex);
        }
    ~GuardMutex()
        {
            SDL_UnlockMutex(m_pMutex);
        }

private:
    SDL_mutex *m_pMutex;
};

#endif
