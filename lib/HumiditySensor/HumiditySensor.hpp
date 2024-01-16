#ifndef HUMIDITY_SENSOR
#define HUMIDITY_SENSOR

#include <cstdint>

class HumiditySensor
{
public:
    virtual uint32_t getHumidity() = 0;
    virtual ~HumiditySensor() = default;
};

#endif  // HUMIDITY_SENSOR