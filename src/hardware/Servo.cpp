#include "Servo.h"

#include <lgpio.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>


Servo::Servo(
    int gpio
)
:
gpio_(gpio),
chipHandle_(-1),
currentAngle_(HomeAngle),
targetAngle_(HomeAngle),
scanPhase_(ScanPhase::Left),
mode_(ServoMode::Home),
running_(false)
{
}


Servo::~Servo()
{
    running_.store(
        false
    );

    if(controlThread_.joinable())
    {
        controlThread_.join();
    }

    if(chipHandle_ < 0)
    {
        return;
    }

    lgTxServo(
        chipHandle_,
        gpio_,
        0,
        ServoFrequencyHz,
        0,
        0
    );

    lgGpioFree(
        chipHandle_,
        gpio_
    );

    lgGpiochipClose(
        chipHandle_
    );

    chipHandle_ = -1;
}


void Servo::init()
{
    if(running_.load())
    {
        return;
    }

    chipHandle_ =
        lgGpiochipOpen(0);

    if(chipHandle_ < 0)
    {
        throw std::runtime_error(
            "Failed to open gpiochip0: "
            + std::to_string(chipHandle_)
        );
    }

    const int claimResult =
        lgGpioClaimOutput(
            chipHandle_,
            0,
            gpio_,
            0
        );

    if(claimResult < 0)
    {
        lgGpiochipClose(
            chipHandle_
        );

        chipHandle_ = -1;

        throw std::runtime_error(
            "Failed to claim servo GPIO "
            + std::to_string(gpio_)
            + ": "
            + std::to_string(claimResult)
        );
    }

    currentAngle_ = HomeAngle;
    targetAngle_ = HomeAngle;
    scanPhase_ = ScanPhase::Left;

    mode_.store(
        ServoMode::Home
    );

    setAngle(
        HomeAngle
    );

    running_.store(
        true
    );

    controlThread_ =
        std::thread(
            &Servo::controlLoop,
            this
        );
}


void Servo::setMode(
    ServoMode mode
)
{
    mode_.store(
        mode
    );
}


void Servo::home()
{
    setMode(
        ServoMode::Home
    );
}


void Servo::startScanning()
{
    setMode(
        ServoMode::Scanning
    );
}


void Servo::stopScanning()
{
    home();
}


void Servo::startEmergencyShake()
{
    setMode(
        ServoMode::Emergency
    );
}


ServoMode Servo::mode() const
{
    return mode_.load();
}


bool Servo::isRunning() const
{
    return running_.load();
}


void Servo::controlLoop()
{
    ServoMode previousMode =
        ServoMode::Home;

    while(running_.load())
    {
        const ServoMode currentMode =
            mode_.load();

        if(currentMode != previousMode)
        {
            applyModeTransition(
                previousMode,
                currentMode
            );

            previousMode =
                currentMode;
        }

        switch(currentMode)
        {
            case ServoMode::Home:
                updateHome();
                break;

            case ServoMode::Scanning:
                updateScanning();
                break;

            case ServoMode::Emergency:
                updateEmergency();
                break;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                ControlIntervalMs
            )
        );
    }
}


void Servo::applyModeTransition(
    ServoMode,
    ServoMode currentMode
)
{
    switch(currentMode)
    {
        case ServoMode::Home:

            targetAngle_ =
                HomeAngle;

            break;


        case ServoMode::Scanning:

            scanPhase_ =
                ScanPhase::Left;

            chooseTargetForCurrentPhase();

            break;


        case ServoMode::Emergency:

            targetAngle_ =
                EmergencyLeftAngle;

            break;
    }
}


void Servo::updateHome()
{
    targetAngle_ =
        HomeAngle;

    if(currentAngle_ != targetAngle_)
    {
        moveTowardTarget(
            NormalStepDegrees
        );
    }
}


