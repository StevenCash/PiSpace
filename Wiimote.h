#ifndef WIIMOTE_H
#define WIIMOTE_H

#include <bluetooth/bluetooth.h>
#include <cwiid.h>
#include <map>
#include "ControllerIntf.h"


class EventHandlerIntf;

typedef enum
{
    NO_CONNECTION,
    CALLBACK_SETUP_ERROR
} WiiMoteErrorCode;

class Wiimote;

typedef std::map<cwiid_wiimote_t*, Wiimote*> WiimoteMap;


class Wiimote : public ControllerIntf
{
public:
    //constructor.  Starts the connection to a wiimote.
    //throw some exception if it fails
    explicit Wiimote(EventHandlerIntf *eventHandler);

    //disconnect the wiimote from this object
    ~Wiimote();

    void WiimoteCallback(
        int mesg_count,
        union cwiid_mesg mesg_array[], 
        struct timespec *timestamp);

private:
    EventHandlerIntf *m_eventHandler;
//setup a constant for the equivalent of BD_ADDR_ANY
//because the one in bluetooth.h is stupid
    static const bdaddr_t kBdAddrAny;


    cwiid_wiimote_t *m_pWiimote;
    int m_index;


    static int index;
    
    //place to register all the callback objects
    static WiimoteMap m_wiimoteMap;

    /* Wiimote Callback */
    static cwiid_mesg_callback_t cwiid_callback;

    bdaddr_t m_bdaddr;

};

#endif
