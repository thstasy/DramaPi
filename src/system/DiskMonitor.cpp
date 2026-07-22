#include "DiskMonitor.h"
#include <sys/statvfs.h>

double DiskMonitor::getDiskUsage() const
{
    struct statvfs stat {};
    if (statvfs("/", &stat) != 0)
    {
        return 0.0;
    }

    unsigned long total =stat.f_blocks;
    unsigned long available =stat.f_bavail;

    if(total == 0){
        return 0.0;
    }

    unsigned long used = total-available;

    return 100.0 * static_cast<double>(used)/total;
    
}
