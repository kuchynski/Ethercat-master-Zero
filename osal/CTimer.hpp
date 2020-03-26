//
// Ethercat master Zero
// kuchynskiandrei@gmail.com
// 2020
//

#ifndef ctimer_H
#define ctimer_H

class CTimer
{
public:
	CTimer(uint64_t set_current_time, uint64_t set_period);
    virtual ~CTimer() {};

    virtual uint64_t GetTimeUs() = 0;
    uint64_t WaitTick();
    void SetPeriodUs(uint64_t set_period);
    uint64_t GetLastTickTimeUs() { return last_tick_time; }
    uint64_t GetNextTickTimeUs() { return next_tick_time; }

protected:
private:
    uint64_t period; // us
    uint64_t last_tick_time; // us
    uint64_t next_tick_time; // us
    virtual void SleepUntilUs(uint64_t awake_time) = 0;
};

#endif
                      
