//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cmodulescanbus_H
#define cmodulescanbus_H

#include "CModule.hpp"

#define SCAN_BUS_MAX_NUMBER_DATAGRAM    4
#define SCAN_BUS_PERIOD_US              1000000

enum eState {esZero, esStart, esSend, esWait};
enum eWaitType {ewtNone, ewtSlaveCount};

class CModuleScanBus : public CModule
{
public:
	CModuleScanBus(CScheduler *set_scheduler);
    ~CModuleScanBus();

    void RunRx(CDatagram *datagram);

private:
    unsigned char state;
    uint64_t time_start;
    uint64_t timeout;
    eWaitType wait_type;
    unsigned int slave_count;
    CDatagram *datagram[SCAN_BUS_MAX_NUMBER_DATAGRAM];

    void StateMachine(uint64_t time_us);
};

#endif
                      