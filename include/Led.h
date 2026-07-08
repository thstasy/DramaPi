#pragma once

#include <string>

class Led
{
public:
    explicit Led(int gpio);

    void on();
    void off();

private:
    int gpio_;

    void run(const std::string& cmd);
};
