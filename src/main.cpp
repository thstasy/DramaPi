// #include <chrono>
// #include <iostream>
// #include <string>
// #include <thread>

// #include "Buzzer.h"
// #include "CpuMonitor.h"
// #include "DiskMonitor.h"
// #include "DistanceSensor.h"
// #include "DramaEngine.h"
// #include "EventDetector.h"
// #include "HumanDetector.h"
// #include "HumanInteraction.h"
// #include "HumanState.h"
// #include "LCD1602.h"
// #include "Led.h"
// #include "MemoryMonitor.h"
// #include "PassiveBuzzer.h"
// #include "Personality.h"
// #include "ReminderLibrary.h"
// #include "Servo.h"
// #include "StartupMelody.h"
// #include "SystemStatus.h"
// #include "TemperatureMonitor.h"
// #include "UptimeMonitor.h"


// using Clock = std::chrono::steady_clock;
// using TimePoint = Clock::time_point;


// namespace
// {
// constexpr int CloseEmergencyDelaySeconds = 10;
// constexpr int MainLoopDelayMs = 20;


// Personality parsePersonality(
//     const std::string& name
// )
// {
//     if(name == "dramatic")
//     {
//         return Personality::Dramatic;
//     }

//     if(name == "corporate")
//     {
//         return Personality::Corporate;
//     }

//     if(name == "emo")
//     {
//         return Personality::Emo;
//     }

//     if(name == "sarcastic")
//     {
//         return Personality::Sarcastic;
//     }

//     if(name == "existential")
//     {
//         return Personality::Existential;
//     }

//     return Personality::Dramatic;
// }


// std::string personalityName(
//     Personality personality
// )
// {
//     switch(personality)
//     {
//         case Personality::Dramatic:
//             return "Dramatic";

//         case Personality::Corporate:
//             return "Corporate";

//         case Personality::Emo:
//             return "Emo";

//         case Personality::Sarcastic:
//             return "Sarcastic";

//         case Personality::Existential:
//             return "Existential";
//     }

//     return "Unknown";
// }


// SystemStatus readSystemStatus(
//     TemperatureMonitor& temperatureMonitor,
//     CpuMonitor& cpuMonitor,
//     MemoryMonitor& memoryMonitor,
//     DiskMonitor& diskMonitor,
//     UptimeMonitor& uptimeMonitor
// )
// {
//     SystemStatus status;

//     status.cpuTemperature =
//         temperatureMonitor.getTemperature();

//     status.cpuUsage =
//         cpuMonitor.getCpuUsage();

//     status.memoryUsage =
//         memoryMonitor.getMemoryUsage();

//     status.diskUsage =
//         diskMonitor.getDiskUsage();

//     status.uptimeHours =
//         uptimeMonitor.getUptimeHours();

//     return status;
// }


// void printStatus(
//     const SystemStatus& status,
//     Personality personality,
//     float distance,
//     const std::string& drama
// )
// {
//     std::cout
//         << "============================\n"
//         << "🎭 Drama Pi\n"
//         << "============================\n"
//         << "Personality: "
//         << personalityName(personality)
//         << '\n'
//         << "CPU Temp: "
//         << status.cpuTemperature
//         << " °C\n"
//         << "CPU Usage: "
//         << status.cpuUsage
//         << " %\n"
//         << "Memory Usage: "
//         << status.memoryUsage
//         << " %\n"
//         << "Disk Usage: "
//         << status.diskUsage
//         << " %\n"
//         << "Human Distance: "
//         << distance
//         << " cm\n"
//         << "Drama Pi: "
//         << drama
//         << "\n\n";
// }


// HumanState resolveHumanState(
//     HumanState detectedState,
//     const TimePoint& now,
//     TimePoint& closeStart
// )
// {
//     /*
//      * Close 必須持續 10 秒，才升級成 Emergency。
//      * 一旦離開 Close，計時立即清除。
//      */
//     if(detectedState != HumanState::Close)
//     {
//         closeStart = TimePoint{};

//         return detectedState;
//     }

//     if(closeStart.time_since_epoch().count() == 0)
//     {
//         closeStart = now;

//         return HumanState::Close;
//     }

//     const auto closeSeconds =
//         std::chrono::duration_cast<
//             std::chrono::seconds
//         >(
//             now - closeStart
//         ).count();

