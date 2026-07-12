#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

void run(const std::string& cmd)
{
    std::cout << cmd << std::endl;
    std::system(cmd.c_str());
}

int main()
{
    // GPIO18 = physical pin 12
    run("pinctrl set 18 op pn");
    run("pinctrl set 18 a0");

    while (true)
    {
        std::cout << "DramaPi looks left..." << std::endl;
        run("echo 1000000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "DramaPi judges silently..." << std::endl;
        run("echo 1500000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "DramaPi makes a dramatic exit." << std::endl;
        run("echo 2000000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
