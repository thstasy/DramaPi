#pragma once

#include <chrono>

#include "HumanState.h"


class HumanInteraction
{
public:

    HumanInteraction();

    void update(HumanState state);

    bool shouldAlarm() const;

    int closeDuration() const;


private:

    bool tooClose_;

    std::chrono::steady_clock::time_point closeStart_;

    bool alarm_;

};