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

struct SDatagramParam
{
    unsigned char index;
    unsigned char command;
    unsigned int size_data;
    unsigned int priority;
    unsigned int address;
};

class CDatagram
{
public:
    CDatagram(struct SDatagramParam &set_param);    // tx datagram
    CDatagram(unsigned char *buf, int size_buffer); // rx datagram
    ~CDatagram();

    void SetParam(struct SDatagramParam &set_param);
    unsigned int SetIntoBuffer(unsigned char *out_buf, bool more_datargam);
    unsigned char GetIndex() { return param.index; }
    unsigned int GetPriority() { return param.priority; }
    unsigned char GetCommand() const { return param.command; }
    unsigned int GetAddress() const { return param.address; }

private:
    unsigned char *buffer;
    unsigned int size_buffer;
    struct SDatagramParam param;
};

#endif
                      
