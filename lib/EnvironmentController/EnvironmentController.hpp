#ifndef ENVIRONMENT_CONTROLLER
#define ENVIRONMENT_CONTROLLER

#include <etl/observer.h>
#include "EnvironmentObserver.hpp"

const unsigned int MAX_OBSERVERS_NUM = 10;

class TemperatureSensor;
class HumiditySensor;
class PeriodicTimer;
#ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
class LightSensor;
#endif 

class EnvironmentController: public etl::observable<EnvironmentObserver, MAX_OBSERVERS_NUM>
{
public:
    EnvironmentController(
        TemperatureSensor&,
        HumiditySensor&,
        PeriodicTimer&
        #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
        , LightSensor&
        #endif 
    );
    void run();
    void setUpperThresholdTemperature(int32_t temperature);
    void setUpperThresholdHumidity(uint32_t humidity);
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    void setUpperThresholdLight(float lux_value);
    #endif
    ~EnvironmentController() = default;
private:
    TemperatureSensor& mTemperatureSensor;
    HumiditySensor& mHumiditySensor;
    PeriodicTimer& mTimer;
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    LightSensor& mLightSensor;
    #endif
    int32_t mUpperTemperatureThreshold;
    uint32_t mUpperHumidityThreshold;
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    float mUpperLuxThreshold;
    #endif
};

#endif  // ENVIRONMENT_CONTROLLER
