#pragma once

#include "Personality.h"

#include <string>

class ReminderLibrary
{
public:
    std::string reminder(Personality personality) const;
};