#include "EnvironmentalData.hpp"
#include "formatters/BasicFormatter.hpp"

void BasicFormatter::format(const EnvironmentalData &data, char *outputBuffer, uint32_t bufferSize)
{
    snprintf(
        outputBuffer, bufferSize,
        "Temperature: %lu, Humidity: %lu\r\n",
        data.temperature, data.humidity);
}