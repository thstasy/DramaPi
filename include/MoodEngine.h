#pragma once
#include "Mood.h"
#include "SystemStatus.h"

class MoodEngine
{
public:
    Mood evaluate(const SystemStatus& status) const;
};