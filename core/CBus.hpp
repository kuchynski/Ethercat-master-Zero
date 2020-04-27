//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef cbus_H
#define cbus_H	

#include <vector>
#include <cstdint>
#include "CDatagram.hpp"
#include "CSlave.hpp"
#include "CNetworkAdapter.hpp"

class CBus
{
public:
    CBus();
    ~CBus();

    unsigned int Send(CNetworkAdapter *adapter);    
    void Clear();

private:
    vector<CSlave> slaves;
};

#endif
                      
