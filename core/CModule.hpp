//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cmodule_H
#define cmodule_H

#include "CScheduler.hpp"

class CScheduler;

#define DEFAULT_TIMEOUT_US      10000

class CModule
{
public:
    CModule(CScheduler *set_scheduler);
    ~CModule();

    virtual void RunTx(uint64_t time_us);
    virtual void RunRx(CDatagram *datagram);
    unsigned char GetIndex() const { return index; }

protected:
    unsigned char index;
    void InsertDatagram(CDatagram *set_datagram);
    void RemoveDatagram(CDatagram *datagram_to_remove);
private:
    CScheduler *scheduler;
    CStack <CDatagram*> datagrams;

    virtual void StateMachine(uint64_t time_us) = 0;
};

#endif
                      
