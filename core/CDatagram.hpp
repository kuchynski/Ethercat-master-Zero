//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cdatagram_H
#define cdatagram_H	

#include <cstdint>
#include <iostream>

using namespace std;

#define MAX_DATAGRAM_PRIORITY   4
#define MAX_DATAGRAM_DATA_SIZE  1486

struct SDatagramParam
{
    unsigned int priority;
    bool temporary;

    uint8_t command;
    uint8_t index;
    uint16_t size_data;
    uint32_t address;
};

class CDatagram
{
public:
    CDatagram(const uint8_t set_index);    // tx datagram
    CDatagram(const struct SDatagramParam &param, const bool set_temporary);    // tx datagram
    CDatagram(unsigned char *buf, int size_buffer); // rx datagram
    ~CDatagram() {};

    void SetParam(const struct SDatagramParam &param);
    unsigned int SetIntoBuffer(unsigned char *out_buf, bool more_datargam);

    unsigned int GetPriority() const { return priority; }
    uint8_t GetCommand() const { return command; }
    uint8_t GetIndex() const { return index; }
    uint32_t GetAddress() const { return address; }
    void GetData(unsigned char *out_buffer) const;
    void SetData(const unsigned char *in_buffer);
    bool IsTemporary() const { return temporary; }

private:
    unsigned int priority;
    bool temporary;

    uint8_t command;
    uint8_t index;
    uint32_t address;
    uint16_t size_data;
    uint16_t irq;
    unsigned char buffer[MAX_DATAGRAM_DATA_SIZE];
    uint16_t wc;
};

#endif
                      
