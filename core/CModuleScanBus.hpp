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

enum eState {esZero = 0, esSCount, esSMemory, esSMemory1, esSMemory2, esSMemory3, esSEeprom, esSPrint, esError};

class CModuleScanBus : public CModule
{
public:
	CModuleScanBus(unsigned char set_index);
    ~CModuleScanBus();

    void RunRx(CDatagram *datagram);

private:
    uint64_t time_start;
    unsigned int slave_number;
    unsigned int eeprom_address;

    void StateMachine(const uint64_t time_us);
    int ReadSlavesEeprom(const uint64_t time_us);
};

#endif
                      