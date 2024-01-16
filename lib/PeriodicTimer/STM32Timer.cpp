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
}

void STM32Timer::stop()
{
    TimerEnabled = false;
}

bool STM32Timer::timerFired()
{
    uint32_t current_tick = HAL_GetTick();
    if ((current_tick - ticks) > this->mTimeOut)
    {
        ticks = HAL_GetTick();
        return true;
    }
    return false;
}