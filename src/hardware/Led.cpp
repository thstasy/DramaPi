#include "Led.h"


Led::Led(int gpio)
:
gpio_(gpio),
chip_("/dev/gpiochip0")
{

    gpiod::line_settings settings;


    settings.set_direction(
        gpiod::line::direction::OUTPUT
    );


    request_.emplace(
        chip_
        .prepare_request()
        .set_consumer("DramaPi-LED")
        .add_line_settings(
            gpio_,
            settings
        )
        .do_request()
    );


    off();

}



Led::~Led()
{
    off();
}



void Led::on()
{

    request_->set_value(
        gpio_,
        gpiod::line::value::ACTIVE
    );

}



void Led::off()
{

    request_->set_value(
        gpio_,
        gpiod::line::value::INACTIVE
    );

}