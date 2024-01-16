#ifndef OUTPUT_FORMATTER
#define OUTPUT_FORMATTER

#include <cstdio>

class EnvironmentalData;

class OutputFormatter
{
public:
    virtual void format(const EnvironmentalData&, char* outputBuffer, uint32_t bufferSize) = 0;
    virtual ~OutputFormatter() = default;
};

#endif  // OUTPUT_FORMATTER