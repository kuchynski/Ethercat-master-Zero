//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef clinuxtimer_H
#define clinuxtimer_H

#include "CTimer.hpp"

class CLinuxTimer: public CTimer
{
public:
       CLinuxTimer(uint64_t p = 0);
       ~CLinuxTimer() {}

       uint64_t GetTimeUs();
private:
       void SleepUntilUs(uint64_t awake_time);
};

#endif
                      
