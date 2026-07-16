#pragma once

#include <gpiod.hpp>
#include <optional>


class Buzzer
{

public:

    explicit Buzzer(int gpio);

    ~Buzzer();


    void on();

    void off();

    void alarm(int milliseconds, int volume);

    void beep(int milliseconds);


private:

    int gpio_;

    gpiod::chip chip_;

    std::optional<gpiod::line_request> request_;

};