#include <cstdio>

#include "StatusLED.hpp"

#include "stm32f4xx_hal.h"

void StatusLED::notification(const EnvironmentalData &data)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

bool StatusLED::getState()
{
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
}
