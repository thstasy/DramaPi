#include "MemoryMonitor.h"

#include <fstream>
#include <string>

double MemoryMonitor::getMemoryUsage() const
{
    std::ifstream file("/proc/meminfo");

    std::string key;
    long value;
    std::string unit;

    long memTotal=0;
    long memAvailable=0;

    while(file>>key>>value>>unit)
    {
        if(key=="MemTotal:")
        {
            memTotal=value;
        }
        else if(key=="MemAvailable:")
        {
            memAvailable=value;
        }

        if(memTotal > 0 && memAvailable > 0)
        {
            break;
        }
    }

    if(memTotal == 0)
    {
        return 0.0;
    }

    long used = memTotal - memAvailable;
    return 100.0 * static_cast<double>(used)/memTotal;

}