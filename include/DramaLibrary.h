#pragma once

#include "DramaEvent.h"
#include "Personality.h"

#include <string>
#include <vector>

class DramaLibrary
{
public:
    std::vector<std::string> quotesFor(
        DramaEvent event,
        Personality personality
    ) const;
};