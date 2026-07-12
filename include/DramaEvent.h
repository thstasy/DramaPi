#pragma once

enum class DramaEvent
{
    // System events
    Normal,
    CpuBusy,
    CpuIdle,
    MemoryHigh,
    DiskFull,
    Overheated,
    LongUptime,

    // Human interaction events
    HumanDetected,
    HumanTooClose,
    HumanTouched,
    HumanLeft,

    // Environmental events
    TooDark,
    TooBright,
    TooHot,
    TooHumid,

    // Emotional events
    ExistentialCrisis,
    FeelingIgnored
};