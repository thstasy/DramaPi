#include "UptimeMonitor.h"

#include <fstream>

double UptimeMonitor::getUptimeHours() const
{
    std::ifstream file("/proc/uptime");

    double uptimeSeconds = 0.0;
    file >> uptimeSeconds;

    return uptimeSeconds / 3600.0;
}