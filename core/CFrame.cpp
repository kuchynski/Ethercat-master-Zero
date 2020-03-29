//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CFrame.hpp"
#include "CNetworkAdapter.hpp"

using namespace std;

#define ETHERNET_HEADER_START   0
#define ETHERNET_HEADER_SIZE    14
#define ETHERCAT_HEADER_START   (ETHERNET_HEADER_START + ETHERNET_HEADER_SIZE)
#define ETHERCAT_HEADER_SIZE    2
#define DATAGRAM_START          (ETHERCAT_HEADER_START + ETHERCAT_HEADER_SIZE)
#define DATAGRAM_HEADER_SIZE    10
#define DATAGRAM_WC_SIZE        2

#define MIN_FRAME_SIZE          (ETHERNET_HEADER_SIZE + ETHERCAT_HEADER_SIZE + DATAGRAM_HEADER_SIZE + DATAGRAM_WC_SIZE)
#define MIN_DATAGRAM_SIZE       (DATAGRAM_HEADER_SIZE + DATAGRAM_WC_SIZE)

//---------------------------------------------
CFrame::CFrame() //: buffer(NULL)
{
}

//---------------------------------------------
CFrame::CFrame(unsigned char *recieve_buf, int recieve_size)
{
    // parsing the frame
//cout << "parse: ";
    if(recieve_size >= MIN_FRAME_SIZE) {
        if(recieve_buf[ETHERNET_HEADER_START+12] != 0x88 || recieve_buf[ETHERNET_HEADER_START+12] != 0xA4) {
            int frame_size = recieve_buf[ETHERCAT_HEADER_START+0] + ((recieve_buf[ETHERCAT_HEADER_START+1] & 0x07) << 8);
            //for(int j = 0; j < recieve_size; j ++) cout << hex << ((int)recieve_buf[j]) << " "; cout << endl;
//cout << frame_size << " frame_size, ";
            unsigned char *datagram_buf = recieve_buf + DATAGRAM_START;
            while(frame_size >= MIN_DATAGRAM_SIZE) {
                int datagram_data_size = datagram_buf[6] + ((datagram_buf[7] & 0x07) << 8);
                int datagram_size = datagram_data_size + DATAGRAM_HEADER_SIZE + DATAGRAM_WC_SIZE;
//cout << datagram_size << " datagram_size" << endl;
                bool more_datagram = datagram_buf[7] & 0x80;
                CDatagram *datagram = new CDatagram(datagram_buf, datagram_size);

                p_datagrams << datagram;

                if(!more_datagram)
                    break;
                datagram_buf += datagram_size;
                frame_size -= datagram_size;
            }
       }
    }
}

//---------------------------------------------
CFrame::CFrame(const CFrame &fr)
{
    *this = fr;
}

//---------------------------------------------------------------------------
CFrame& CFrame::operator=(const CFrame &fr)
{
    Clear();

    p_datagrams = fr.p_datagrams;    
    return *this;
}

//---------------------------------------------------------------------------
CFrame& CFrame::operator<<(CDatagram *p_new_datagram)
{
    p_datagrams << p_new_datagram;    
    return *this;
}

//---------------------------------------------
CFrame& CFrame::operator>>(CDatagram **p_new_datagram)
{
    //cout << "f0 " << p_new_datagram << endl;
    p_datagrams >> *p_new_datagram;
    //cout << "f1 " << p_new_datagram << endl;
    return *this;
}

//---------------------------------------------
CFrame::~CFrame()
{
    Clear();
}

//---------------------------------------------
void CFrame::Clear()
{
    p_datagrams.Clear();
   // if(buffer) {
     //   delete [] buffer;
       // buffer = 0;
    //}
}

//---------------------------------------------
unsigned int CFrame::Send(CNetworkAdapter *adapter)
{
    unsigned char *buffer = adapter->GetBufferToSend();
    unsigned int size = 0;

    if(buffer && p_datagrams.GetSize()) {
        size = Gather(buffer);
        adapter->Send(buffer, size);
        p_datagrams.Clear();
    }

    return size;
}

//---------------------------------------------
unsigned int CFrame::Gather(unsigned char *buffer)
{
    unsigned int size = 0;
    unsigned char *buffer_datargams = buffer + ETHERNET_HEADER_SIZE + ETHERCAT_HEADER_SIZE;

    for(int i = 0, last_i = p_datagrams.GetSize() - 1; i <= last_i; i ++) {
//        cout << " " << more_datagram << endl;
        size += p_datagrams[i]->SetIntoBuffer(buffer_datargams + size, i < last_i);
    }

    memset(buffer, 0, 12);
    buffer[ETHERNET_HEADER_START+12] = 0x88;
    buffer[ETHERNET_HEADER_START+13] = 0xA4;
    buffer[ETHERCAT_HEADER_START+0] = size;
    buffer[ETHERCAT_HEADER_START+1] = 0x10 + ((size >> 8) & 0x07);

    return size + ETHERNET_HEADER_SIZE + ETHERCAT_HEADER_SIZE;
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