//     if(closeSeconds >= CloseEmergencyDelaySeconds)
//     {
//         return HumanState::Emergency;
//     }

//     return HumanState::Close;
// }


// DramaEvent resolveDramaEvent(
//     DramaEvent systemEvent,
//     HumanState humanState
// )
// {
//     if(humanState == HumanState::Emergency)
//     {
//         return DramaEvent::HumanTooClose;
//     }

//     if(humanState != HumanState::Alone)
//     {
//         return DramaEvent::HumanDetected;
//     }

//     return systemEvent;
// }


// void handleAlone(
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo
// )
// {
//     lcd.print(
//         "Drama Pi",
//         "Watching..."
//     );

//     greenLed.on();
//     redLed.off();

//     buzzer.off();

//     servo.home();
// }


// void handleNearby(
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo
// )
// {
//     lcd.print(
//         "Human detected",
//         "I see you..."
//     );

//     greenLed.on();
//     redLed.off();

//     buzzer.off();

//     servo.home();
// }


// void handleApproaching(
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo
// )
// {
//     lcd.print(
//         "Approaching",
//         "Please slow down"
//     );

//     greenLed.on();
//     redLed.off();

//     buzzer.off();

//     servo.startScanning();
// }


// void handleClose(
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo
// )
// {
//     lcd.print(
//         "PERSONAL SPACE",
//         "BACK AWAY"
//     );

//     greenLed.off();
//     redLed.on();

//     buzzer.off();

//     servo.startScanning();
// }


// void handlePersonalSpace(
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo
// )
// {
//     lcd.print(
//         "PERSONAL SPACE",
//         "NO HARASSMENT"
//     );

//     redLed.on();
//     greenLed.off();

//     buzzer.off();

//     servo.startScanning();
// }


// void handleEmergency(
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo,
//     int& emergencyMessage
// )
// {
//     switch(emergencyMessage)
//     {
//         case 0:
//             lcd.print(
//                 "EMERGENCY",
//                 "HR notified"
//             );
//             break;

//         case 1:
//             lcd.print(
//                 "SOCIAL CUES",
//                 "IGNORED"
//             );
//             break;

//         case 2:
//             lcd.print(
//                 "HR NOTIFIED",
//                 "HR DOES NOTHING"
//             );
//             break;
//     }

//     emergencyMessage =
//         (emergencyMessage + 1) % 3;

//     redLed.on();
//     greenLed.off();

//     buzzer.on();

//     /*
//      * Emergency 專用的快速左右甩頭。
//      * emergencyShake() 本身會短暫阻塞。
//      */
//     servo.startEmergencyShake();

//     std::cout
//         << "\n🚨 EMERGENCY\n"
//         << "HR not care.\n\n";
// }


// void handlePhysicalReaction(
//     HumanState humanState,
//     LCD1602& lcd,
//     Led& redLed,
//     Led& greenLed,
//     Buzzer& buzzer,
//     Servo& servo,
//     int& emergencyMessage
// )
// {
//     switch(humanState)
//     {
//         case HumanState::Alone:
//             handleAlone(
//                 lcd,
//                 redLed,
//                 greenLed,
//                 buzzer,
//                 servo
//             );
//             break;

//         case HumanState::Nearby:
//             handleNearby(
//                 lcd,
//                 redLed,
//                 greenLed,
//                 buzzer,
//                 servo
//             );
//             break;

//         case HumanState::Approaching:
//             handleApproaching(
//                 lcd,
//                 redLed,
//                 greenLed,
//                 buzzer,
//                 servo
//             );
//             break;

//         case HumanState::Close:
//             handleClose(
//                 lcd,
//                 redLed,
//                 greenLed,
//                 buzzer,
//                 servo
//             );
//             break;

//         case HumanState::PersonalSpace:
//             handlePersonalSpace(
//                 lcd,
//                 redLed,
//                 greenLed,
//                 buzzer,
//                 servo
//             );
//             break;

//         case HumanState::Emergency:
//             handleEmergency(
//                 lcd,
//                 redLed,
//                 greenLed,
//                 buzzer,
//                 servo,
//                 emergencyMessage
//             );
//             break;
//     }
// }


// void handleReminder(
//     const TimePoint& now,
//     TimePoint& lastReminder,
//     ReminderLibrary& reminderLibrary,
//     Personality personality,
//     LCD1602& lcd
// )
// {
//     const auto minutes =
//         std::chrono::duration_cast<
//             std::chrono::minutes
//         >(
//             now - lastReminder
//         ).count();

