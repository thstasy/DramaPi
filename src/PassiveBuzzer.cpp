#include "PassiveBuzzer.h"

#include <lgpio.h>

#include <chrono>
#include <cmath>
#include <stdexcept>
#include <thread>
#include <unordered_map>


PassiveBuzzer::PassiveBuzzer(int gpio)
:
gpio_(gpio),
chipHandle_(-1)
{
    chipHandle_ = lgGpiochipOpen(0);

    if(chipHandle_ < 0)
    {
        throw std::runtime_error(
            "Failed to open gpiochip0"
        );
    }

    const int result =
        lgGpioClaimOutput(
            chipHandle_,
            0,
            gpio_,
            0
        );

    if(result < 0)
    {
        lgGpiochipClose(chipHandle_);

        throw std::runtime_error(
            "Failed to claim passive buzzer GPIO"
        );
    }
}


PassiveBuzzer::~PassiveBuzzer()
{
    stop();

    if(chipHandle_ >= 0)
    {
        lgGpioFree(
            chipHandle_,
            gpio_
        );

        lgGpiochipClose(
            chipHandle_
        );
    }
}


void PassiveBuzzer::playTone(
    double frequency,
    int durationMs
)
{
    if(durationMs <= 0)
    {
        return;
    }

    if(frequency <= 0.0)
    {
        stop();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(durationMs)
        );

        return;
    }

    const int result =
        lgTxPwm(
            chipHandle_,
            gpio_,
            static_cast<float>(frequency),
            50.0f,
            0.0f,
            0
        );

    if(result < 0)
    {
        throw std::runtime_error(
            "lgTxPwm failed: "
            + std::to_string(result)
        );
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(durationMs)
    );

    stop();
}


void PassiveBuzzer::stop()
{
    if(chipHandle_ < 0)
    {
        return;
    }

    lgTxPwm(
        chipHandle_,
        gpio_,
        2000.0f,
        0.0f,
        0.0f,
        0
    );

    lgGpioWrite(
        chipHandle_,
        gpio_,
        0
    );
}


double PassiveBuzzer::frequencyFor(
    const std::string& pitch,
    int octaveShift
) const
{
    if(pitch == "R")
    {
        return 0.0;
    }

    static const std::unordered_map<
        std::string,
        double
    > frequencies =
    {
        {"C4", 261.63},
        {"D4", 293.66},
        {"E4", 329.63},
        {"F4", 349.23},
        {"G4", 392.00},
        {"A4", 440.00},
        {"B4", 493.88},

        {"C5", 523.25},
        {"D5", 587.33},
        {"E5", 659.25},
        {"F5", 698.46},
        {"G5", 783.99},
        {"A5", 880.00},
        {"B5", 987.77}
    };

    const auto it =
        frequencies.find(pitch);

    if(it == frequencies.end())
    {
        throw std::invalid_argument(
            "Unknown note: " + pitch
        );
    }

    return it->second *
        std::pow(
            2.0,
            octaveShift
        );
}


void PassiveBuzzer::playMelody(
    const std::vector<Note>& melody,
    int bpm,
    int octaveShift
)
{
    if(bpm <= 0)
    {
        throw std::invalid_argument(
            "BPM must be greater than zero"
        );
    }

    const double beatMs =
        60000.0 / bpm;

    for(const auto& note : melody)
    {
        const int totalMs =
            static_cast<int>(
                beatMs * note.beats
            );

        const int soundMs =
            static_cast<int>(
                totalMs * 0.88
            );

        playTone(
            frequencyFor(
                note.pitch,
                octaveShift
            ),
            soundMs
        );

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                totalMs - soundMs
            )
        );
    }

    stop();
}