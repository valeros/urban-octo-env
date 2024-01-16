#ifndef HUMIDITY_SENSOR_STUB
#define HUMIDITY_SENSOR_STUB

#include <cstdint>
#include <limits>

#include "HumiditySensor.hpp"

class HumiditySensorStub: public HumiditySensor
{
public:
    HumiditySensorStub(): mReturnValue(std::numeric_limits<uint32_t>::max()) {}
    explicit HumiditySensorStub(uint32_t returnValue): mReturnValue(returnValue){}
    uint32_t getHumidity() override {
        return mReturnValue;
    }
    void setHumidity(uint32_t value) {
        mReturnValue = value;
    }
    void reset() {
        mReturnValue = std::numeric_limits<uint32_t>::max();
    }
private:
    uint32_t mReturnValue;
};

#endif  // HUMIDITY_SENSOR_STUB