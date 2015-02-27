#ifndef CONTACTLISTENER
#define CONTACTLISTENER

#include <Box2D/Box2D.h>
class EventHandler;

class ContactListener : public b2ContactListener
{
private:
    EventHandler& m_eventHandler;

public:
    explicit ContactListener(EventHandler& eventHandler);
    void BeginContact(b2Contact* contact);

};

#endif
