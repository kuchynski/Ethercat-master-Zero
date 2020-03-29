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

    buffer[OFFSET_CMD] = param.command;
    buffer[OFFSET_IDX] = param.index;
    buffer[OFFSET_ADDRESS+0] = param.address;
    buffer[OFFSET_ADDRESS+1] = param.address >> 8;
    buffer[OFFSET_ADDRESS+2] = param.address >> 16;
    buffer[OFFSET_ADDRESS+3] = param.address >> 24;
    buffer[OFFSET_SIZE+0] = param.size_data;
    buffer[OFFSET_SIZE+1] = param.size_data >> 8;
}

//---------------------------------------------
CDatagram::CDatagram(unsigned char *buf, int size_buffer)
{
    buffer = new unsigned char[size_buffer];
    memcpy(buffer, buf, size_buffer);

    param.command = buffer[OFFSET_CMD];
    param.index = buffer[OFFSET_IDX];
    param.address = buffer[OFFSET_ADDRESS+0];
    param.address += ((unsigned int)buffer[OFFSET_ADDRESS+1] << 8);
    param.address += ((unsigned int)buffer[OFFSET_ADDRESS+2] << 16);
    param.address += ((unsigned int)buffer[OFFSET_ADDRESS+3] << 24);
    param.size_data = buffer[OFFSET_SIZE+0];
    param.size_data += ((unsigned int)buffer[OFFSET_SIZE+1] << 8);
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
