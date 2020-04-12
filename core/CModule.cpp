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
CModule::CModule(const unsigned char set_index, const unsigned int set_max_number_datagram) : 
    index(set_index),
    state(0),
    wait_type(ewtNone),
    max_number_datagram(set_max_number_datagram)
{
    for(int i = 0; i < set_max_number_datagram; i ++) {
        working_datagrams << new CDatagram(index);
    }
}

//---------------------------------------------
CModule::~CModule()
{
    while(working_datagrams.GetSize()) {
        CDatagram *dg;
        working_datagrams >> dg;
        delete dg;
    }
}

//---------------------------------------------
void CModule::ChangeState(const unsigned char set_state)
{
    state_count = 0;
    state_subcount = 0;
    state = set_state;
}

//---------------------------------------------
void CModule::ChangeState(const int ret_value, const unsigned char set_state_plus, const unsigned char set_state_minus)
{
    if(ret_value > 0)
        ChangeState(set_state_plus);
    else if(ret_value < 0)
        ChangeState(set_state_minus);
}

//---------------------------------------------
void CModule::InsertTxDatagram(CDatagram *set_datagram)
{
    tx_datagrams << set_datagram;
}

//---------------------------------------------
/*void CModule::RemoveDatagram(CDatagram *datagram_to_remove)
{
    for(int i = 0; i < tx_datagrams.GetSize(); i ++) {
        if(tx_datagrams[i] == datagram_to_remove) {
            tx_datagrams.Erase(i);
            break;
        }
    }
}
*/
//---------------------------------------------
void CModule::RunTx(CScheduler *scheduler, uint64_t time_us)
{
    StateMachine(time_us);

    for(int i = 0; i < tx_datagrams.GetSize(); i ++)
        scheduler->AddTxDatagram(tx_datagrams[i]);

    tx_datagrams.Clear();
}

//---------------------------------------------
void CModule::RunRx(CDatagram *datagram)
{
    if(wait_type == ewtMemoryRead) {
        slaves[active_slave_number].UploadMemoryStep2(datagram);
    }
    SetTimeout(0);
    tx_rx_datagram_count --;

    delete datagram;
}

//---------------------------------------------
int CModule::WaitRXDatagram(const uint64_t time_us)
{
    if(timeout == 0) // datagram is arrived
        return 1; // just positive
    return ((time_us - timeout) >= DEFAULT_TIMEOUT_US)? -1 : 0;
}

//---------------------------------------------
int CModule::MultiWaitRXDatagram(const uint64_t time_us)
{
    if(tx_rx_datagram_count == 0) // all datagrams are arrived
        return 1; // just positive
    return ((time_us - timeout) >= DEFAULT_TIMEOUT_US)? -1 : 0;
}

//---------------------------------------------
int CModule::ReadAllSlavesMemoryInTurn(const uint64_t time_us, const unsigned int address, const unsigned int size)
{
    int ret = 0;
    if(state_subcount == 0) {
        active_slave_number = 0;
        state_subcount = 1;
    }
    if(state_subcount == 1) {
        ReadSlaveMemorySend(working_datagrams[1], time_us, address, size);
        state_subcount = 2;
    }
    else {
        ret = WaitRXDatagram(time_us);
        if(ret > 0) {
            if(++active_slave_number < slaves.GetSize())
                ret = 0;
            state_subcount = 1;
        }
    }

    return ret;
}

//---------------------------------------------
void CModule::ReadSlaveMemorySend(CDatagram *dg, const uint64_t time_us, const unsigned int address, const unsigned int size)
{
    slaves[active_slave_number].UploadMemoryStep1(dg, (address << 16) | ((0-active_slave_number) & 0xFFFF), size);
    wait_type = ewtMemoryRead;
    SetTimeout(time_us);
    InsertTxDatagram(dg);
}

//---------------------------------------------
int CModule::WriteAllSlavesMemoryInTurn(const uint64_t time_us, const unsigned int address, const unsigned int size, const uint8_t *data)
{
    int ret = 0;
    if(state_subcount == 0) {
        active_slave_number = 0;
        state_subcount = 1;
    }
    if(state_subcount == 1) {
        if(data)
            slaves[active_slave_number].SetMemory(address, data, size);
        WriteSlaveMemory(working_datagrams[1], time_us, address, size);
        state_subcount = 2;
    }
    else {
        ret = WaitRXDatagram(time_us);
        if(ret > 0) {
            if(++active_slave_number < slaves.GetSize())
                ret = 0;
            state_subcount = 1;
        }
    }

    return ret;
}

//---------------------------------------------
int CModule::WriteAllSlavesMemoryAtOnce(const uint64_t time_us, const unsigned int address, const unsigned int size, const uint8_t *data)
{
    int ret = 0;
    if(state_subcount == 0) {
        for(active_slave_number = 0; active_slave_number < slaves.GetSize(); active_slave_number++) {
            if(data)
                slaves[active_slave_number].SetMemory(address, data, size);
            WriteSlaveMemory(working_datagrams[1], time_us, address, size);
        }
        tx_rx_datagram_count = slaves.GetSize();
        state_subcount = 1;
    }
    else {
        ret = MultiWaitRXDatagram(time_us);
    }

    return ret;
}

//---------------------------------------------
void CModule::WriteSlaveMemory(CDatagram *dg, const uint64_t time_us, const unsigned int address, const unsigned int size)
{
    uint8_t *data = slaves[active_slave_number].GetMemory(address);
    slaves[active_slave_number].DownloadMemoryStep(dg, (address << 16) | ((0-active_slave_number) & 0xFFFF), data, size);
    wait_type = ewtMemoryWrite;
    SetTimeout(time_us);
    InsertTxDatagram(dg);
}

//---------------------------------------------
//---------------------------------------------
