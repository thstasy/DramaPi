#include <iostream>
#include <string>

#include "CpuMonitor.h"
#include "DiskMonitor.h"
#include "DramaEngine.h"
#include "EventDetector.h"
#include "MemoryMonitor.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"
#include "UptimeMonitor.h"
#include "Personality.h"

Personality parsePersonality(const std::string& name){
    if (name == "dramatic")
        return Personality::Dramatic;

    if (name == "corporate")
        return Personality::Corporate;

    if (name == "emo")
        return Personality::Emo;

    if (name == "sarcastic")
        return Personality::Sarcastic;

    if (name == "existential")
        return Personality::Existential;

    return Personality::Dramatic;    
}

std::string personalityName(Personality personality)
{
    switch (personality)
    {
        case Personality::Dramatic:
            return "Dramatic";

        case Personality::Corporate:
            return "Corporate";

        case Personality::Emo:
            return "Emo";

        case Personality::Sarcastic:
            return "Sarcastic";

        case Personality::Existential:
            return "Existential";
    }

    return "Unknown";
}

int main(int argc, char* argv[])
{
    std::cout << "============================\n";
    std::cout << "🎭 Drama Pi\n";
    std::cout << "============================\n";

    TemperatureMonitor temperatureMonitor;
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    DiskMonitor diskMonitor;
    UptimeMonitor uptimeMonitor;
    EventDetector eventDetector;
    DramaEngine dramaEngine;
    Personality personality = Personality::Dramatic;

    if(argc>1)
    {
        personality=parsePersonality(argv[1]);
    }

    SystemStatus status;
    status.cpuTemperature = temperatureMonitor.getTemperature();
    status.cpuUsage = cpuMonitor.getCpuUsage();
    status.memoryUsage = memoryMonitor.getMemoryUsage();
    status.diskUsage = diskMonitor.getDiskUsage();
    status.uptimeHours = uptimeMonitor.getUptimeHours();

    DramaEvent event = eventDetector.detect(status);

    std::cout << "Personality: "
          << personalityName(personality)
          << "\n";
          
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
              << " %\n";

    std::cout << "Uptime: "
              << status.uptimeHours
              << " hours\n";

    std::cout << "Drama Pi: "
            << dramaEngine.comment(event, personality)
            << "\n";              

    return 0;
}