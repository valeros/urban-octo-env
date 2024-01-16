#ifndef ENVIRONMENTAL_DATA
#define ENVIRONMENTAL_DATA

#include <cstdio>
#include <cstdint>

struct EnvironmentalData
{
    int32_t temperature;
    uint32_t humidity;
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    float ambient_light;
    #endif
};

#endif  // ENVIRONMENTAL_DATA