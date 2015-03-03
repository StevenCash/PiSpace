#ifndef SHIPS_H
#define SHIPS_H
#include "ShipIntf.h"

#include <vector>
class ShipIntf;
class SDL_mutex;

class b2World;

class Ships
{
public:
    explicit Ships(b2World& world);
    ~Ships();
    void AddShip();
    ShipIntf* Next();
    unsigned int Length() const;
    ShipIntf* operator[](unsigned int index);
    
private:
    std::vector<ShipIntf*> m_ships;
    b2World& m_world;
    unsigned int m_index;
    SDL_mutex *m_pMutex;
};
#endif
