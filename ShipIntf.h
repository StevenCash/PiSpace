#ifndef SHIPINTF_H
#define SHIPINTF_H
#include <Box2D/Box2D.h>
#include <vector>

#define SHIP_FORWARD 0x00000001
#define SHIP_CCW     0x00000002
#define SHIP_CW      0x00000004
#define SHIP_SHOOT   0x00000008
#define SHIP_STOP    0x00000010
#define SHIP_RESET   0x00000020
#define SHIP_BOMB    0x00000040

class ShipIntf
{
public:
    virtual ~ShipIntf(){};
    //The function that actually puts the object on the screen
    virtual void DoCommands() = 0;

    virtual void Draw() = 0;

    //take a but field of controls and store whatever is needed for
    //future processing
    virtual void ProcessInput(int commands) = 0;

private:
};

typedef std::vector<ShipIntf*> Ships;

#endif //SHIP_H
