#pragma once

#include <string>


struct Note
{
    std::string pitch;
    float beats;
    float gap = 0.12f;   
};

// #pragma once

// #include <string>

// struct Note
// {
//     std::string pitch;
//     float beats;

//     float volume = 1.0f;   // 0.0 ~ 1.0
//     bool legato = false;   // 是否不要停頓
// };