#pragma once

#include <string>


struct Note
{
    std::string pitch;
    float beats;
    float gap = 0.12f;   
};