//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CScheduler.hpp"

using namespace std;

//---------------------------------------------
CScheduler::CScheduler(CTimer *set_timer, CNetworkAdapter *set_adapter) : 
    timer(set_timer), 
    adapter(set_adapter), 
    module_index(0)
{}

//---------------------------------------------
CScheduler::~CScheduler()
{}

//---------------------------------------------
void CScheduler::Run()
{
    int count = 1000000;
    unsigned char recieve_buf[MAX_FRAME_SIZE];
    uint64_t current_cycle_time = timer->GetTimeUs();

    while(count --) 
    {        
        // Work
        for(vector<CModule*>::iterator p_module = modules.begin(); p_module != modules.end(); p_module ++) {
            (*p_module)->RunTx(this, current_cycle_time);
        }

        // Wait
        current_cycle_time = timer->WaitTick();

        // Send
        for(int i = 0; i < MAX_DATAGRAM_PRIORITY; i++) {
            unsigned int send_size = frames[i].Send(adapter);
            //if(send_size) cout << "send " << send_size << endl;
        }

        // Recieve
        int recieve_size;
        while((recieve_size = adapter->Receive(recieve_buf)) > 0) {
//            for(int j = 0; j < recieve_size; j ++) cout << hex << ((int)recieve_buf[j]) << " "; cout << endl;
            CFrame frame_rx(recieve_buf, recieve_size);
            SendDatagramBack(frame_rx);
        }
    }
}

//---------------------------------------------
void CScheduler::AddModule(CModule *new_module)
{
    modules.push_back(new_module);
}

//---------------------------------------------
void CScheduler::AddTxDatagram(CDatagram *new_datagram)
{
    const unsigned int priority = new_datagram->GetPriority();
    frames[priority] << new_datagram;
}

//---------------------------------------------
void CScheduler::SendDatagramBack(CFrame &frame)
{
    while(1) {
        CDatagram *datagram;
        frame >> &datagram;
        if(datagram) {
            CModule *module = GetModuleWithIndex(datagram->GetIndex());
            if(module)
                module->RunRx(datagram); // Send back to the owner
            delete datagram;
        }
        else
            break;
    }
}

//---------------------------------------------
CModule* CScheduler::GetModuleWithIndex(const unsigned char index)
{
    for(vector<CModule*>::iterator p_module = modules.begin(); p_module != modules.end(); p_module ++) {
        if((*p_module)->GetIndex() == index)
            return *p_module;
    }
    return NULL;
}

//---------------------------------------------
