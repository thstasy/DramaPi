#pragma once

#include <string>
#include "Mood.h"

class DramaEngine
{
public:
    std::string comment(Mood mood) const;
};