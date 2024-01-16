#include "STM32Timer.hpp"

#include "stm32f4xx_hal.h"

static volatile bool TimerFired = false;
static volatile bool TimerEnabled = false;
static volatile uint32_t ticks = 0;

STM32Timer::STM32Timer(uint32_t msTimeOut) : mTimeOut(msTimeOut) {}

void STM32Timer::start()
{
    ticks = HAL_GetTick();
    TimerEnabled = true;
}

void STM32Timer::reset()
{
    ticks = 0;
}

void STM32Timer::stop()
{
    ticks = 0;
    TimerEnabled = false;
}

bool STM32Timer::timerFired()
{
    if (TimerEnabled && (HAL_GetTick() - ticks > this->mTimeOut))
    {
        return true;
    }
    return false;
}