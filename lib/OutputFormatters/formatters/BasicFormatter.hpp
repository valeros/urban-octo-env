#ifndef BASIC_FORMATTER
#define BASIC_FORMATTER

#include "OutputFormatter.hpp"

class EnvironmentalData;

class BasicFormatter: public OutputFormatter
{
public:
    void format(const EnvironmentalData&, char*, uint32_t) override;
};

#endif  // BASIC_FORMATTER