//     if(minutes < 5)
//     {
//         return;
//     }

//     const std::string reminder =
//         reminderLibrary.reminder(
//             personality
//         );

//     std::cout
//         << "⏰ Reminder: "
//         << reminder
//         << "\n\n";

//     lcd.print(
//         "REMINDER",
//         reminder.substr(
//             0,
//             16
//         )
//     );

//     lastReminder = now;
// }
// }


// int main(
//     int argc,
//     char* argv[]
// )
// {
//     /*
//      * ============================
//      * Software components
//      * ============================
//      */

//     TemperatureMonitor temperatureMonitor;
//     CpuMonitor cpuMonitor;
//     MemoryMonitor memoryMonitor;
//     DiskMonitor diskMonitor;
//     UptimeMonitor uptimeMonitor;

//     EventDetector eventDetector;
//     HumanDetector humanDetector;
//     DramaEngine dramaEngine;
//     ReminderLibrary reminderLibrary;
//     HumanInteraction humanInteraction;


//     /*
//      * ============================
//      * Hardware components
//      * ============================
//      */

//     DistanceSensor distanceSensor(
//         7,
//         8
//     );

//     LCD1602 lcd(0x27);

//     lcd.init();

//     Led redLed(27);
//     Led greenLed(17);

//     Buzzer buzzer(22);

//     buzzer.off();

//     PassiveBuzzer musicBuzzer(19);

//     Servo servo(18);

//     servo.init();


//     /*
//      * ============================
//      * Startup sequence
//      * ============================
//      */

//     lcd.print(
//         "Drama Pi",
//         "Bonjour..."
//     );

//     musicBuzzer.playMelody(
//         StartupMelody,
//         112,
//         0
//     );


//     /*
//      * ============================
//      * Runtime state
//      * ============================
//      */

//     Personality personality =
//         Personality::Dramatic;

//     if(argc > 1)
//     {
//         personality =
//             parsePersonality(
//                 argv[1]
//             );
//     }

//     TimePoint lastReminder =
//         Clock::now();

//     TimePoint closeStart{};

//     int emergencyMessage = 0;


//     /*
//      * ============================
//      * Main loop
//      * ============================
//      */

//     while(true)
//     {
//         const auto now =
//             Clock::now();

//         const SystemStatus status =
//             readSystemStatus(
//                 temperatureMonitor,
//                 cpuMonitor,
//                 memoryMonitor,
//                 diskMonitor,
//                 uptimeMonitor
//             );

//         const float distance =
//             distanceSensor.distance();

//         const HumanState detectedState =
//             humanDetector.detect(
//                 distance
//             );

//         const HumanState humanState =
//             resolveHumanState(
//                 detectedState,
//                 now,
//                 closeStart
//             );

//         humanInteraction.update(
//             humanState
//         );

//         const DramaEvent systemEvent =
//             eventDetector.detect(
//                 status
//             );

//         const DramaEvent event =
//             resolveDramaEvent(
//                 systemEvent,
//                 humanState
//             );

//         const std::string drama =
//             dramaEngine.comment(
//                 event,
//                 personality
//             );

//         printStatus(
//             status,
//             personality,
//             distance,
//             drama
//         );

//         handlePhysicalReaction(
//             humanState,
//             lcd,
//             redLed,
//             greenLed,
//             buzzer,
//             servo,
//             emergencyMessage
//         );

//         handleReminder(
//             now,
//             lastReminder,
//             reminderLibrary,
//             personality,
//             lcd
//         );

//         std::this_thread::sleep_for(
//             std::chrono::milliseconds(
//                 MainLoopDelayMs
//             )
//         );
//     }

//     return 0;
// }

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Buzzer.h"
#include "CpuMonitor.h"
#include "DiskMonitor.h"
#include "DistanceSensor.h"
#include "DramaEngine.h"
#include "EventDetector.h"
#include "HumanDetector.h"
#include "HumanInteraction.h"
#include "HumanState.h"
#include "LCD1602.h"
#include "Led.h"
#include "MemoryMonitor.h"
#include "PassiveBuzzer.h"
#include "Personality.h"
#include "ReminderLibrary.h"
#include "Servo.h"
#include "StartupMelody.h"
#include "StatusLights.h"
#include "StatusLightMode.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"
#include "UptimeMonitor.h"


