//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>

#include "CModuleScanBus.hpp"

using namespace std;

//---------------------------------------------
CModuleScanBus::CModuleScanBus(CScheduler *set_scheduler) : 
    CModule(set_scheduler),
    state(esZero),
    time_start(0),
    wait_type(ewtNone),
    slave_count(0)
{
    for(int i = 0; i < SCAN_BUS_MAX_NUMBER_DATAGRAM; i ++)
        datagram[i] = NULL;

    struct SDatagramParam datagram_param;
    datagram_param.index = index;
    datagram_param.command = 7;
    datagram_param.size_data = 1;
    datagram_param.priority = 2;
    datagram_param.address = 0;
    
    datagram[0] = new CDatagram(datagram_param);
}

//---------------------------------------------
CModuleScanBus::~CModuleScanBus()
{
    for(int i = 0; i < SCAN_BUS_MAX_NUMBER_DATAGRAM; i ++) {
        if(datagram[i])
            delete datagram[i];
    }
}

//---------------------------------------------
void CModuleScanBus::RunRx(CDatagram *datagram)
{
    // Parse a datagram here

    if(wait_type == ewtSlaveCount) {
        unsigned int address = datagram->GetAddress();
        slave_count = address;
    }

    timeout = 0;
    CModule::RunRx(datagram);
}

//---------------------------------------------
void CModuleScanBus::StateMachine(uint64_t time_us)
{
    switch(state) 
    {
    case esZero:
        //cout << "esZero" << endl;
        if(time_start <= time_us) {
           // cout << "start: " << time_start << " " << time_us << endl;
            time_start = time_us + SCAN_BUS_PERIOD_US;
            state = esStart;
        }
        break;
    case esStart:
       // cout << "esStart" << endl;
        state = esSend;
        break;
    case esSend:
        //cout << "esSend" << endl;
        InsertDatagram(datagram[0]);
        wait_type = ewtSlaveCount;
        timeout = time_us;
        state = esWait;
        break;
    case esWait:
        //cout << "esWait" << endl;
        if(timeout == 0) {
            state = esZero;
        }
        else if((time_us - timeout) >= DEFAULT_TIMEOUT_US) {
            slave_count = 0;
            state = esZero;
        }
        if(state != esWait)
            cout << "Slave count: " << slave_count << endl;
        break;
    default:
        break;
    }

}

//---------------------------------------------
