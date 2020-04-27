//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cslave_H
#define cslave_H	

#include <cstdint>
#include <iostream>

#include "CDatagram.hpp"

using namespace std;

#define MAX_DATAGRAM_PRIORITY   4
#define SLAVE_MEMORY_SIZE       0xA00
#define SLAVE_EEPROM_SIZE       0x100

struct SSlaveMemory
{
    unsigned char reg[SLAVE_MEMORY_SIZE];
    unsigned char eeprom[SLAVE_EEPROM_SIZE];
};

class CSlave
{
public:
    CSlave();
    ~CSlave();

    virtual uint8_t GetByte(const unsigned int address) const;
    virtual uint16_t GetWord(const unsigned int address) const;
    virtual uint32_t GetDWord(const unsigned int address) const;
    virtual uint64_t GetQWord(const unsigned int address) const;
    virtual uint8_t* GetMemory(const unsigned int address);

    virtual void SetByte(const unsigned int address, const uint8_t data);
    virtual void SetWord(const unsigned int address, const uint16_t data);
    virtual void SetDWord(const unsigned int address, const uint32_t data);
    virtual void SetQWord(const unsigned int address, const uint64_t data);
    virtual void SetMemory(const unsigned int address, const uint8_t *data, const unsigned int size);

    void UploadMemoryStep1(CDatagram *datagram, const unsigned int address, const unsigned int size);
    void UploadMemoryStep2(CDatagram *datagram);

    void DownloadMemoryStep(CDatagram *datagram, const unsigned int address, const uint8_t *data, const unsigned int size);

private:
    struct SSlaveMemory memory;
};

#endif
                      
