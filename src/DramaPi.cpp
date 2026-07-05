#include "DramaPi.h"
#include "ContextDramaLibrary.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <iomanip>

#include "CpuMonitor.h"
#include "DiskMonitor.h"
#include "DramaEngine.h"
#include "EventDetector.h"
#include "MemoryMonitor.h"
#include "Personality.h"
#include "ReminderLibrary.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"
#include "UptimeMonitor.h"

namespace
{
    Personality parsePersonality(const std::string& name)
    {
        if (name == "dramatic") return Personality::Dramatic;
        if (name == "corporate") return Personality::Corporate;
        if (name == "emo") return Personality::Emo;
        if (name == "sarcastic") return Personality::Sarcastic;
        if (name == "existential") return Personality::Existential;

        return Personality::Dramatic;
    }

    std::string personalityName(Personality personality)
    {
        switch (personality)
        {
            case Personality::Dramatic: return "Dramatic";
            case Personality::Corporate: return "Corporate";
            case Personality::Emo: return "Emo";
            case Personality::Sarcastic: return "Sarcastic";
            case Personality::Existential: return "Existential";
        }

        return "Unknown";
    }
}


DramaPi::DramaPi(int argc, char* argv[])
    : personality(Personality::Dramatic),
      reminderMode(false)
{
    if (argc > 1)
    {
        personality = parsePersonality(argv[1]);
    }

    if (argc > 2 && std::string(argv[2]) == "reminder")
    {
        reminderMode = true;
    }
}

void DramaPi::run()
{
    if (reminderMode)
    {
        runReminderLoop();
    }
    else
    {
        runStatusOnce();
    }
}

void DramaPi::runReminderLoop()
{
    ReminderLibrary reminderLibrary;

    std::cout << "🎭 Drama Pi Reminder started\n";
    std::cout << "Personality: "
              << personalityName(personality)
              << "\n";
    std::cout << "Reminder interval: 10 minutes\n\n";

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::minutes(10));

        std::cout << "⏰ Reminder: "
                  << reminderLibrary.reminder(personality)
                  << "\n\n"
                  << std::flush;
    }
}


void DramaPi::runStatusOnce()
{
    TemperatureMonitor temperatureMonitor;
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    DiskMonitor diskMonitor;
    UptimeMonitor uptimeMonitor;
    EventDetector eventDetector;
    DramaEngine dramaEngine;
    ContextDramaLibrary contextDrama;
    SystemStatus status;

    status.cpuTemperature = temperatureMonitor.getTemperature();
    status.cpuUsage = cpuMonitor.getCpuUsage();
    status.memoryUsage = memoryMonitor.getMemoryUsage();
    status.diskUsage = diskMonitor.getDiskUsage();
    status.uptimeHours = uptimeMonitor.getUptimeHours();

    DramaEvent event = eventDetector.detect(status);

    std::cout << "============================\n";
    std::cout << "🎭 Drama Pi\n";
    std::cout << "============================\n";

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "Personality: "
            << personalityName(personality)
            << "\n\n";

    std::cout << "🌡 CPU Temp: "
            << status.cpuTemperature
            << " °C\n\n";

    std::cout << "⚙ CPU Usage: "
              << std::fixed << std::setprecision(3)
              << status.cpuUsage
              << " %\n";

    std::cout << "   "
            << contextDrama.cpuUsageLine(status, personality)
            << "\n\n";

    std::cout << "🧠 Memory Usage: "
            << status.memoryUsage
            << " %\n";
    std::cout << "   "
            << contextDrama.memoryLine(status, personality)
            << "\n\n";

    std::cout << "💾 Disk Usage: "
            << status.diskUsage
            << " %\n";
    std::cout << "   "
            << contextDrama.diskLine(status, personality)
            << "\n\n";

    std::cout << "⏳ Uptime: "
            << status.uptimeHours
            << " hours\n";
    std::cout << "   "
            << contextDrama.uptimeLine(status, personality)
            << "\n\n";

    std::cout << "🎭 Today's Drama\n";
    std::cout << dramaEngine.comment(event, personality)
            << "\n";

}