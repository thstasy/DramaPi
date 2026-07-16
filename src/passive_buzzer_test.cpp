// #include "PassiveBuzzer.h"

// #include <thread>
// #include <chrono>


// struct Note
// {
//     int frequency;
//     int duration;
// };


// int main()
// {

//     PassiveBuzzer buzzer(19);

//     buzzer.init();



//     Note melody[] =
//     {

//         {440, 500},   // A4
//         {440, 500},   // A4
//         {440, 500},   // A4

//         {349, 350},   // F4
//         {523, 150},   // C5

//         {440, 500},   // A4

//         {349, 350},   // F4
//         {523, 150},   // C5
//         {440, 800},   // A4

//     };



//     for(auto& note : melody)
//     {

//         buzzer.playTone(
//             note.frequency,
//             note.duration
//         );


//         std::this_thread::sleep_for(
//             std::chrono::milliseconds(50)
//         );

//     }


//     buzzer.stop();


//     return 0;
// }
// #include "PassiveBuzzer.h"
// #include "StartupMelody.h"


// int main()
// {
//     PassiveBuzzer buzzer(19);

//     buzzer.playMelody(
//         StartupMelody,
//         120,
//         2
//     );

//     return 0;
// }

#include "PassiveBuzzer.h"

#include <thread>
#include <chrono>

int main()
{
    PassiveBuzzer buzzer(19);

    for(int i = 0; i < 4; ++i)
    {
        buzzer.playTone(2000, 800);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(300)
        );

        buzzer.playTone(4000, 800);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(300)
        );
    }

    return 0;
}