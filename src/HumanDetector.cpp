// #include "HumanDetector.h"


// HumanState HumanDetector::detect(
//     float distance
// ) const
// {
//     if(distance > 100.0f)
//     {
//         return HumanState::Alone;
//     }

//     if(distance > 70.0f)
//     {
//         return HumanState::Nearby;
//     }

//     if(distance > 40.0f)
//     {
//         return HumanState::Approaching;
//     }

//     if(distance > 25.0f)
//     {
//         return HumanState::Close;
//     }

//     if(distance > 15.0f)
//     {
//         return HumanState::PersonalSpace;
//     }

//     return HumanState::Emergency;
// }

#include "HumanDetector.h"


HumanState HumanDetector::detect(
    float distance
) const
{
    if(distance > AloneDistanceCm)
    {
        return HumanState::Alone;
    }

    if(distance > NearbyDistanceCm)
    {
        return HumanState::Nearby;
    }

    if(distance > ApproachingDistanceCm)
    {
        return HumanState::Approaching;
    }

    if(distance > CloseDistanceCm)
    {
        return HumanState::Close;
    }

    if(distance > PersonalSpaceDistanceCm)
    {
        return HumanState::PersonalSpace;
    }

    return HumanState::Emergency;
}
