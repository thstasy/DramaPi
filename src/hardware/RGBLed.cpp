#include "RGBLed.h"


RGBLed::RGBLed(
    int redPin,
    int greenPin,
    int bluePin
)
:
redPin_(redPin),
greenPin_(greenPin),
bluePin_(bluePin),
chip_("/dev/gpiochip0")
{

    gpiod::line_settings settings;

    settings.set_direction(
        gpiod::line::direction::OUTPUT
    );


    request_ =
        std::make_shared<gpiod::line_request>(
            chip_
                .prepare_request()
                .add_line_settings(
                    {
                        redPin_,
                        greenPin_,
                        bluePin_
                    },
                    settings
                )
                .do_request()
        );


    off();

}



void RGBLed::set(
    bool red,
    bool green,
    bool blue
)
{

    request_->set_value(
        redPin_,
        red
            ? gpiod::line::value::ACTIVE
            : gpiod::line::value::INACTIVE
    );


    request_->set_value(
        greenPin_,
        green
            ? gpiod::line::value::ACTIVE
            : gpiod::line::value::INACTIVE
    );


    request_->set_value(
        bluePin_,
        blue
            ? gpiod::line::value::ACTIVE
            : gpiod::line::value::INACTIVE
    );

}



void RGBLed::off()
{
    set(false,false,false);
}



void RGBLed::red()
{
    set(true,false,false);
}



void RGBLed::green()
{
    set(false,true,false);
}



void RGBLed::blue()
{
    set(false,false,true);
}



void RGBLed::yellow()
{
    // red + green
    set(true,true,false);
}



void RGBLed::purple()
{
    // red + blue
    set(true,false,true);
}



void RGBLed::cyan()
{
    // green + blue
    set(false,true,true);
}



void RGBLed::white()
{
    set(true,true,true);
}



void RGBLed::orange()
{
    // red + green
    set(true,true,false);
}