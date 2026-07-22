#include "EventDetector.h"

DramaEvent EventDetector::detect(const SystemStatus& status) const
{
    if (status.cpuTemperature > 75.0) {
        return DramaEvent::Overheated;
    }

    if (status.memoryUsage > 85.0) {
        return DramaEvent::MemoryHigh;
    }

    if (status.diskUsage > 90.0) {
        return DramaEvent::DiskFull;
    }

    if (status.uptimeHours > 24.0) {
        return DramaEvent::LongUptime;
    }

    if (status.cpuUsage > 80.0) {
        return DramaEvent::CpuBusy;
    }

    // if (status.cpuUsage < 1.0) {
    //     return DramaEvent::CpuIdle;
    // }

    return DramaEvent::Normal;
}
