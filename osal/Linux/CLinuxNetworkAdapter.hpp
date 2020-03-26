//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef clinuxnetworkadapter_H
#define clinuxnetworkadapter_H

#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/eventfd.h>

#include "CNetworkAdapter.hpp"

class CLinuxNetworkAdapter : public CNetworkAdapter
{
public:
    CLinuxNetworkAdapter(const char *name);
    ~CLinuxNetworkAdapter();

    bool Send(unsigned char *buf, unsigned int size);
    int Receive(unsigned char *buf);
    bool IsCreate() {return fd >= 0;}

private:
    int fd;
    struct sockaddr_ll dest_addr;
    struct iovec iov;

    void Close();
};

#endif
                      
