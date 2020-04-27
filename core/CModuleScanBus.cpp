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
CModuleScanBus::CModuleScanBus(unsigned char set_index) : 
    CModule(set_index, SCAN_BUS_MAX_NUMBER_DATAGRAM),
    time_start(0)
{
    struct SDatagramParam param;
    param.index = index;
    param.command = 7;
    param.size_data = 1;
    param.priority = 2;
    param.address = 0;

    permanent_datagrams[0]->SetParam(param);

    state = esZero;
}

//---------------------------------------------
CModuleScanBus::~CModuleScanBus()
{
}

//---------------------------------------------
void CModuleScanBus::RunRx(CDatagram *datagram)
{
    // Parse a datagram here

    if(wait_type == ewtCustom0) {
        const unsigned int address = datagram->GetAddress();
        slave_number = address;
    }

    CModule::RunRx(datagram);
}

//---------------------------------------------
void CModuleScanBus::StateMachine(const uint64_t time_us)
{
    int ret;
    uint8_t data8;

    switch(state) 
    {
    case esZero:
        if(time_start <= time_us) {
            time_start = time_us + SCAN_BUS_PERIOD_US;
            ChangeState(esSCount);
        }
        break;

    case esSCount:
        if(state_count == 0) {
            state_count = 1;
            wait_type = ewtCustom0;
            SetTimeout(time_us);
            InsertTxDatagram(permanent_datagrams[0]);
        }
        else {
            ret = WaitRXDatagram(time_us);
            if(ret > 0) {
                slaves.clear();
                for(int i = 0; i < slave_number; i ++)
                    slaves.push_back(CSlave());
            }
            ChangeState(ret, slaves.size()? esSMemory : esError, esError);
        }
        break;

    case esSMemory:
        ret = ReadAllSlavesMemoryInTurn(time_us, state_count, 0x200);
        if(ret < 0)
            ChangeState(esError);
        else if(ret > 0) {
            state_subcount = 0;
            state_count += 0x200;
            if(state_count >= SLAVE_MEMORY_SIZE) {
                ChangeState(esSMemory1);
            }
        }
        break;

    case esSMemory1:
        data8 = 2;
        ret = WriteAllSlavesMemoryAtOnce(time_us, 0x120, 1, &data8);
        ChangeState(ret, esSMemory2, esError);
        break;

    case esSMemory2:
        data8 = 4;
        ret = WriteAllSlavesMemoryAtOnce(time_us, 0x120, 1, &data8);
        ChangeState(ret, esSMemory3, esError);
        break;

    case esSMemory3:
        data8 = 8;
        ret = WriteAllSlavesMemoryAtOnce(time_us, 0x120, 1, &data8);
        ChangeState(ret, esSEeprom, esError);
        eeprom_address = 0;
        break;

    case esSEeprom:
        ret = ReadSlavesEeprom(time_us);
        ChangeState(ret, esSPrint, esError);
        break;

    case esSPrint:
        cout << "Slave count: " << slaves.size() << endl;
        for(int i = 0; i < slaves.size(); i ++)
            cout << "Slave" << i << ": " << slaves[i].GetQWord(0) << endl;
        ChangeState(esZero);
        break;

    case esError:
        slaves.clear();
        ChangeState(esSPrint);
        break;

    default:
        ChangeState(esZero);
        break;
    }
}

unsigned int count_eeprom = 0;
//---------------------------------------------
int CModuleScanBus::ReadSlavesEeprom(const uint64_t time_us)
{
    int ret = 0;
    if(state_count == 0) { // write address
        uint8_t data[5];
        data[0] = 0x01; // read
        memcpy(data + 1, &eeprom_address, 4);
        ret = WriteAllSlavesMemoryAtOnce(time_us, 0x503, 5, data);
        if(ret > 0) {
            state_count = 1;
            state_subcount = 0;
            ret = 0;
        }
    }
    else if(state_count == 1) { // read dword
        ret = ReadAllSlavesMemoryAtOnce(time_us, 0x508, 4);
        if(ret > 0) {
            //copy here
            ++eeprom_address;
            if(eeprom_address < SLAVE_EEPROM_SIZE/2) {
                state_subcount = 0;
                state_count = 0;
                ret = 0;
            }
        }
    }

    return ret;
}

//---------------------------------------------
