#include "HumanInteraction.h"


HumanInteraction::HumanInteraction()
:
tooClose_(false),
alarm_(false)
{

}


void HumanInteraction::update(HumanState state)
{

    auto now =
        std::chrono::steady_clock::now();


    if(state == HumanState::PersonalSpace ||
       state == HumanState::Emergency)
    {

        if(!tooClose_)
        {
            tooClose_ = true;

            closeStart_ = now;

            alarm_ = false;
        }


        auto seconds =
            std::chrono::duration_cast<
                std::chrono::seconds
            >
            (
                now - closeStart_
            )
            .count();


        if(seconds >= 10)
        {
            alarm_ = true;
        }

    }
    else
    {
        // 人離開，重置
        tooClose_ = false;
        alarm_ = false;
    }

}



bool HumanInteraction::shouldAlarm() const
{
    return alarm_;
}



int HumanInteraction::closeDuration() const
{
    if(!tooClose_)
        return 0;


    auto now =
        std::chrono::steady_clock::now();


    return std::chrono::duration_cast<
        std::chrono::seconds
    >
    (
        now - closeStart_
    )
    .count();
}