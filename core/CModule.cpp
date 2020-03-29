//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CModule.hpp"
#include "CScheduler.hpp"

using namespace std;

//---------------------------------------------
CModule::CModule(CScheduler *set_scheduler) : scheduler(set_scheduler)
{
    index = scheduler->GetNewModuleIndex();
}

//---------------------------------------------
CModule::~CModule()
{
/*    while(datagrams.GetSize()) {
        CDatagram *datagram;
        datagrams >> datagram;
        delete datagram;
    }*/
}

//---------------------------------------------
void CModule::InsertDatagram(CDatagram *set_datagram)
{
    datagrams << set_datagram;
}

//---------------------------------------------
void CModule::RemoveDatagram(CDatagram *datagram_to_remove)
{
    for(int i = 0; i < datagrams.GetSize(); i ++) {
        if(datagrams[i] == datagram_to_remove) {
            datagrams.Erase(i);
            break;
        }
    }
}

//---------------------------------------------
void CModule::RunTx(uint64_t time_us)
{
    StateMachine(time_us);

    for(int i = 0; i < datagrams.GetSize(); i ++)
        scheduler->AddTxDatagram(datagrams[i]);

    datagrams.Clear();
}

//---------------------------------------------
void CModule::RunRx(CDatagram *datagram)
{
//    unsigned char datagram_index = datagram->GetIndex();
    //unsigned int size_data = datagram->GetSize();
//    cout << "module " << (int)index << " rx: " << (int)datagram_index << endl;

    delete datagram;
}

//---------------------------------------------
//---------------------------------------------
