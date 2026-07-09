#include "Servo.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

Servo::Servo()
{
}

bool Servo::exists(const std::string& path) const
{
    return std::filesystem::exists(path);
}

void Servo::writeFile(const std::string& path, int value)
{
    std::ofstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open: " + path);
    }

    file << value;
}

void Servo::init()
{
    if (!exists(PwmPath)) {
        writeFile(std::string(PwmChipPath) + "/export", Channel);
        std::this_thread::sleep_for(100ms);
    }

    writeFile(std::string(PwmPath) + "/period", PeriodNs);
    writeFile(std::string(PwmPath) + "/duty_cycle", CenterNs);
    writeFile(std::string(PwmPath) + "/enable", 1);
}

void Servo::setDutyCycle(int dutyNs)
{
    writeFile(std::string(PwmPath) + "/duty_cycle", dutyNs);
}

void Servo::lookLeft()
{
    setDutyCycle(LeftNs);
}

void Servo::lookCenter()
{
    setDutyCycle(CenterNs);
}

void Servo::lookRight()
{
    setDutyCycle(RightNs);
}