#include "StatusLights.h"


StatusLights::StatusLights(
    Led& redLed,
    Led& greenLed
)
:
redLed_(redLed),
greenLed_(greenLed),
mode_(StatusLightMode::Safe)
{
    setMode(
        StatusLightMode::Safe
    );
}


void StatusLights::setMode(
    StatusLightMode mode
)
{
    mode_ = mode;

    switch(mode_)
    {
        case StatusLightMode::Safe:

            redLed_.off();
            greenLed_.on();

            break;


        case StatusLightMode::Warning:

            redLed_.off();
            greenLed_.on();

            break;


        case StatusLightMode::Danger:

            greenLed_.off();
            redLed_.on();

            break;


        case StatusLightMode::Emergency:

            greenLed_.off();
            redLed_.on();

            break;
    }
}


StatusLightMode StatusLights::mode() const
{
    return mode_;
}
