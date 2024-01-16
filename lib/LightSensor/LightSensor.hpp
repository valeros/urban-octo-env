#ifndef AMBIENT_LIGHT_SENSOR
#define AMBIENT_LIGHT_SENSOR

#include <stdint.h>

class LightSensor
{
public:
    virtual float get_lux_value() = 0;
    virtual ~LightSensor() = default;
};

#endif  // AMBIENT_LIGHT_SENSOR