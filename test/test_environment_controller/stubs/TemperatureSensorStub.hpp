#ifndef TEMPERATURE_SENSOR_STUB
#define TEMPERATURE_SENSOR_STUB

#include <cstdint>
#include <limits>

#include "TemperatureSensor.hpp"

class TemperatureSensorStub: public TemperatureSensor
{
public:
    TemperatureSensorStub(): mReturnValue(std::numeric_limits<int32_t>::max()) {}
    explicit TemperatureSensorStub(int32_t returnValue): mReturnValue(returnValue){}
    int32_t getTemperature() override {
        return mReturnValue;
    }
    void setTemperature(int32_t value) {
        mReturnValue = value;
    }
    void reset() {
        mReturnValue = std::numeric_limits<uint32_t>::max();
    }
private:
    int32_t mReturnValue;
};

#endif  // TEMPERATURE_SENSOR_STUB