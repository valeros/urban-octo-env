#include "EnvironmentController.hpp"
#include "EnvironmentalData.hpp"
#include "Logger.hpp"
#include "STM32Timer.hpp"
#include "StatusLED.hpp"

#include "BMP180.hpp"

#include "hardware.hpp"

#include "formatters/BasicFormatter.hpp"
#include "formatters/JSONFormatter.hpp"

int main(void)
{
  Hardware::init();
  printf("Initializing Application...\r\n");

  BMP180 rht_sensor;
  STM32Timer timer(1000);
  StatusLED led;
  JSONFormatter formatter;
  Logger USARTLogger(formatter);

  EnvironmentController app(rht_sensor, rht_sensor, timer);
  app.setUpperThresholdTemperature(25.0);
  app.add_observer(USARTLogger);
  app.add_observer(led);

  printf("Application started!\r\n");

  while (1)
  {
    app.run();
  }
}
