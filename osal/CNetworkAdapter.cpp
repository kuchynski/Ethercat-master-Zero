//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <string.h>
#include "CNetworkAdapter.hpp"

using namespace std;

//---------------------------------------------
CNetworkAdapter::CNetworkAdapter(): ptr(0)
{
    buffer = new unsigned char[SEGMENT_SIZE * NUMBER_SEGMENTS];

    for(int i = 0; i < NUMBER_SEGMENTS; i ++)
        segments[i] = buffer + (SEGMENT_SIZE * i);
}

//---------------------------------------------
CNetworkAdapter::~CNetworkAdapter()
{
    delete [] buffer;
}

//---------------------------------------------
unsigned char* CNetworkAdapter::GetBufferToSend()
{
    unsigned char *ret = segments[ptr++];
    if(ptr >= NUMBER_SEGMENTS)
        ptr = 0;
    return ret;
}

//---------------------------------------------
bool CNetworkAdapter::Send(unsigned char *buf, unsigned int size)
{
//    cout << "base send:";
//    for(int i = 0; i < size; i++)
//        cout << " " << hex << ((int)buf[i]);
//    cout << endl;
    return true;
}

//---------------------------------------------
