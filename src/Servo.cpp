#include "Servo.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <stdexcept>
#include <thread>


using namespace std::chrono_literals;


Servo::Servo()
:
currentAngle_(HomeAngle),
targetAngle_(HomeAngle),
scanning_(false),
scanPhase_(ScanPhase::Left)
{
}


bool Servo::exists(
    const std::string& path
) const
{
    return std::filesystem::exists(path);
}


void Servo::writeFile(
    const std::string& path,
    int value
)
{
    std::ofstream file(path);

    if(!file)
    {
        throw std::runtime_error(
            "Failed to open: " + path
        );
    }

    file << value;
    file.flush();

    if(!file)
    {
        throw std::runtime_error(
            "Failed to write: " + path
        );
    }
}


void Servo::init()
{
    if(!exists(PwmPath))
    {
        writeFile(
            PwmChipPath + "/export",
            Channel
        );

        for(
            int attempt = 0;
            attempt < 100 && !exists(PwmPath);
            ++attempt
        )
        {
            std::this_thread::sleep_for(
                10ms
            );
        }

        if(!exists(PwmPath))
        {
            throw std::runtime_error(
                "PWM2 was not created after export"
            );
        }
    }


    writeFile(
        PwmPath + "/enable",
        0
    );

    writeFile(
        PwmPath + "/period",
        PeriodNs
    );

    writeFile(
        PwmPath + "/duty_cycle",
        angleToDuty(HomeAngle)
    );

    writeFile(
        PwmPath + "/enable",
        1
    );


    /*
     * 開機時永遠回到絕對 90°。
     */
    currentAngle_ = HomeAngle;
    targetAngle_ = HomeAngle;

    scanning_ = false;
    scanPhase_ = ScanPhase::Left;

    setDutyCycle(
        angleToDuty(HomeAngle)
    );

    std::this_thread::sleep_for(
        500ms
    );
}


void Servo::setDutyCycle(
    int dutyNs
)
{
    writeFile(
        PwmPath + "/duty_cycle",
        dutyNs
    );
}


int Servo::angleToDuty(
    int angle
) const
{
    angle = std::clamp(
        angle,
        0,
        180
    );

    return MinimumDutyNs
        +
        (
            angle
            * (MaximumDutyNs - MinimumDutyNs)
            / 180
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
                    ScanMiddleMinimum,
                    ScanMiddleMaximum
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
                    ScanMiddleMinimum,
                    ScanMiddleMaximum
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


void Servo::moveOneStepTowardTarget()
{
    if(currentAngle_ < targetAngle_)
    {
        currentAngle_ =
            std::min(
                currentAngle_ + StepDegrees,
                targetAngle_
            );
    }
    else if(currentAngle_ > targetAngle_)
    {
        currentAngle_ =
            std::max(
                currentAngle_ - StepDegrees,
                targetAngle_
            );
    }

    setDutyCycle(
        angleToDuty(
            currentAngle_
        )
    );
}


void Servo::moveSmooth(
    int targetAngle,
    int delayMs
)
{
    targetAngle =
        std::clamp(
            targetAngle,
            0,
            180
        );

    while(currentAngle_ != targetAngle)
    {
        if(currentAngle_ < targetAngle)
        {
            ++currentAngle_;
        }
        else
        {
            --currentAngle_;
        }

        setDutyCycle(
            angleToDuty(
                currentAngle_
            )
        );

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                delayMs
            )
        );
    }
}


void Servo::home()
{
    scanning_ = false;

    scanPhase_ =
        ScanPhase::Left;

    targetAngle_ =
        HomeAngle;

    moveSmooth(
        HomeAngle
    );
}


void Servo::startScanning()
{
    if(scanning_)
    {
        return;
    }

    scanning_ = true;

    scanPhase_ =
        ScanPhase::Left;

    chooseTargetForCurrentPhase();
}


void Servo::stopScanning()
{
    scanning_ = false;

    scanPhase_ =
        ScanPhase::Left;

    /*
     * 停止掃描後永遠回到絕對 90°，
     * 不是 75°–105° 的隨機中間。
     */
    targetAngle_ =
        HomeAngle;
}


void Servo::update()
{
    /*
     * 每次主迴圈只移動一小步。
     */
    moveOneStepTowardTarget();


    if(currentAngle_ != targetAngle_)
    {
        return;
    }


    /*
     * 沒有掃描時，到達 90° 後保持不動。
     */
    if(!scanning_)
    {
        return;
    }


    /*
     * 掃描模式抵達目標後，
     * 前往下一個區域。
     */
    advanceScanPhase();
}


bool Servo::isScanning() const
{
    return scanning_;
}


/*
 * ============================
 * Compatibility methods
 * ============================
 */

void Servo::shakeHead()
{
    startScanning();
    update();
}


void Servo::stopShaking()
{
    stopScanning();
    update();
}


void Servo::lookLeft()
{
    scanning_ = false;

    targetAngle_ =
        randomAngle(
            LeftMinimum,
            LeftMaximum
        );

    moveSmooth(
        targetAngle_
    );
}


void Servo::lookCenter()
{
    home();
}


void Servo::lookRight()
{
    scanning_ = false;

    targetAngle_ =
        randomAngle(
            RightMinimum,
            RightMaximum
        );

    moveSmooth(
        targetAngle_
    );
}