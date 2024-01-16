/*
 Copyright (c) 2014-present PlatformIO <contact@platformio.org>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
**/

#include <cstdint>
#include <cstdio>

#include <unity.h>

#ifdef STM32_TARGET
#include "stm32f4xx_hal.h"
#endif

#include "test_basic_formatter.hpp"
#include "test_json_formatter.hpp"

int main()
{
    UNITY_BEGIN();

#ifdef STM32_TARGET
    HAL_Init();
    HAL_Delay(1000);
#endif

    RUN_TEST(test_basic_formatter_default_input);
    RUN_TEST(test_basic_formatter_zero_temperature_and_humidity);

    RUN_TEST(test_json_formatter_basic_input);
    RUN_TEST(test_json_formatter_negative_temperature);
    RUN_TEST(test_json_formatter_zero_temperature_and_humidity);

    UNITY_END(); // stop unit testing

#ifdef STM32_TARGET
    while (1)
    {
    }
#endif
}

#ifdef STM32_TARGET
extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}
#endif

void setUp(void)
{
}

void tearDown(void)
{
}