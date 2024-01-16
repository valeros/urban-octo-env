#include "Logger.hpp"
#include "EnvironmentalData.hpp"
#include "formatters/OutputFormatter.hpp"

namespace
{
    const uint32_t OUTPUT_BUFFER_SIZE = 512;
}

void init_usart()
{
}

Logger::Logger(OutputFormatter &formatter) : mFormatter(formatter)
{
    init_usart();
}

void Logger::notification(const EnvironmentalData &data)
{
    char outputBuffer[OUTPUT_BUFFER_SIZE] = {0};
    this->mFormatter.format(data, outputBuffer, OUTPUT_BUFFER_SIZE);
    printf("%s\r\n", outputBuffer);
    // HAL_UART_Transmit(&UartHandle, (uint8_t *)outputBuffer, strlen(outputBuffer), 1000);
}