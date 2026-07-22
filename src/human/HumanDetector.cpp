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
