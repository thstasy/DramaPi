// #pragma once

// #include "HumanState.h"

// class HumanDetector
// {
// public:

//     HumanState detect(float distance) const;

// };

#pragma once

#include "HumanState.h"


class HumanDetector
{
public:
    [[nodiscard]]
    HumanState detect(
        float distance
    ) const;


private:
    static constexpr float AloneDistanceCm = 100.0f;

    static constexpr float NearbyDistanceCm = 70.0f;

    static constexpr float ApproachingDistanceCm = 40.0f;

    static constexpr float CloseDistanceCm = 25.0f;

    static constexpr float PersonalSpaceDistanceCm = 15.0f;
};
