#include <lgpio.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;

namespace
{
constexpr int Gpio5 = 5;
constexpr int Gpio6 = 6;
constexpr int Gpio12 = 12;

void setChannels(
    int chipHandle,
    int gpio5,
    int gpio6,
    int gpio12
)
{
    lgGpioWrite(
        chipHandle,
        Gpio5,
        gpio5
    );

    lgGpioWrite(
        chipHandle,
        Gpio6,
        gpio6
    );

    lgGpioWrite(
        chipHandle,
        Gpio12,
        gpio12
    );
}

void showCombination(
    int chipHandle,
    int gpio5,
    int gpio6,
    int gpio12
)
{
    std::cout
        << "GPIO5="
        << gpio5
        << " GPIO6="
        << gpio6
        << " GPIO12="
        << gpio12
        << '\n';

    setChannels(
        chipHandle,
        gpio5,
        gpio6,
        gpio12
    );

    std::this_thread::sleep_for(
        3s
    );
}
}

int main()
{
    const int chipHandle =
        lgGpiochipOpen(0);

    if(chipHandle < 0)
    {
        throw std::runtime_error(
            "Failed to open gpiochip0"
        );
    }

    if(
        lgGpioClaimOutput(
            chipHandle,
            0,
            Gpio5,
            0
        ) < 0
        ||
        lgGpioClaimOutput(
            chipHandle,
            0,
            Gpio6,
            0
        ) < 0
        ||
        lgGpioClaimOutput(
            chipHandle,
            0,
            Gpio12,
            0
        ) < 0
    )
    {
        throw std::runtime_error(
            "Failed to claim RGB GPIOs"
        );
    }

    showCombination(
        chipHandle,
        0,
        0,
        0
    );

    showCombination(
        chipHandle,
        0,
        0,
        1
    );

    showCombination(
        chipHandle,
        0,
        1,
        0
    );

    showCombination(
        chipHandle,
        0,
        1,
        1
    );

    showCombination(
        chipHandle,
        1,
        0,
        0
    );

    showCombination(
        chipHandle,
        1,
        0,
        1
    );

    showCombination(
        chipHandle,
        1,
        1,
        0
    );

    showCombination(
        chipHandle,
        1,
        1,
        1
    );

    lgGpioFree(
        chipHandle,
        Gpio5
    );

    lgGpioFree(
        chipHandle,
        Gpio6
    );

    lgGpioFree(
        chipHandle,
        Gpio12
    );

    lgGpiochipClose(
        chipHandle
    );

    return 0;
}