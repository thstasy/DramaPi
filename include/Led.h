// #pragma once

// #include <string>

// class Led
// {
// public:
//     explicit Led(int gpio);

//     void on();
//     void off();

// private:
//     int gpio_;

//     void run(const std::string& cmd);
// };
#pragma once

#include <gpiod.hpp>
#include <optional>


class Led
{

public:

    explicit Led(int gpio);

    ~Led();


    void on();

    void off();


private:

    int gpio_;

    gpiod::chip chip_;

    std::optional<gpiod::line_request> request_;

};