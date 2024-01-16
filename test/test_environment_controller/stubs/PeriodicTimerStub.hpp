#ifndef PERIODIC_TIMER_STUB
#define PERIODIC_TIMER_STUB

#include "PeriodicTimer.hpp"

class PeriodicTimerStub: public PeriodicTimer
{
public:
    void start() override {}
    void stop() override {}
    void reset() override {}
    bool timerFired() override {
        return true;
    }
};

#endif  // PERIODIC_TIMER_STUB