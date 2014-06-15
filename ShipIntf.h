#ifndef SHIPINTF_H
#define SHIPINTF_H

#include <vector>

class ShipIntf
{
public:
    virtual ~ShipIntf(){};
    //The function that actually puts the object on the screen
    virtual void Draw() = 0;
    virtual void RotateCW() = 0;
    virtual void RotateCCW() = 0;
    virtual void Forward() = 0;
private:
};

typedef std::vector<ShipIntf*> Ships;

#endif //SHIP_H
