#include "Led.h"

#include <cstdlib>

Led::Led(int gpio)
    : gpio_(gpio)
{
}

void Led::run(const std::string& cmd)
{
    std::system(cmd.c_str());
}

void Led::on()
{
    run("sudo pinctrl set "
        + std::to_string(gpio_)
        + " op dh");
}

void Led::off()
{
    run("sudo pinctrl set "
        + std::to_string(gpio_)
        + " op dl");
}
