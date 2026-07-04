#pragma once

#include <string>
#include "SystemStatus.h"

class DramaEngine
{
public:
    std::string comment(const SystemStatus& status) const;
};