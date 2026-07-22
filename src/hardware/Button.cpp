#include "Button.h"

#include <lgpio.h>

#include <stdexcept>
#include <string>


Button::Button(
    int gpio,
    bool activeLow
)
:
gpio_(gpio),
chipHandle_(-1),
activeLow_(activeLow),
initialized_(false),
stablePressed_(false),
lastRawPressed_(false),
lastRawChange_(
    std::chrono::steady_clock::now()
)
{
}


Button::~Button()
{
    if(chipHandle_ < 0)
    {
        return;
    }

    lgGpioFree(
        chipHandle_,
        gpio_
    );

    lgGpiochipClose(
        chipHandle_
    );

    chipHandle_ = -1;
}


void Button::init()
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
            "Failed to open gpiochip0 for button GPIO "
            + std::to_string(gpio_)
        );
    }

    const int flags =
        activeLow_
        ? LG_SET_PULL_UP
        : LG_SET_PULL_DOWN;

    const int result =
        lgGpioClaimInput(
            chipHandle_,
            flags,
            gpio_
        );

    if(result < 0)
    {
        lgGpiochipClose(
            chipHandle_
        );

        chipHandle_ = -1;

        throw std::runtime_error(
            "Failed to claim button GPIO "
            + std::to_string(gpio_)
            + ": "
            + std::to_string(result)
        );
    }

    stablePressed_ =
        readRawPressed();

    lastRawPressed_ =
        stablePressed_;

    lastRawChange_ =
        std::chrono::steady_clock::now();

    initialized_ =
        true;
}


void Button::update()
{
    if(!initialized_)
    {
        throw std::runtime_error(
            "Button has not been initialized"
        );
    }

    /*
     * 直接更新目前按鈕狀態。
     *
     * 先不做 debounce，
     * 避免短按在主迴圈中被漏掉。
     */
    stablePressed_ =
        readRawPressed();
}


bool Button::isPressed() const
{
    return stablePressed_;
}


bool Button::readRawPressed() const
{
    const int value =
        lgGpioRead(
            chipHandle_,
            gpio_
        );

    if(value < 0)
    {
        throw std::runtime_error(
            "Failed to read button GPIO "
            + std::to_string(gpio_)
        );
    }

    /*
     * activeLow = true：
     * GPIO LOW 代表按下。
     *
     * activeLow = false：
     * GPIO HIGH 代表按下。
     */
    return activeLow_
        ? value == 0
        : value != 0;
}