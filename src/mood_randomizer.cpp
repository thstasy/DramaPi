#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

void run(const std::string& cmd) {
    std::system(cmd.c_str());
}

void redOn()    { run("sudo pinctrl set 17 op dh"); }
void redOff()   { run("sudo pinctrl set 17 op dl"); }
void greenOn()  { run("sudo pinctrl set 27 op dh"); }
void greenOff() { run("sudo pinctrl set 27 op dl"); }

void servo(int duty) {
    run("echo " + std::to_string(duty) +
        " | sudo tee /sys/class/pwm/pwmchip0/pwm2/duty_cycle > /dev/null");
}

void lcd(const std::string& line1, const std::string& line2) {
    std::cout << "\n[LCD]\n"
              << line1 << "\n"
              << line2 << "\n";
}

void transition() {
    redOff();
    greenOff();
    servo(1500000);
    lcd("Loading drama", "Please wait...");
    std::this_thread::sleep_for(1s);
}

void judging() {
    redOn();
    greenOff();
    servo(2000000);
    lcd("Human input:", "Disappointing.");
}

void germanMode() {
    redOn();
    greenOff();
    servo(1000000);

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

void bufferOverflow() {
    for (int i = 0; i < 4; ++i) {
        redOn(); greenOff(); servo(1000000);
        std::this_thread::sleep_for(300ms);
        redOff(); greenOn(); servo(2000000);
        std::this_thread::sleep_for(300ms);
    }
    lcd("Too much", "drama.");
}

void existential() {
    redOn();
    greenOff();
    servo(1500000);
    lcd("I am just", "electrons.");
}

void undefinedBehavior() {
    redOn();
    greenOn();
    servo(1000000);
    std::this_thread::sleep_for(500ms);
    servo(2000000);
    lcd("Undefined", "Behavior");
}

int main() {
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
