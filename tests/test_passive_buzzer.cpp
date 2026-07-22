#include "PassiveBuzzer.h"
#include "StartupMelody.h"

#include <iostream>

int main()
{
    try
    {
        PassiveBuzzer buzzer(19);

        buzzer.playMelody(
            StartupMelody,
            112,
            0
        );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}