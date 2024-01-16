#ifndef BMP180_H
#define BMP180_H

#include "HumiditySensor.hpp"
#include "TemperatureSensor.hpp"

class BMP180 : public HumiditySensor, public TemperatureSensor
{
public:
  explicit BMP180() noexcept;
  int32_t getTemperature() override;
  uint32_t getHumidity() override;

private:
  void initialize();
  void getTemperature(int32_t &temperature);
};

#endif // BMP180_H
