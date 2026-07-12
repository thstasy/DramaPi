#include "Buzzer.h"

#include <thread>
#include <chrono>


Buzzer::Buzzer(int gpio)
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
        .set_consumer("DramaPi-Buzzer")
        .add_line_settings(
            gpio_,
            settings
        )
        .do_request()
    );


    off();

}



Buzzer::~Buzzer()
{
    off();
}



void Buzzer::on()
{

    request_->set_value(
        gpio_,
        gpiod::line::value::ACTIVE
    );

}



void Buzzer::off()
{

    request_->set_value(
        gpio_,
        gpiod::line::value::INACTIVE
    );

}



void Buzzer::beep(int milliseconds)
{

    on();


    std::this_thread::sleep_for(
        std::chrono::milliseconds(milliseconds)
    );


    off();

}