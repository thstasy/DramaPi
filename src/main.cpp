#include <iostream>

#include "DramaEngine.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"

int main()
{
    std::cout << "============================\n";
    std::cout << "🎭 Drama Pi\n";
    std::cout << "============================\n";

    TemperatureMonitor temperatureMonitor;
    DramaEngine dramaEngine;

    SystemStatus status;
    status.cpuTemperature = temperatureMonitor.getTemperature();

    std::cout << "CPU Temp: "
              << status.cpuTemperature
              << " °C\n";

    std::cout << "Drama Pi: "
              << dramaEngine.comment(status)
              << "\n";

    return 0;
}