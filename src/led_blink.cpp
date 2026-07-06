#include <gpiod.hpp>
#include <chrono>
#include <thread>

int main()
{
    constexpr unsigned int GPIO17 = 17;

    auto request =
        gpiod::chip("/dev/gpiochip4")
            .prepare_request()
            .set_consumer("led")
            .add_line_settings(
                GPIO17,
                gpiod::line_settings()
                    .set_direction(gpiod::line::direction::OUTPUT)
            )
            .do_request();

    while (true)
    {
        request.set_value(GPIO17, gpiod::line::value::ACTIVE);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        request.set_value(GPIO17, gpiod::line::value::INACTIVE);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
