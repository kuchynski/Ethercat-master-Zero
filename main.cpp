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
#include "CScheduler.hpp"

using namespace std;

int main(void)
{
    CLinuxNetworkAdapter adapter("enp7s0");
    CLinuxTimer timer(1000);
    CScheduler scheduler(&timer, &adapter);
    CModule module0(&scheduler);
    CModule module1(&scheduler);

    scheduler.AddModule(&module0);
    scheduler.AddModule(&module1);

    cout << "Master 0" << endl;

    scheduler.Run();
}
