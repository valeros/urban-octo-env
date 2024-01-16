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

#include "formatters/BasicFormatter.hpp"
#include "EnvironmentalData.hpp"

void test_basic_formatter_default_input(void)
{
    const uint32_t OUTPUT_BUFFER_SIZE = 512;
    char outputBuffer[OUTPUT_BUFFER_SIZE] = {0};

    EnvironmentalData data;
    data.humidity = 25000;
    data.temperature = 10000;

    BasicFormatter formatter;
    formatter.format(data, outputBuffer, OUTPUT_BUFFER_SIZE);

    char expectedOutput[OUTPUT_BUFFER_SIZE] = {};
    snprintf(
        expectedOutput, OUTPUT_BUFFER_SIZE,
        "Temperature: %d, Humidity: %u\r\n",
        data.temperature, data.humidity);

    TEST_ASSERT_EQUAL_STRING(expectedOutput, outputBuffer);
}

void test_basic_formatter_negative_temperature(void)
{
    const uint32_t OUTPUT_BUFFER_SIZE = 512;
    char outputBuffer[OUTPUT_BUFFER_SIZE] = {0};

    EnvironmentalData data;
    data.humidity = 10000;
    data.temperature = -5000;

    BasicFormatter formatter;
    formatter.format(data, outputBuffer, OUTPUT_BUFFER_SIZE);

    char expectedOutput[OUTPUT_BUFFER_SIZE] = {};
    snprintf(
        expectedOutput, OUTPUT_BUFFER_SIZE,
        "Temperature: %d, Humidity: %u\r\n",
        data.temperature, data.humidity);

    TEST_ASSERT_EQUAL_STRING(expectedOutput, outputBuffer);
}

void test_basic_formatter_zero_temperature_and_humidity(void)
{
    const uint32_t OUTPUT_BUFFER_SIZE = 512;
    char outputBuffer[OUTPUT_BUFFER_SIZE] = {0};

    EnvironmentalData data;
    data.humidity = 0;
    data.temperature = 0;

    BasicFormatter formatter;
    formatter.format(data, outputBuffer, OUTPUT_BUFFER_SIZE);

    char expectedOutput[OUTPUT_BUFFER_SIZE] = {};
    snprintf(
        expectedOutput, OUTPUT_BUFFER_SIZE,
        "Temperature: %d, Humidity: %u\r\n",
        data.temperature, data.humidity);

    TEST_ASSERT_EQUAL_STRING(expectedOutput, outputBuffer);
}
