#ifndef STATUS_LED
#define STATUS_LED

#include "EnvironmentObserver.hpp"

class EnvironmentalData;

// Definition of an event handler for mouse notifications.
class StatusLED : public EnvironmentObserver
{
public:
  void notification(const EnvironmentalData&) override;
  bool getState();
private:
  bool isOn;
};

#endif  // STATUS_LED
