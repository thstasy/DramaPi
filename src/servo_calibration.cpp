#include <lgpio.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    constexpr int gpio = 18;
    constexpr int frequency = 50;

    const int chipHandle =
        lgGpiochipOpen(0);

    if(chipHandle < 0)
    {
        throw std::runtime_error(
            "Failed to open gpiochip0"
        );
    }

    const int claimResult =
        lgGpioClaimOutput(
            chipHandle,
            0,
            gpio,
            0
        );

    if(claimResult < 0)
    {
        lgGpiochipClose(
            chipHandle
        );

        throw std::runtime_error(
            "Failed to claim GPIO18"
        );
    }

    const auto moveTo =
        [chipHandle, gpio, frequency](
            int pulseWidthUs,
            const char* label
        )
        {
            std::cout
                << label
                << ": "
                << pulseWidthUs
                << " us\n";

            const int result =
                lgTxServo(
                    chipHandle,
                    gpio,
                    pulseWidthUs,
                    frequency,
                    0,
                    0
                );

            if(result < 0)
            {
                throw std::runtime_error(
                    "lgTxServo failed"
                );
            }

            std::this_thread::sleep_for(
                2s
            );
        };

    moveTo(
        1800,
        "Center"
    );

    moveTo(
        1600,
        "Side A"
    );

    moveTo(
        1800,
        "Center"
    );

    moveTo(
        2000,
        "Side B"
    );

    moveTo(
        1800,
        "Center"
    );

    lgTxServo(
        chipHandle,
        gpio,
        0,
        frequency,
        0,
        0
    );

    lgGpioFree(
        chipHandle,
        gpio
    );

    lgGpiochipClose(
        chipHandle
    );

    return 0;
}