#ifndef JSON_FORMATTER
#define JSON_FORMATTER

#include <cstdio>

#include "OutputFormatter.hpp"

class EnvironmentalData;

class JSONFormatter: public OutputFormatter
{
public:
    void format(const EnvironmentalData&, char*, uint32_t) override;
};

#endif // JSON_FORMATTER