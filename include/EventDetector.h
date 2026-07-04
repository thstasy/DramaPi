#pragma once

#include "DramaEvent.h"
#include "SystemStatus.h"

class EventDetector
{
public:
    DramaEvent detect(const SystemStatus& status) const;
};