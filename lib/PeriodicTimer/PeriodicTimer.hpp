#ifndef PERIODIC_TIMER
#define PERIODIC_TIMER

class PeriodicTimer
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool timerFired() = 0;
    virtual void reset() = 0;
    virtual ~PeriodicTimer() = default;
};

#endif  // PERIODIC_TIMER