#include "MoodEngine.h"

Mood MoodEngine::evaluate(const SystemStatus& status) const
{
    if (status.cpuTemperature > 75.0) {
        return Mood::ExistentialCrisis;
    }

    if (status.memoryUsage > 85.0) {
        return Mood::Angry;
    }

    if (status.cpuUsage > 80.0) {
        return Mood::Busy;
    }

    if (status.cpuUsage < 5.0) {
        return Mood::HeartBroken;
    }

    if (status.diskUsage > 90.0) {
        return Mood::ExistentialCrisis;
    }

    return Mood::Calm;
}