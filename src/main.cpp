#include <iostream>

#include "CpuMonitor.h"
#include "DramaEngine.h"
#include "MoodEngine.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"
#include "MemoryMonitor.h"
#include "DiskMonitor.h"

int main()
{
    std::cout << "============================\n";
    std::cout << "🎭 Drama Pi\n";
    std::cout << "============================\n";

    TemperatureMonitor temperatureMonitor;
    CpuMonitor cpuMonitor;
    MoodEngine moodEngine;
    DramaEngine dramaEngine;
    MemoryMonitor memoryMonitor;
    DiskMonitor diskMonitor;

    SystemStatus status;
    status.cpuTemperature = temperatureMonitor.getTemperature();
    status.cpuUsage = cpuMonitor.getCpuUsage();
    status.memoryUsage = memoryMonitor.getMemoryUsage();
    status.diskUsage = diskMonitor.getDiskUsage();

    Mood mood = moodEngine.evaluate(status);

    std::cout << "CPU Temp: "
              << status.cpuTemperature
              << " °C\n";

    std::cout << "CPU Usage: "
              << status.cpuUsage
              << " %\n";

    std::cout << "Disk Usage: "
              << status.diskUsage
              << " %\n";

    std::cout << "Memory Usage: "
              << status.memoryUsage
              << "%\n";          

    std::cout << "Drama Pi: "
              << dramaEngine.comment(mood)
              << "\n";

    return 0;
}