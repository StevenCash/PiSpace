#include "Wiimote.h"
#include <iostream>
#include <SDL2/SDL.h>

#include "WiimoteIntf.h"
#include "ShipIntf.h"

int Wiimote::index = 0;
WiimoteMap Wiimote::m_wiimoteMap;

//setup a constant for the equivalent of BD_ADDR_ANY
//because the one in bluetooth.h is stupid
const bdaddr_t Wiimote::kBdAddrAny = {{0, 0, 0, 0, 0, 0}};


Wiimote::Wiimote(WiimoteIntf *wiimoteHandler)
    :m_wiimoteHandler(wiimoteHandler),
     m_pWiimote(NULL),
     m_index(index++),
     m_bdaddr(kBdAddrAny)
{
    std::cout << "Put wiimote into discovery mode (press 1+2)" << std::endl;
    m_pWiimote = cwiid_open(&m_bdaddr, CWIID_FLAG_MESG_IFC);
    if(m_pWiimote == NULL)
    {
        std::cerr << "No connection. Quitting" << std::endl;
        throw NO_CONNECTION;
    }
    else
    {
        std::cout << "Connected" << std::endl;
        cwiid_command(m_pWiimote, CWIID_CMD_LED, CWIID_LED1_ON);
    }


    if (cwiid_set_mesg_callback(m_pWiimote, cwiid_callback))
    {
        std::cerr << "Error setting callback.  Exitting" << std::endl;
        cwiid_close(m_pWiimote);
        m_pWiimote = NULL;
        throw CALLBACK_SETUP_ERROR;
    }
    else
    {
        m_wiimoteMap[m_pWiimote] = this;
    }
    std::cout << "WiiMote " << m_index << " connected" << std::endl;
    cwiid_command(m_pWiimote, CWIID_CMD_RPT_MODE, CWIID_RPT_BTN);
}


void Wiimote::WiimoteCallback(
    int mesg_count,
    union cwiid_mesg mesg_array[], 
    struct timespec */*timestamp*/)
{
    for (int i=0; i < mesg_count; ++i) 
    {
        switch (mesg_array[i].type) 
        {
	case CWIID_MESG_BTN:
        {
            Uint32 buttons = mesg_array[i].btn_mesg.buttons;
            if(buttons & CWIID_BTN_HOME)
            {
                m_wiimoteHandler->Button_Home(m_index);
            }
            else
            {
                unsigned int command = 0;
                if(buttons & CWIID_BTN_UP)
                {
                    command |= SHIP_CCW;
                }
                if(buttons & CWIID_BTN_DOWN)
                {
                    command |= SHIP_CW;
                }
                if(buttons & CWIID_BTN_2)
                {
                    command |= SHIP_FORWARD;
                }
                if(buttons & CWIID_BTN_A)
                {
                    command |= SHIP_SHOOT;
                }
                m_wiimoteHandler->ButtonPushed(m_index,command);
            }
            break;
        }
        default:
            break;
        }
    }
}

/*static*/
void Wiimote::cwiid_callback
(
    cwiid_wiimote_t *wiimote, 
    int mesg_count,
    union cwiid_mesg mesg_array[], 
    struct timespec *timestamp)
{
    //This probably should be mutex protected

    //search the static list for the wiimote to call back on
    WiimoteMap::iterator iter = m_wiimoteMap.find(wiimote);
    if(iter != m_wiimoteMap.end())
    {
        iter->second->WiimoteCallback(mesg_count,
                                     mesg_array,
                                     timestamp);

    }
}

Wiimote::~Wiimote()
{
    m_wiimoteMap.erase(m_pWiimote);
    cwiid_close(m_pWiimote);
}
