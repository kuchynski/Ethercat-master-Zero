//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cmodule_H
#define cmodule_H

#include <list>
#include <vector>
#include "CScheduler.hpp"
#include "CSlave.hpp"

class CScheduler;

#define DEFAULT_TIMEOUT_US      10000

enum eWaitType {ewtNone, ewtMemoryRead, ewtMemoryWrite, ewtCustom0, ewtCustom1};

class CModule
{
public:
    CModule(const unsigned char set_index, const unsigned int set_max_number_datagram);
    ~CModule();

    virtual void RunTx(CScheduler *scheduler, uint64_t time_us);
    virtual void RunRx(CDatagram *datagram);
    unsigned char GetIndex() const { return index; }

protected:
    unsigned char index;
    vector<CDatagram*> permanent_datagrams;
    unsigned int slave_number;
    eWaitType wait_type;
    vector<CSlave> slaves;
    unsigned char state;
    unsigned int state_count;
    unsigned int state_subcount;

    void ChangeState(const unsigned char set_state);
    void ChangeState(const int ret_value, const unsigned char set_state_minus, const unsigned char set_state_plus);
    void InsertTxDatagram(CDatagram *set_datagram);
    //void RemoveDatagram(CDatagram *datagram_to_remove);
    int WaitRXDatagram(const uint64_t time_us);
    void SetTimeout(const uint64_t set_timeout) { timeout = set_timeout; }

    int ReadAllSlavesMemoryInTurn(const uint64_t time_us, const unsigned int address, const unsigned int size);    
    int ReadAllSlavesMemoryAtOnce(const uint64_t time_us, const unsigned int address, const unsigned int size);    
    void ReadSlaveMemorySend(CDatagram *dg, const uint64_t time_us, const unsigned int address, const unsigned int size);

    int WriteAllSlavesMemoryInTurn(const uint64_t time_us, const unsigned int address, const unsigned int size, const uint8_t *data = NULL);
    int WriteAllSlavesMemoryAtOnce(const uint64_t time_us, const unsigned int address, const unsigned int size, const uint8_t *data = NULL);
    void WriteSlaveMemory(CDatagram *dg, const uint64_t time_us, const unsigned int address, const unsigned int size);

private:
    list<CDatagram*> tx_datagrams;
    uint64_t timeout;
    unsigned int max_number_datagram;
    unsigned int active_slave_number;
    unsigned int tx_rx_datagram_count;

    virtual void StateMachine(uint64_t time_us) = 0;
    int MultiWaitRXDatagram(const uint64_t time_us);
};

#endif
                      
