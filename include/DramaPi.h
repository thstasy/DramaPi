#pragma once

#include "Personality.h"

class DramaPi
{
public:
    DramaPi(int argc, char* argv[]);
    void run();

private:
    Personality personality;
    bool reminderMode;

    void runStatusOnce();
    void runReminderLoop();
};