using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;


namespace
{
constexpr int CloseEmergencyDelaySeconds = 10;
constexpr int MainLoopDelayMs = 100;


Personality parsePersonality(
    const std::string& name
)
{
    if(name == "corporate")
    {
        return Personality::Corporate;
    }

    if(name == "emo")
    {
        return Personality::Emo;
    }

    if(name == "sarcastic")
    {
        return Personality::Sarcastic;
    }

    if(name == "existential")
    {
        return Personality::Existential;
    }

    return Personality::Dramatic;
}


std::string personalityName(
    Personality personality
)
{
    switch(personality)
    {
        case Personality::Dramatic:
            return "Dramatic";

        case Personality::Corporate:
            return "Corporate";

        case Personality::Emo:
            return "Emo";

        case Personality::Sarcastic:
            return "Sarcastic";

        case Personality::Existential:
            return "Existential";
    }

    return "Unknown";
}


SystemStatus readSystemStatus(
    TemperatureMonitor& temperatureMonitor,
    CpuMonitor& cpuMonitor,
    MemoryMonitor& memoryMonitor,
    DiskMonitor& diskMonitor,
    UptimeMonitor& uptimeMonitor
)
{
    SystemStatus status;

    status.cpuTemperature =
        temperatureMonitor.getTemperature();

    status.cpuUsage =
        cpuMonitor.getCpuUsage();

    status.memoryUsage =
        memoryMonitor.getMemoryUsage();

    status.diskUsage =
        diskMonitor.getDiskUsage();

    status.uptimeHours =
        uptimeMonitor.getUptimeHours();

    return status;
}


HumanState resolveHumanState(
    HumanState detectedState,
    const TimePoint& now,
    TimePoint& closeStart
)
{
    if(detectedState != HumanState::Close)
    {
        closeStart = TimePoint{};

        return detectedState;
    }

    if(closeStart.time_since_epoch().count() == 0)
    {
        closeStart = now;

        return HumanState::Close;
    }

    const auto closeSeconds =
        std::chrono::duration_cast<
            std::chrono::seconds
        >(
            now - closeStart
        ).count();

    if(closeSeconds >= CloseEmergencyDelaySeconds)
    {
        return HumanState::Emergency;
    }

    return HumanState::Close;
}


DramaEvent resolveDramaEvent(
    DramaEvent systemEvent,
    HumanState humanState
)
{
    if(humanState == HumanState::Emergency)
    {
        return DramaEvent::HumanTooClose;
    }

    if(humanState != HumanState::Alone)
    {
        return DramaEvent::HumanDetected;
    }

    return systemEvent;
}


void printStatus(
    const SystemStatus& status,
    Personality personality,
    float distance,
    const std::string& drama
)
{
    std::cout
        << "============================\n"
        << "🎭 Drama Pi\n"
        << "============================\n"
        << "Personality: "
        << personalityName(personality)
        << '\n'
        << "CPU Temp: "
        << status.cpuTemperature
        << " °C\n"
        << "CPU Usage: "
        << status.cpuUsage
        << " %\n"
        << "Memory Usage: "
        << status.memoryUsage
        << " %\n"
        << "Disk Usage: "
        << status.diskUsage
        << " %\n"
        << "Human Distance: "
        << distance
        << " cm\n"
        << "Drama Pi: "
        << drama
        << "\n\n";
}


void handleHumanState(
    HumanState humanState,
    LCD1602& lcd,
    StatusLights& lights,
    Buzzer& buzzer,
    Servo& servo,
    int& emergencyMessage
)
{
    switch(humanState)
    {
        case HumanState::Alone:

            lcd.print(
                "Drama Pi",
                "Watching..."
            );

            lights.setMode(
                StatusLightMode::Safe
            );

            buzzer.off();

            servo.home();

            break;


        case HumanState::Nearby:

            lcd.print(
                "Human detected",
                "I see you..."
            );

            lights.setMode(
                StatusLightMode::Safe
            );

            buzzer.off();

            servo.home();

            break;


        case HumanState::Approaching:

            lcd.print(
                "Approaching",
                "Please slow down"
            );

            lights.setMode(
                StatusLightMode::Warning
            );

            buzzer.off();

            servo.startScanning();

            break;


        case HumanState::Close:

            lcd.print(
                "PERSONAL SPACE",
                "BACK AWAY"
            );

            lights.setMode(
                StatusLightMode::Danger
            );

            buzzer.off();

            servo.startScanning();

            break;


        case HumanState::PersonalSpace:

            lcd.print(
                "PERSONAL SPACE",
                "NO HARASSMENT"
            );

            lights.setMode(
                StatusLightMode::Danger
            );

            buzzer.off();

            servo.startScanning();

            break;


        case HumanState::Emergency:

            switch(emergencyMessage)
            {
                case 0:
                    lcd.print(
                        "EMERGENCY",
                        "HR notified"
                    );
                    break;

                case 1:
                    lcd.print(
                        "SOCIAL CUES",
                        "IGNORED"
                    );
                    break;

                case 2:
                    lcd.print(
                        "HR NOTIFIED",
                        "HR DOES NOTHING"
                    );
                    break;
            }

            emergencyMessage =
                (emergencyMessage + 1) % 3;

            lights.setMode(
                StatusLightMode::Emergency
            );

            buzzer.on();

            servo.startEmergencyShake();

            std::cout
                << "\n🚨 EMERGENCY\n"
                << "HR not care.\n\n";

            break;
    }
}


void handleReminder(
    const TimePoint& now,
    TimePoint& lastReminder,
    ReminderLibrary& reminderLibrary,
    Personality personality,
    LCD1602& lcd
)
{
    const auto minutes =
        std::chrono::duration_cast<
            std::chrono::minutes
        >(
            now - lastReminder
        ).count();

    if(minutes < 5)
    {
        return;
    }

    const std::string reminder =
        reminderLibrary.reminder(
            personality
        );

    std::cout
        << "⏰ Reminder: "
        << reminder
        << "\n\n";

    lcd.print(
        "REMINDER",
        reminder.substr(
            0,
            16
        )
    );

    lastReminder = now;
}
}


