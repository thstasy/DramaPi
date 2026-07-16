#include <lgpio.h>

#include <chrono>
#include <iostream>
#include <thread>


int main()
{
    constexpr int GPIO = 19;

    const int handle = lgGpiochipOpen(0);

    if(handle < 0)
    {
        std::cerr
            << "lgGpiochipOpen failed: "
            << handle
            << '\n';

        return 1;
    }


    const int claimResult =
        lgGpioClaimOutput(
            handle,
            0,
            GPIO,
            0
        );

    if(claimResult < 0)
    {
        std::cerr
            << "lgGpioClaimOutput failed: "
            << claimResult
            << '\n';

        lgGpiochipClose(handle);
        return 1;
    }


    const int pwmResult =
        lgTxPwm(
            handle,
            GPIO,
            3000.0,
            50.0,
            0.0,
            0
        );

    std::cout
        << "lgTxPwm result: "
        << pwmResult
        << '\n';


    std::this_thread::sleep_for(
        std::chrono::seconds(3)
    );


    lgTxPwm(
        handle,
        GPIO,
        3000.0,
        0.0,
        0.0,
        0
    );


    lgGpioWrite(
        handle,
        GPIO,
        0
    );


    lgGpioFree(
        handle,
        GPIO
    );

    lgGpiochipClose(handle);

    return 0;
}