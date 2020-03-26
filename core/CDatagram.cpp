//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CDatagram.hpp"

using namespace std;

#define HEADER_SIZE     10
#define WC_SIZE         2
#define OFFSET_CMD      0
#define OFFSET_IDX      1
#define OFFSET_ADDRESS  2
#define OFFSET_SIZE     6
#define OFFSET_MORE_DATAGRAM    7
#define OFFSET_IRQ      8
#define OFFSET_DATA     10

//---------------------------------------------
CDatagram::CDatagram(struct SDatagramParam &set_param) :
    param(set_param)
{
    size_buffer = HEADER_SIZE + WC_SIZE + param.size_data; 
    buffer = new unsigned char[size_buffer];
    memset(buffer, 0, size_buffer);

    buffer[OFFSET_CMD] = 0x07;
    buffer[OFFSET_IDX] = param.index;
    buffer[OFFSET_ADDRESS+0] = 0x01;
    buffer[OFFSET_ADDRESS+1] = 0x02;
    buffer[OFFSET_ADDRESS+2] = 0x03;
    buffer[OFFSET_ADDRESS+3] = 0x04;
    buffer[OFFSET_SIZE+0] = param.size_data;
    buffer[OFFSET_SIZE+1] = param.size_data >> 8;
    buffer[OFFSET_DATA] = param.priority;
}

//---------------------------------------------
CDatagram::CDatagram(unsigned char *buf, int size_buffer)
{
    buffer = new unsigned char[size_buffer];
    memcpy(buffer, buf, size_buffer);
}

//---------------------------------------------
CDatagram::~CDatagram()
{
    delete [] buffer;
}

//---------------------------------------------
void CDatagram::SetParam(struct SDatagramParam &set_param)
{
    param = set_param;
}

//---------------------------------------------
unsigned int CDatagram::SetIntoBuffer(unsigned char *out_buf, bool more_datargam)
{
    memcpy(out_buf, buffer, size_buffer);
    if(more_datargam)
        out_buf[OFFSET_MORE_DATAGRAM] |= 0x80;
    return size_buffer;
}
