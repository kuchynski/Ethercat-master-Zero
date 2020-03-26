//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cscheduler_H
#define cscheduler_H

#include "CFrame.hpp"
#include "CNetworkAdapter.hpp"
#include "CTimer.hpp"
#include "CModule.hpp"

class CModule;

class CScheduler
{
public:
	CScheduler(CTimer *set_timer, CNetworkAdapter *set_adapter);
    ~CScheduler();

    void AddModule(CModule *new_module);
    void AddTxDatagram(CDatagram *new_datagram);
    unsigned char GetNewModuleIndex() { return module_index++; }
    void Run();

private:
    CTimer *timer;
    CNetworkAdapter *adapter;
    CFrame frames[MAX_DATAGRAM_PRIORITY];
    CStack <CModule*> modules;
    unsigned char module_index;

    void SendDatagramBack(CFrame &frame);
    CModule* GetModuleWithIndex(unsigned char index);
};

#endif
                      
