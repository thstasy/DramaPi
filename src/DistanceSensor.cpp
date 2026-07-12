#include "DistanceSensor.h"

#include <gpiod.hpp>
#include <chrono>
#include <thread>


DistanceSensor::DistanceSensor(
    int triggerPin,
    int echoPin
)
    :
    triggerPin(triggerPin),
    echoPin(echoPin)
{
}


float DistanceSensor::distance() const
{
    gpiod::chip chip("/dev/gpiochip0");

    // Trigger pin: OUTPUT
    auto trig =
        chip.prepare_request()
            .set_consumer("dramapi-trigger")
            .add_line_settings(
                triggerPin,
                gpiod::line_settings()
                    .set_direction(
                        gpiod::line::direction::OUTPUT
                    )
            )
            .do_request();



    // Echo pin: INPUT
    auto echo =
        chip.prepare_request()
            .set_consumer("dramapi-echo")
            .add_line_settings(
                echoPin,
                gpiod::line_settings()
                    .set_direction(
                        gpiod::line::direction::INPUT
                    )
            )
            .do_request();



    // Send 10us trigger pulse

    trig.set_value(
        triggerPin,
        gpiod::line::value::ACTIVE
    );


    std::this_thread::sleep_for(
        std::chrono::microseconds(10)
    );


    trig.set_value(
        triggerPin,
        gpiod::line::value::INACTIVE
    );



    // Wait for echo rising edge

    auto start =
        std::chrono::high_resolution_clock::now();


    while(
        echo.get_value(echoPin)
        ==
        gpiod::line::value::INACTIVE
    )
    {
        start =
            std::chrono::high_resolution_clock::now();
    }



    // Wait for echo falling edge

    auto end = start;


    while(
        echo.get_value(echoPin)
        ==
        gpiod::line::value::ACTIVE
    )
    {
        end =
            std::chrono::high_resolution_clock::now();
    }



    auto duration =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start)
        .count();



    // HC-SR04:
    // distance(cm) = time(us) / 58

    return duration / 58.0;
}