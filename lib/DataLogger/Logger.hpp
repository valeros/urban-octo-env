#ifndef LOGGER_OBVERVER
#define LOGGER_OBVERVER

#include <cstdio>

#include "EnvironmentObserver.hpp"

class EnvironmentalData;
class OutputFormatter;

// Definition of an event handler for mouse notifications.
class Logger : public EnvironmentObserver
{
public:
  explicit Logger(OutputFormatter&);
  void notification(const EnvironmentalData&) override;
private:
  OutputFormatter& mFormatter;
};

#endif  // LOGGER_OBVERVER
