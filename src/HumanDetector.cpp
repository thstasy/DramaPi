#include "HumanDetector.h"


HumanState HumanDetector::detect(float distance) const
{
    if(distance > 200)
    {
        return HumanState::Alone;
    }

    if(distance > 150)
    {
        return HumanState::Nearby;
    }

    if(distance > 100)
    {
        return HumanState::Approaching;
    }

    if(distance > 50)
    {
        return HumanState::Close;
    }

    if(distance > 30)
    {
        return HumanState::PersonalSpace;
    }

    return HumanState::Emergency;
}