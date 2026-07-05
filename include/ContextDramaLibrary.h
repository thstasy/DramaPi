#pragma once

#include "Personality.h"
#include "SystemStatus.h"

#include <string>

class ContextDramaLibrary
{
public:
    std::string cpuUsageLine(const SystemStatus& status, Personality personality) const;
    std::string memoryLine(const SystemStatus& status, Personality personality) const;
    std::string diskLine(const SystemStatus& status, Personality personality) const;
    std::string uptimeLine(const SystemStatus& status, Personality personality) const;
};