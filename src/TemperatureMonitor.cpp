#include "TemperatureMonitor.h"
#include "CommandRunner.h"

#include <string>

double TemperatureMonitor::getTemperature() const
{
    std::string raw = runCommand("vcgencmd measure_temp");

    std::size_t start = raw.find('=');
    std::size_t end = raw.find('\'');

    if (start == std::string::npos || end == std::string::npos)
    {
        return -1.0;
    }

    std::string number = raw.substr(start + 1, end - start - 1);
    return std::stod(number);
}