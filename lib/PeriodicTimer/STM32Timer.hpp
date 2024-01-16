#ifndef STM32_TIMER_H
#define STM32_TIMER_H

#include <cstdio>
#include "PeriodicTimer.hpp"

class STM32Timer : public PeriodicTimer
{
public:
    explicit STM32Timer(uint32_t timeOut) noexcept;
    void start() override;
    void stop() override;
    void reset() override;
    bool timerFired() override;

private:
    uint32_t mTimeOut;
};

#endif // STM32_TIMER_H