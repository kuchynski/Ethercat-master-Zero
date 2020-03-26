//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <unistd.h>
#include <string.h>

#include "CLinuxNetworkAdapter.hpp"


using namespace std;

//---------------------------------------------
CLinuxNetworkAdapter::CLinuxNetworkAdapter(const char *name)
{
    fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(fd >= 0) {
        struct ifreq ifr;
        struct sockaddr_ll sll;

    	memset(&ifr, 0, sizeof(struct ifreq));
    	memset(ifr.ifr_name, '\0', IFNAMSIZ);
	    strncpy(ifr.ifr_name, name, sizeof(name));
        ifr.ifr_flags |= IFF_UP;
    	if(ioctl(fd, SIOCSIFFLAGS, &ifr) < 0)
		    Close();
    	if(ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
		    Close();

	    memset(&dest_addr, 0, sizeof(struct sockaddr_ll));
	    dest_addr.sll_family   = AF_PACKET;
	    dest_addr.sll_protocol = htons(ETH_P_ALL);
	    dest_addr.sll_ifindex  = ifr.ifr_ifindex;
	    dest_addr.sll_halen    = ETH_ALEN;
        memset(&dest_addr.sll_addr, 0xFF, 6);
	    //rt_eth_aton(dest_addr.sll_addr, "FF:FF:FF:FF:FF:FF");
    	memset(&sll, 0, sizeof(sll));
	    sll.sll_family   = AF_PACKET;
	    sll.sll_ifindex  = ifr.ifr_ifindex;
	    sll.sll_protocol = htons(ETH_P_ALL);
    	if(bind(fd, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		    Close();

    	if(ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
		    Close();

        if((ifr.ifr_flags & IFF_PROMISC) == 0) {
    		ifr.ifr_flags |= IFF_PROMISC;
        	if(ioctl(fd, SIOCSIFFLAGS, &ifr) < 0)
    		    Close();
        }
    }

    cout << "open adapter " << name << " = " << fd << endl;
}

//---------------------------------------------
CLinuxNetworkAdapter::~CLinuxNetworkAdapter()
{
    cout << "close adapter" << endl;
    Close();
}

//---------------------------------------------
void CLinuxNetworkAdapter::Close()
{
    if(fd >= 0) {
        close(fd);
        fd = -1;
    }
}
//---------------------------------------------
bool CLinuxNetworkAdapter::Send(unsigned char *buf, unsigned int size)
{
    bool ret = false;
	struct msghdr msg = {0};

    //cout << "send" << endl;

	iov.iov_base = buf;
	iov.iov_len = size;
	msg.msg_iov = &iov;
	msg.msg_name = &dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iovlen = 1;

	if(sendmsg(fd, &msg, 0) > 0) {
        ret = true;
    }
    else
        cout << "bad send" << endl;

    CNetworkAdapter::Send(buf, size);

    return ret;
}

//---------------------------------------------
int CLinuxNetworkAdapter::Receive(unsigned char *buf)
{
	return recvfrom(fd, buf, SEGMENT_SIZE, MSG_TRUNC | MSG_DONTWAIT, NULL, NULL);
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
