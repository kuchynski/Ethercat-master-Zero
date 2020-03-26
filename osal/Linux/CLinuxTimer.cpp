//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include "CLinuxTimer.hpp"

using namespace std;

//---------------------------------------------
CLinuxTimer::CLinuxTimer(uint64_t p): CTimer(GetTimeUs(), p)
{}

//---------------------------------------------
uint64_t CLinuxTimer::GetTimeUs()
{
    struct timespec linux_time;
    
    clock_gettime(CLOCK_REALTIME, &linux_time);
	return linux_time.tv_sec * 1000000ULL + linux_time.tv_nsec / 1000;
}

//---------------------------------------------
void CLinuxTimer::SleepUntilUs(uint64_t awake_time)
{
    struct timespec linux_time;

    linux_time.tv_sec = awake_time / 1000000;
    linux_time.tv_nsec = (awake_time % 1000000) * 1000;
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &linux_time, 0);
}

//---------------------------------------------
