#ifndef SHIPINTF_H
#define SHIPINTF_H

#include <vector>

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
