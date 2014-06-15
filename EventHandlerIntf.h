#ifndef EVENTHANDLERINTF_H
#define EVENTHANDLERINTF_H

//Interface to receive events from some kind of controller object.
class EventHandlerIntf
{
public:
  //Destructor
  virtual ~EventHandlerIntf() {};

  //Accessor functions to add events to the event handler
  //Currently, these map to Wiimote buttons
  //Should really be functionality based
  virtual void TurnCW(int controllerIndex) = 0;
  virtual void TurnCCW(int controllerIndex) = 0;
  virtual void Forward(int controllerIndex) = 0;
  virtual void Fire_1(int controllerIndex) = 0;
  virtual void Fire_2(int controllerIndex) = 0;
  virtual void Quit() = 0;
  
};



#endif
