#pragma once

#include "Led.h"
#include "StatusLightMode.h"


class StatusLights
{
public:
    StatusLights(
        Led& redLed,
        Led& greenLed
    );


    void setMode(
        StatusLightMode mode
    );


    [[nodiscard]]
    StatusLightMode mode() const;


private:
    Led& redLed_;

    Led& greenLed_;

    StatusLightMode mode_;
};
