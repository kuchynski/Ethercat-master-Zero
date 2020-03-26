//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cnetworkadapter_H
#define cnetworkadapter_H

#include <cstdint>

#define NUMBER_SEGMENTS 16
#define SEGMENT_SIZE    2048

class CNetworkAdapter
{
public:
	CNetworkAdapter();
    ~CNetworkAdapter();

    unsigned char* GetBufferToSend();
    virtual bool Send(unsigned char *buf, unsigned int size);
    virtual int Receive(unsigned char *buf) = 0;
    virtual bool IsCreate() = 0;

private:
    unsigned char *buffer;
    unsigned char *segments[NUMBER_SEGMENTS];
    int ptr;
};

#endif
                      
