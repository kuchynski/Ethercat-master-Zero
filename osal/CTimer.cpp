//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#include <iostream>
#include "CTimer.hpp"

using namespace std;

//---------------------------------------------
CTimer::CTimer(uint64_t set_current_time, uint64_t set_period = 0): 
    period(set_period), 
    last_tick_time(set_current_time), 
    next_tick_time(set_current_time + set_period)
{}

//---------------------------------------------
void CTimer::SetPeriodUs(uint64_t set_period)
{
    period = set_period;
    last_tick_time = GetTimeUs();
    next_tick_time = last_tick_time + set_period;
}

//---------------------------------------------
uint64_t CTimer::WaitTick()
{
    SleepUntilUs(next_tick_time);
    last_tick_time = next_tick_time;
    next_tick_time += period;

    return last_tick_time;
}

//---------------------------------------------