int main(
    int argc,
    char* argv[]
)
{
    TemperatureMonitor temperatureMonitor;
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    DiskMonitor diskMonitor;
    UptimeMonitor uptimeMonitor;

    EventDetector eventDetector;
    HumanDetector humanDetector;
    DramaEngine dramaEngine;
    ReminderLibrary reminderLibrary;
    HumanInteraction humanInteraction;

    DistanceSensor distanceSensor(
        7,
        8
    );

    LCD1602 lcd(0x27);

    lcd.init();

    Led redLed(27);
    Led greenLed(17);

    StatusLights lights(
        redLed,
        greenLed
    );

    Buzzer buzzer(22);

    buzzer.off();

    PassiveBuzzer musicBuzzer(19);

    Servo servo(18);

    servo.init();

    lcd.print(
        "Drama Pi",
        "Bonjour..."
    );

    musicBuzzer.playMelody(
        StartupMelody,
        112,
        0
    );

    Personality personality =
        Personality::Dramatic;

    if(argc > 1)
    {
        personality =
            parsePersonality(
                argv[1]
            );
    }

    TimePoint lastReminder =
        Clock::now();

    TimePoint closeStart{};

    int emergencyMessage = 0;

    while(true)
    {
        const auto now =
            Clock::now();

        const SystemStatus status =
            readSystemStatus(
                temperatureMonitor,
                cpuMonitor,
                memoryMonitor,
                diskMonitor,
                uptimeMonitor
            );

        const float distance =
            distanceSensor.distance();

        const HumanState detectedState =
            humanDetector.detect(
                distance
            );

        const HumanState humanState =
            resolveHumanState(
                detectedState,
                now,
                closeStart
            );

        humanInteraction.update(
            humanState
        );

        const DramaEvent systemEvent =
            eventDetector.detect(
                status
            );

        const DramaEvent event =
            resolveDramaEvent(
                systemEvent,
                humanState
            );

        const std::string drama =
            dramaEngine.comment(
                event,
                personality
            );

        printStatus(
            status,
            personality,
            distance,
            drama
        );

        handleHumanState(
            humanState,
            lcd,
            lights,
            buzzer,
            servo,
            emergencyMessage
        );

        handleReminder(
            now,
            lastReminder,
            reminderLibrary,
            personality,
            lcd
        );

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                MainLoopDelayMs
            )
        );
    }

    return 0;
}
