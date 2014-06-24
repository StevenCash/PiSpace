#ifndef WIIMOTEINTF_H
#define WIIMOTEINTF_H

//Interface to receive events from some kind of controller object.
class WiimoteIntf
{
public:
    //Destructor
    virtual ~WiimoteIntf() {};
    virtual void ButtonPushed(int controller, int buttons) = 0;
    //Accessor functions to add events to the event handler    
    virtual void Button_Home(int controllerIndex) = 0;
};



#endif
