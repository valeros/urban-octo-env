#ifndef ENVIRONMENT_OBSERVER_STUB
#define ENVIRONMENT_OBSERVER_STUB

#include "EnvironmentObserver.hpp"
#include "EnvironmentalData.hpp"

class EnvironmentObserverStub : public EnvironmentObserver
{
public:
  EnvironmentObserverStub():mWasCalled(false){}
  void notification(const EnvironmentalData& data) override {
      mWasCalled = true;
      mData = data;
  }
  EnvironmentalData& getPassedData() {
      return mData;
  }
  bool wasCalled() {
      return mWasCalled;
  }
  void reset() {
    mWasCalled = false;
  }
private:
  bool mWasCalled;
  EnvironmentalData mData;
};

#endif  // ENVIRONMENT_OBSERVER_STUB
