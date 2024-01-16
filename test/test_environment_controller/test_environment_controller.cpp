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

#include "EnvironmentController.hpp"
#include "formatters/OutputFormatter.hpp"

#include "stubs/TemperatureSensorStub.hpp"
#include "stubs/HumiditySensorStub.hpp"
#include "stubs/PeriodicTimerStub.hpp"
#include "stubs/EnvironmentObserverStub.hpp"

TemperatureSensorStub temperatureSensorStub;
HumiditySensorStub humiditySensorStub;
PeriodicTimerStub periodicTimerStub;
EnvironmentObserverStub observer;

void setUp(void)
{
    temperatureSensorStub.reset();
    humiditySensorStub.reset();
    observer.reset();
}

EnvironmentController makeEnvironmentController(int32_t temperatureThreshold, uint32_t humidityThreshold)
{
    EnvironmentController app(temperatureSensorStub, humiditySensorStub, periodicTimerStub);

    app.setUpperThresholdTemperature(temperatureThreshold);
    app.setUpperThresholdHumidity(humidityThreshold);

    return app;
}

EnvironmentController makeEnvironmentController(int32_t temperatureThreshold, uint32_t humidityThreshold, EnvironmentObserver &observer)
{
    EnvironmentController app = makeEnvironmentController(temperatureThreshold, humidityThreshold);
    app.add_observer(observer);
    return app;
}

void test_environment_controller_observer_notified_when_temperature_exceeds_threshold(void)
{
    temperatureSensorStub.setTemperature(12000);
    humiditySensorStub.setHumidity(25000);

    EnvironmentController app = makeEnvironmentController(
        temperatureSensorStub.getTemperature() + 1000,
        humiditySensorStub.getHumidity() - 1000,
        observer);

    app.run();

    TEST_ASSERT_TRUE(observer.wasCalled());
}

void test_environment_controller_observer_notified_when_humidity_exceeds_threshold(void)
{
    temperatureSensorStub.setTemperature(12000);
    humiditySensorStub.setHumidity(99000);

    EnvironmentController app = makeEnvironmentController(
        temperatureSensorStub.getTemperature() - 1000,
        humiditySensorStub.getHumidity() + 1000,
        observer);

    app.run();

    TEST_ASSERT_TRUE(observer.wasCalled());
}

void test_environment_controller_observer_not_notified_if_data_below_threshold(void)
{
    temperatureSensorStub.setTemperature(10000);
    humiditySensorStub.setHumidity(10000);

    EnvironmentController app = makeEnvironmentController(
        temperatureSensorStub.getTemperature() + 1000,
        humiditySensorStub.getHumidity() + 1000,
        observer);

    app.run();

    TEST_ASSERT_FALSE(observer.wasCalled());
}

void test_environment_controller_observer_receives_correct_environmental_data(void)
{
    temperatureSensorStub.setTemperature(30000);
    humiditySensorStub.setHumidity(80000);

    EnvironmentController app = makeEnvironmentController(
        temperatureSensorStub.getTemperature() - 1000,
        humiditySensorStub.getHumidity() - 1000,
        observer);

    app.run();
    EnvironmentalData &receivedData = observer.getPassedData();

    TEST_ASSERT_TRUE(observer.wasCalled());
    TEST_ASSERT_EQUAL_MESSAGE(
        temperatureSensorStub.getTemperature(), receivedData.temperature,
        "Temperature values don't match!");
    TEST_ASSERT_EQUAL_MESSAGE(
        humiditySensorStub.getHumidity(), receivedData.humidity,
        "Humidity values don't match!");
}

int main()
{
    UNITY_BEGIN();

#ifdef STM32_TARGET
    HAL_Init();
    HAL_Delay(1000);
#endif

    RUN_TEST(test_environment_controller_observer_notified_when_temperature_exceeds_threshold);
    RUN_TEST(test_environment_controller_observer_notified_when_humidity_exceeds_threshold);
    RUN_TEST(test_environment_controller_observer_not_notified_if_data_below_threshold);
    RUN_TEST(test_environment_controller_observer_receives_correct_environmental_data);

    UNITY_END();

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
