#ifndef ENVIRONMENT_OBSERVER
#define ENVIRONMENT_OBSERVER

#include <cstdio>
#include <etl/observer.h>

class EnvironmentalData;

typedef etl::observer<const EnvironmentalData&> EnvironmentObserver;

#endif  // ENVIRONMENT_OBSERVER
