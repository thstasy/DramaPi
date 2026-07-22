#include "LCD1602.h"
#include "Led.h"
#include "Servo.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>


using namespace std::chrono_literals;


Led red(27);
Led green(17);

LCD1602 lcdDisplay(0x27);

Servo servo(18);


void lcd(
    const std::string& line1,
    const std::string& line2
)
{
    std::cout
        << "\n[LCD]\n"
        << line1
        << '\n'
        << line2
        << "\n\n";

    lcdDisplay.print(
        line1,
        line2
    );
}


void transition()
{
    red.off();
    green.off();

    servo.home();

    lcd(
        "Loading drama",
        "Please wait..."
    );

    std::this_thread::sleep_for(
        1500ms
    );
}


void judging()
{
    red.on();
    green.off();

    servo.startScanning();

    lcd(
        "Human input:",
        "Disappointing."
    );

    std::this_thread::sleep_for(
        2500ms
    );

    servo.home();
}


void germanMode()
{
    red.on();
    green.off();

    servo.startScanning();

    static const std::vector<
        std::pair<
            std::string,
            std::string
        >
    > lines =
    {
        {"Form missing", "Which? Secret."},
        {"Key returned.", "Rent continues."},
        {"Efficient?", "Nice myth."},
        {"Appointment?", "Next lifetime"},
        {"Your problem", "Not our dept."},
        {"Blue Card?", "Blue Screen."}
    };

    static std::mt19937 rng(
        std::random_device{}()
    );

    const auto& message =
        lines[
            rng() % lines.size()
        ];

    lcd(
        message.first,
        message.second
    );

    std::this_thread::sleep_for(
        2500ms
    );

    servo.home();
}


void bufferOverflow()
{
    servo.startEmergencyShake();

    for(int i = 0; i < 4; ++i)
    {
        red.on();
        green.off();

        std::this_thread::sleep_for(
            300ms
        );

        red.off();
        green.on();

        std::this_thread::sleep_for(
            300ms
        );
    }

    servo.home();

    lcd(
        "Too much",
        "drama."
    );
}


void existential()
{
    red.on();
    green.off();

    servo.home();

    lcd(
        "I am just",
        "electrons."
    );

    std::this_thread::sleep_for(
        1500ms
    );
}


void undefinedBehavior()
{
    red.on();
    green.on();

    servo.startEmergencyShake();

    lcd(
        "Undefined",
        "Behavior"
    );

    std::this_thread::sleep_for(
        3s
    );

    servo.home();
}


int main()
{
    lcdDisplay.init();

    servo.init();

    const std::vector<
        void (*)()
    > scenes =
    {
        judging,
        germanMode,
        bufferOverflow,
        existential,
        undefinedBehavior
    };

    std::mt19937 rng(
        std::random_device{}()
    );

    while(true)
    {
        transition();

        scenes[
            rng() % scenes.size()
        ]();

        std::this_thread::sleep_for(
            5s
        );
    }

    return 0;
}
