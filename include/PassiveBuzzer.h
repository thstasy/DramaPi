#pragma once

#include "Melody.h"

#include <string>
#include <vector>


class PassiveBuzzer
{
public:

    explicit PassiveBuzzer(int gpio);

    ~PassiveBuzzer();


    void playTone(
        double frequency,
        int durationMs
    );


    void playMelody(
        const std::vector<Note>& melody,
        int bpm,
        int octaveShift = 0
    );


    void stop();


private:

    double frequencyFor(
        const std::string& pitch,
        int octaveShift
    ) const;


    int gpio_;
    int chipHandle_;
};