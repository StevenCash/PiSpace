#ifndef CONTROLLERINTF_H
#define CONTROLLERINTF_H

//Interface to receive events from some kind of controller object.
class ControllerIntf
{
public:
    //Destructor
    virtual ~ControllerIntf() {};
    virtual void ButtonPushed(int controller, int buttons) = 0;
    //Accessor functions to add events to the event handler    
    virtual void ButtonHome(int controllerIndex) = 0;
};



#endif
