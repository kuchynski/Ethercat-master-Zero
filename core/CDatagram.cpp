//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CDatagram.hpp"

using namespace std;

#define MAX_DATAGRAM_SIZE   1500
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
CDatagram::CDatagram(const uint8_t set_index) : index(set_index) // tx datagram
{
    struct SDatagramParam param;
    param.index = index;
    param.command = 0;
    param.size_data = 0;
    param.priority = 0;
    param.address = 0;
    SetParam(param);
}

//---------------------------------------------
CDatagram::CDatagram(struct SDatagramParam &param) // tx datagram
{
    index = param.index;
    SetParam(param);
}

//---------------------------------------------
CDatagram::CDatagram(unsigned char *buf, int size_buffer) // rx datagram
{    
    command = buf[OFFSET_CMD];
    index = buf[OFFSET_IDX];
    memcpy(&address, buf + OFFSET_ADDRESS, sizeof(uint32_t));
    memcpy(&size_data, buf + OFFSET_SIZE, sizeof(uint16_t));
    size_data &= 0x07FF;
    memcpy(buffer, buf + HEADER_SIZE, size_data);
    wc = buf[size_buffer-2];
    wc += ((uint16_t)buf[size_buffer-1] << 8);
//cout << "rx: " << (int)command << " " << (int)index << " " << (int)address << " " << (int)size_data << " " << (int)wc << endl;
}

//---------------------------------------------
void CDatagram::SetParam(struct SDatagramParam &param)
{
    command = param.command;
    address = param.address;
    size_data = param.size_data;
}

//---------------------------------------------
unsigned int CDatagram::SetIntoBuffer(unsigned char *out_buf, bool more_datargam)
{
    const unsigned int size_buffer = HEADER_SIZE + WC_SIZE + size_data;

    out_buf[OFFSET_CMD] = command;
    out_buf[OFFSET_IDX] = index;
    memcpy(out_buf + OFFSET_ADDRESS, &address, sizeof(uint32_t));
    memcpy(out_buf + OFFSET_SIZE, &size_data, sizeof(uint16_t));

    memcpy(out_buf + HEADER_SIZE, buffer, size_data);
    if(more_datargam)
        out_buf[OFFSET_MORE_DATAGRAM] |= 0x80;
    
    out_buf[OFFSET_IRQ+0] = 0;
    out_buf[OFFSET_IRQ+1] = 0;
    out_buf[size_buffer-1] = 0;
    out_buf[size_buffer-2] = 0;

    return size_buffer;
}

//---------------------------------------------
void CDatagram::GetData(unsigned char *out_buffer) const
{
    memcpy(out_buffer, buffer, size_data);
}

//---------------------------------------------
void CDatagram::SetData(const unsigned char *in_buffer)
{
    memcpy(buffer, in_buffer, size_data);
}

//---------------------------------------------
//---------------------------------------------
