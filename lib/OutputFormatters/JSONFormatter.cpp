#include <ArduinoJson.h>

#include "formatters/JSONFormatter.hpp"
#include "EnvironmentalData.hpp"

const int16_t JSON_DOC_BUFFER_SIZE = 512;

void JSONFormatter::format(const EnvironmentalData& data, char* outputBuffer, const uint32_t bufferSize) {
    StaticJsonDocument<JSON_DOC_BUFFER_SIZE> doc;
    doc["temperature"] = data.temperature;
    doc["humidity"] = data.humidity;
    #ifdef DEV_AMBIENT_LIGHT_SENSOR_AVAILABLE
    doc["ambient_light"] = data.ambient_light;
    #endif
    serializeJson(doc, outputBuffer, bufferSize);
}
