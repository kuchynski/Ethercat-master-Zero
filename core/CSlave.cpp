//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CSlave.hpp"

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
CSlave::CSlave()
{
}

//---------------------------------------------
//---------------------------------------------
CSlave::~CSlave()
{
}
//---------------------------------------------
uint8_t CSlave::GetByte(const unsigned int address) const
{
    return memory.reg[address];
}

//---------------------------------------------
uint16_t CSlave::GetWord(const unsigned int address) const
{
    return *(uint16_t*)(memory.reg + address);
}

//---------------------------------------------
uint32_t CSlave::GetDWord(const unsigned int address) const
{
    return *(uint32_t*)(memory.reg + address);
}

//---------------------------------------------
uint64_t CSlave::GetQWord(const unsigned int address) const
{
    return *(uint64_t*)(memory.reg + address);
}

//---------------------------------------------
void CSlave::SetByte(const unsigned int address, const uint8_t data)
{
    memory.reg[address] = data;
}

//---------------------------------------------
void CSlave::SetWord(const unsigned int address, const uint16_t data)
{
    *(uint16_t*)(memory.reg + address) = data;
}

//---------------------------------------------
void CSlave::SetDWord(const unsigned int address, const uint32_t data)
{
    *(uint32_t*)(memory.reg + address) = data;
}

//---------------------------------------------
void CSlave::SetQWord(const unsigned int address, const uint64_t data)
{
    *(uint64_t*)(memory.reg + address) = data;
}

//---------------------------------------------
uint8_t* CSlave::GetMemory(const unsigned int address)
{
    return memory.reg + address;
}

//---------------------------------------------
void CSlave::SetMemory(const unsigned int address, const uint8_t *data, const unsigned int size)
{
    memcpy(memory.reg + address, data, size);
}

//---------------------------------------------
void CSlave::UploadMemoryStep1(CDatagram *datagram, const unsigned int address, const unsigned int size)
{
    struct SDatagramParam param;
    param.command = 1; // APRD
    param.size_data = size;
    param.address = address;

    datagram->SetParam(param);
}

//---------------------------------------------
void CSlave::UploadMemoryStep2(CDatagram *datagram)
{
    const unsigned int offset_address = datagram->GetAddress() >> 16;
    if(offset_address < SLAVE_MEMORY_SIZE)
        datagram->GetData(memory.reg + offset_address);
//    else
//        cout << "error " << offset_address << endl;
//cout << "a " << offset_address << " " << (int)memory.reg[offset_address] << "=" << endl;
}

//---------------------------------------------
void CSlave::DownloadMemoryStep(CDatagram *datagram, const unsigned int address, const uint8_t *data, const unsigned int size)
{
    struct SDatagramParam param;
    param.command = 2; // APWD
    param.size_data = size;
    param.address = address;

    datagram->SetParam(param);
    datagram->SetData(data);
}

//---------------------------------------------
