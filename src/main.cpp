#include <iostream>

#include "CpuMonitor.h"
#include "DramaEngine.h"
#include "MoodEngine.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"

int main()
{
    std::cout << "============================\n";
    std::cout << "🎭 Drama Pi\n";
    std::cout << "============================\n";

    TemperatureMonitor temperatureMonitor;
    CpuMonitor cpuMonitor;
    MoodEngine moodEngine;
    DramaEngine dramaEngine;

    SystemStatus status;
    status.cpuTemperature = temperatureMonitor.getTemperature();
    status.cpuUsage = cpuMonitor.getCpuUsage();

    Mood mood = moodEngine.evaluate(status);

    std::cout << "CPU Temp: "
              << status.cpuTemperature
              << " °C\n";

    std::cout << "CPU Usage: "
              << status.cpuUsage
              << " %\n";

    std::cout << "Drama Pi: "
              << dramaEngine.comment(mood)
              << "\n";

    return 0;
}