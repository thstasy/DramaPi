#include "Led.h"
#include "LCD1602.h"
#include "Servo.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

Led red(17);
Led green(27);
LCD1602 lcdDisplay(0x27);
Servo servo;

void lcd(const std::string& line1, const std::string& line2)
{
    std::cout << "\n[LCD]\n"
              << line1 << "\n"
              << line2 << "\n";

    lcdDisplay.print(line1, line2);
}

void transition()
{
    red.off();
    green.off();
    servo.lookCenter();

    lcd("Loading drama", "Please wait...");
    std::this_thread::sleep_for(1s);
}

void judging()
{
    red.on();
    green.off();
    servo.lookRight();

    lcd("Human input:", "Disappointing.");
}

void germanMode()
{
    red.on();
    green.off();
    servo.lookLeft();

    static std::vector<std::pair<std::string, std::string>> lines = {
        {"Form missing", "Which? Secret."},
        {"Key returned.", "Rent continues."},
        {"Efficient?", "Nice myth."},
        {"Appointment?", "Next lifetime"},
        {"Your problem", "Not our dept."},
        {"Blue Card?", "Blue Screen."}
    };

    static std::mt19937 rng(std::random_device{}());
    auto& msg = lines[rng() % lines.size()];

    lcd(msg.first, msg.second);
}

void bufferOverflow()
{
    for (int i = 0; i < 4; ++i) {
        red.on();
        green.off();
        servo.lookLeft();
        std::this_thread::sleep_for(300ms);

        red.off();
        green.on();
        servo.lookRight();
        std::this_thread::sleep_for(300ms);
    }

    lcd("Too much", "drama.");
}

void existential()
{
    red.on();
    green.off();
    servo.lookCenter();

    lcd("I am just", "electrons.");
}

void undefinedBehavior()
{
    red.on();
    green.on();
    servo.lookLeft();
    std::this_thread::sleep_for(500ms);

    servo.lookRight();

    lcd("Undefined", "Behavior");
}

int main()
{
    lcdDisplay.init();
    servo.init();

    std::vector<void(*)()> scenes = {
        judging,
        germanMode,
        bufferOverflow,
        existential,
        undefinedBehavior
    };

    std::mt19937 rng(std::random_device{}());

    while (true) {
        transition();
        scenes[rng() % scenes.size()]();
        std::this_thread::sleep_for(5s);
    }
}