#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std::chrono_literals;

void run(const char* cmd)
{
    std::system(cmd);
}

int level(int gpio)
{
    std::string cmd =
        "pinctrl lev " + std::to_string(gpio);

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return 0;

    char buffer[16];
    fgets(buffer, sizeof(buffer), pipe);
    pclose(pipe);

    return std::atoi(buffer);
}

int main()
{
    const int trig = 24;
    const int echo = 25;

    run("sudo pinctrl set 24 op dl");
    run("sudo pinctrl set 25 ip pd");

    while (true) {
        run("sudo pinctrl set 24 dl");
        std::this_thread::sleep_for(2us);

        run("sudo pinctrl set 24 dh");
        std::this_thread::sleep_for(10us);
        run("sudo pinctrl set 24 dl");

        auto timeoutStart = steady_clock::now();

        while (level(echo) == 0) {
            if (steady_clock::now() - timeoutStart > 50ms) {
                std::cout << "Timeout waiting for echo HIGH\n";
                goto next;
            }
        }

        {
            auto start = steady_clock::now();

            while (level(echo) == 1) {
                if (steady_clock::now() - start > 50ms) {
                    std::cout << "Timeout waiting for echo LOW\n";
                    goto next;
                }
            }

            auto end = steady_clock::now();
            auto durationUs = duration_cast<microseconds>(end - start).count();

            double distanceCm = durationUs * 0.0343 / 2.0;

            std::cout << "Distance: " << distanceCm << " cm\n";
        }

    next:
        std::this_thread::sleep_for(500ms);
    }
}
