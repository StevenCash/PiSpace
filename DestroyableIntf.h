#ifndef DESTROYABLEINTF_H
#define DESTROYABLEINTF_H

class DestroyableIntf
{
public:
    virtual void DestroyObject() = 0;
    virtual ~DestroyableIntf() {};
};

struct DestroyableIntfContainer
{
    DestroyableIntf* pDestroyable;
};

#endif
