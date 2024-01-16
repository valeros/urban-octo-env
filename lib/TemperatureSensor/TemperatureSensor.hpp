#ifndef TEMPERATURE_SENSOR
#define TEMPERATURE_SENSOR

#include <cstdio>

class TemperatureSensor
{
public:
    virtual int32_t getTemperature() = 0;
    virtual ~TemperatureSensor() = default;
};

#endif  // TEMPERATURE_SENSOR