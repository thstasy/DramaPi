#pragma once

#include <gpiod.hpp>
#include <memory>


class RGBLed
{
public:

    RGBLed(
        int redPin,
        int greenPin,
        int bluePin
    );


    void off();

    void red();
    void green();
    void blue();

    void yellow();
    void purple();
    void cyan();
    void white();

    void orange();


private:

    int redPin_;
    int greenPin_;
    int bluePin_;


    gpiod::chip chip_;

    std::shared_ptr<gpiod::line_request> request_;


    void set(
        bool red,
        bool green,
        bool blue
    );

};