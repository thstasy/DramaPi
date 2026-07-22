#include "CpuMonitor.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace
{
    struct CpuTimes
    {
        long user;
        long nice;
        long system;
        long idle;
        long iowait;
        long irq;
        long softirq;
        long steal;
    };

    CpuTimes readCpuTimes()
    {
        std::ifstream file("/proc/stat");
        std::string cpuLabel;

        CpuTimes times{};
        file >> cpuLabel
             >> times.user
             >> times.nice
             >> times.system
             >> times.idle
             >> times.iowait
             >> times.irq
             >> times.softirq
             >> times.steal;

        return times;
    }

    long idleTime(const CpuTimes& t)
    {
        return t.idle + t.iowait;
    }

    long totalTime(const CpuTimes& t)
    {
        return t.user + t.nice + t.system + t.idle
             + t.iowait + t.irq + t.softirq + t.steal;
    }
}

double CpuMonitor::getCpuUsage() const
{
    CpuTimes first = readCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CpuTimes second = readCpuTimes();

    long totalDelta = totalTime(second) - totalTime(first);
    long idleDelta = idleTime(second) - idleTime(first);

    if (totalDelta == 0)
    {
        return 0.0;
    }

    return 100.0 * (1.0 - static_cast<double>(idleDelta) / totalDelta);
}
