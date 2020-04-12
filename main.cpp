//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include <time.h>
#include <unistd.h>

#include "CLinuxNetworkAdapter.hpp"
#include "CLinuxTimer.hpp"
#include "CModuleScanBus.hpp"

using namespace std;

int main(void)
{
    CLinuxNetworkAdapter adapter("enp7s0");
    CLinuxTimer timer(1000);
    CScheduler scheduler(&timer, &adapter);
    CModuleScanBus module0(scheduler.GetNewModuleIndex());
    //CModuleScanBus module1(&scheduler);

    scheduler.AddModule(&module0);
    //scheduler.AddModule(&module1);

    cout << "Master 0" << endl;

    scheduler.Run();
}
