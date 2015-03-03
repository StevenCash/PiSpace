#include "Ship.h"
#include "Ships.h"
#include "GuardMutex.h"

#include <Box2D/Box2D.h>

/*****************************************/
Ships::Ships(b2World& world):
    m_world(world),
    m_index(0),
    m_pMutex(SDL_CreateMutex())
{
    //nothing
}

/*****************************************/
void Ships::AddShip()
{
    GuardMutex guard(m_pMutex);
    m_ships.push_back(new Ship(m_world));
}

/*****************************************/
ShipIntf* Ships::Next()
{
    GuardMutex guard(m_pMutex);
    ShipIntf *pShip = 0;
    if(m_index < m_ships.size())
    {
        pShip = m_ships[m_index];
        ++m_index;
    }
    else
    {
        m_index = 0;
    }
    return pShip;
}

/*****************************************/
unsigned int Ships::Length() const
{
    GuardMutex guard(m_pMutex);
    return m_ships.size();
}

/*****************************************/
ShipIntf* Ships::operator[](unsigned int index)
{
    GuardMutex guard(m_pMutex);
    if(index < m_ships.size())
    {
        return m_ships[index];
    }
    else
    {
        return 0;
    }
}

/*****************************************/
Ships::~Ships()
{
    SDL_DestroyMutex(m_pMutex);
}
