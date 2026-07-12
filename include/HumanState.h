#pragma once

enum class HumanState
{
    Alone,              // >200 cm
    Nearby,             // 150-200 cm
    Approaching,        // 100-150 cm
    Close,              // 50-100 cm
    PersonalSpace,      // 30-50 cm
    Emergency           // <30 cm
};