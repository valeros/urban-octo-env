#include <cstdio>

#include "EnvironmentController.hpp"
#include "TemperatureSensor.hpp"
#include "HumiditySensor.hpp"
#include "PeriodicTimer.hpp"
#include "EnvironmentalData.hpp"
#ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
#include "LightSensor.hpp"
#endif

EnvironmentController::EnvironmentController(
    TemperatureSensor& ts,
    HumiditySensor& hs,
    PeriodicTimer& pt
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    ,LightSensor& ls
    #endif
):
    mTemperatureSensor(ts),
    mHumiditySensor(hs),
    mTimer(pt),
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    mLightSensor(ls),
    #endif
    mUpperTemperatureThreshold(1000),
    mUpperHumidityThreshold(1000)
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    ,mUpperLuxThreshold(1000)
    #endif
{
    this->mTimer.start();
}

void EnvironmentController::setUpperThresholdTemperature(int32_t temperature) {
    this->mUpperTemperatureThreshold = temperature;
}

void EnvironmentController::setUpperThresholdHumidity(uint32_t humidity) {
    this->mUpperHumidityThreshold = humidity;
}

#ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
void EnvironmentController::setUpperThresholdLight(float lux_value) {
    this->mUpperLuxThreshold = lux_value;
}
#endif

void EnvironmentController::run() {
    if (this->mTimer.timerFired()){
        this->mTimer.reset();

        auto temperature = mTemperatureSensor.getTemperature();
        auto humidity = mHumiditySensor.getHumidity();
        #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
        auto ambient_light = mLightSensor.get_lux_value();
        #endif

        bool notificationRequired = (temperature > mUpperTemperatureThreshold
            || humidity > mUpperHumidityThreshold
            #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
            || ambient_light > mUpperLuxThreshold
            #endif
        );
        
        if (notificationRequired) {
            EnvironmentalData data{
                temperature,
                humidity
                #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
                , ambient_light
                #endif
            };
            this->notify_observers(data);
        }
    }
}