void Servo::updateScanning()
{
    if(currentAngle_ != targetAngle_)
    {
        moveTowardTarget(
            NormalStepDegrees
        );

        return;
    }

    advanceScanPhase();
}


void Servo::updateEmergency()
{
    if(currentAngle_ != targetAngle_)
    {
        moveTowardTarget(
            EmergencyStepDegrees
        );

        return;
    }

    targetAngle_ =
        targetAngle_ == EmergencyLeftAngle
        ? EmergencyRightAngle
        : EmergencyLeftAngle;
}


void Servo::setAngle(
    int angle
)
{
    if(chipHandle_ < 0)
    {
        throw std::runtime_error(
            "Servo has not been initialized"
        );
    }

    angle =
        std::clamp(
            angle,
            0,
            180
        );

    const int pulseWidthUs =
        angleToPulseWidth(
            angle
        );

    const int result =
        lgTxServo(
            chipHandle_,
            gpio_,
            pulseWidthUs,
            ServoFrequencyHz,
            0,
            0
        );

    if(result < 0)
    {
        throw std::runtime_error(
            "lgTxServo failed on GPIO "
            + std::to_string(gpio_)
            + ": "
            + std::to_string(result)
        );
    }
}


int Servo::angleToPulseWidth(
    int angle
) const
{
    angle =
        std::clamp(
            angle,
            0,
            180
        );

    if(angle <= HomeAngle)
    {
        return MinimumPulseWidthUs
            +
            (
                angle
                * (
                    CenterPulseWidthUs
                    - MinimumPulseWidthUs
                )
                / HomeAngle
            );
    }

    return CenterPulseWidthUs
        +
        (
            (angle - HomeAngle)
            * (
                MaximumPulseWidthUs
                - CenterPulseWidthUs
            )
            / (
                180 - HomeAngle
            )
        );
}


int Servo::randomAngle(
    int minimum,
    int maximum
)
{
    static std::random_device randomDevice;

    static std::mt19937 generator(
        randomDevice()
    );

    std::uniform_int_distribution<int> distribution(
        minimum,
        maximum
    );

    return distribution(
        generator
    );
}


void Servo::chooseTargetForCurrentPhase()
{
    switch(scanPhase_)
    {
        case ScanPhase::Left:

            targetAngle_ =
                randomAngle(
                    LeftMinimum,
                    LeftMaximum
                );

            break;


        case ScanPhase::MiddleAfterLeft:

            targetAngle_ =
                randomAngle(
                    MiddleMinimum,
                    MiddleMaximum
                );

            break;


        case ScanPhase::Right:

            targetAngle_ =
                randomAngle(
                    RightMinimum,
                    RightMaximum
                );

            break;


        case ScanPhase::MiddleAfterRight:

            targetAngle_ =
                randomAngle(
                    MiddleMinimum,
                    MiddleMaximum
                );

            break;
    }
}


void Servo::advanceScanPhase()
{
    switch(scanPhase_)
    {
        case ScanPhase::Left:
            scanPhase_ =
                ScanPhase::MiddleAfterLeft;
            break;

        case ScanPhase::MiddleAfterLeft:
            scanPhase_ =
                ScanPhase::Right;
            break;

        case ScanPhase::Right:
            scanPhase_ =
                ScanPhase::MiddleAfterRight;
            break;

        case ScanPhase::MiddleAfterRight:
            scanPhase_ =
                ScanPhase::Left;
            break;
    }

    chooseTargetForCurrentPhase();
}


void Servo::moveTowardTarget(
    int stepDegrees
)
{
    if(currentAngle_ < targetAngle_)
    {
        currentAngle_ =
            std::min(
                currentAngle_ + stepDegrees,
                targetAngle_
            );
    }
    else if(currentAngle_ > targetAngle_)
    {
        currentAngle_ =
            std::max(
                currentAngle_ - stepDegrees,
                targetAngle_
            );
    }

    setAngle(
        currentAngle_
    );
}
