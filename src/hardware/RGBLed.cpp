#include "RGBLed.h"

#include <lgpio.h>

#include <algorithm>
#include <stdexcept>
#include <string>


RGBLed::RGBLed(
    int redGpio,
    int greenGpio,
    int blueGpio
)
:
redGpio_(redGpio),
greenGpio_(greenGpio),
blueGpio_(blueGpio),
chipHandle_(-1),
initialized_(false)
{
}


RGBLed::~RGBLed()
{
    if(chipHandle_ < 0)
    {
        return;
    }

    off();

    lgGpioFree(
        chipHandle_,
        redGpio_
    );

    lgGpioFree(
        chipHandle_,
        greenGpio_
    );

    lgGpioFree(
        chipHandle_,
        blueGpio_
    );

    lgGpiochipClose(
        chipHandle_
    );

    chipHandle_ = -1;
}


void RGBLed::init()
{
    if(initialized_)
    {
        return;
    }

    chipHandle_ =
        lgGpiochipOpen(0);

    if(chipHandle_ < 0)
    {
        throw std::runtime_error(
            "Failed to open gpiochip0 for RGB LED"
        );
    }

    const int redResult =
        lgGpioClaimOutput(
            chipHandle_,
            0,
            redGpio_,
            0
        );

    const int greenResult =
        lgGpioClaimOutput(
            chipHandle_,
            0,
            greenGpio_,
            0
        );

    const int blueResult =
        lgGpioClaimOutput(
            chipHandle_,
            0,
            blueGpio_,
            0
        );

    if(
        redResult < 0
        || greenResult < 0
        || blueResult < 0
    )
    {
        throw std::runtime_error(
            "Failed to claim RGB LED GPIOs"
        );
    }

    initialized_ =
        true;

    off();
}


void RGBLed::off()
{
    setBrightness(
        0,
        0,
        0
    );
}


void RGBLed::red()
{
    setBrightness(
        100,
        0,
        0
    );
}


void RGBLed::green()
{
    setBrightness(
        0,
        100,
        0
    );
}


void RGBLed::blue()
{
    setBrightness(
        0,
        0,
        100
    );
}


void RGBLed::yellow()
{
    setBrightness(
        100,
        100,
        0
    );
}


void RGBLed::purple()
{
    setBrightness(
        100,
        0,
        100
    );
}


void RGBLed::cyan()
{
    setBrightness(
        0,
        100,
        100
    );
}


void RGBLed::white()
{
    setBrightness(
        100,
        100,
        100
    );
}


void RGBLed::setBrightness(
    int redPercent,
    int greenPercent,
    int bluePercent
)
{
    if(!initialized_)
    {
        return;
    }

    setChannelBrightness(
        redGpio_,
        redPercent
    );

    setChannelBrightness(
        greenGpio_,
        greenPercent
    );

    setChannelBrightness(
        blueGpio_,
        bluePercent
    );
}


void RGBLed::set(
    bool redOn,
    bool greenOn,
    bool blueOn
)
{
    setBrightness(
        redOn ? 100 : 0,
        greenOn ? 100 : 0,
        blueOn ? 100 : 0
    );
}


void RGBLed::setChannelBrightness(
    int gpio,
    int percent
)
{
    percent =
        std::clamp(
            percent,
            0,
            100
        );

    const int result =
        lgTxPwm(
            chipHandle_,
            gpio,
            PwmFrequencyHz,
            static_cast<float>(
                percent
            ),
            0,
            0
        );

    if(result < 0)
    {
        throw std::runtime_error(
            "lgTxPwm failed on RGB GPIO "
            + std::to_string(gpio)
            + ": "
            + std::to_string(result)
        );
    }
}
