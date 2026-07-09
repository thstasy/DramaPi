#pragma once

#include <string>

class Servo
{
public:
    Servo();

    void init();

    void lookLeft();
    void lookCenter();
    void lookRight();

private:
    static constexpr const char* PwmChipPath = "/sys/class/pwm/pwmchip0";
    static constexpr const char* PwmPath = "/sys/class/pwm/pwmchip0/pwm2";

    static constexpr int Channel = 2;
    static constexpr int PeriodNs = 20000000;   // 20 ms = 50 Hz
    static constexpr int LeftNs = 1000000;      // 1.0 ms
    static constexpr int CenterNs = 1500000;    // 1.5 ms
    static constexpr int RightNs = 2000000;     // 2.0 ms

    void writeFile(const std::string& path, int value);
    bool exists(const std::string& path) const;
    void setDutyCycle(int dutyNs);
};