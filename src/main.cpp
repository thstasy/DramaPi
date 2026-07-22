#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Button.h"
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
#include "MatrixController.h"
#include "MatrixMode.h"
#include "MemoryMonitor.h"
#include "PassiveBuzzer.h"
#include "Personality.h"
#include "ReminderLibrary.h"
#include "RGBLed.h"
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
constexpr int MatrixMessageDurationMs = 1500;
constexpr int ChoiceLoopDelayMs = 30;
constexpr int BreathingMinimum = 8;
constexpr int BreathingMaximum = 100;
constexpr int BreathingStep = 3;


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


std::string matrixModeName(
    MatrixMode mode
)
{
    switch(mode)
    {
        case MatrixMode::Neutral:
            return "Neutral";

        case MatrixMode::BluePill:
            return "Blue Pill";

        case MatrixMode::RedPill:
            return "Red Pill";
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
    MatrixMode matrixMode,
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
        << "Matrix Mode: "
        << matrixModeName(matrixMode)
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


void showMatrixModeChange(
    MatrixMode mode,
    LCD1602& lcd
)
{
    switch(mode)
    {
        case MatrixMode::Neutral:

            lcd.print(
                "MATRIX MODE",
                "Neutral"
            );

            std::cout
                << "Matrix mode: Neutral\n";

            break;


        case MatrixMode::BluePill:

            lcd.print(
                "BLUE PILL",
                "Asleep"
            );

            std::cout
                << "Matrix mode: Blue Pill\n";

            break;


        case MatrixMode::RedPill:

            lcd.print(
                "RED PILL",
                "Truth hurts"
            );

            std::cout
                << "Matrix mode: Red Pill\n";

            break;
    }
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
    /*
     * Active Buzzer 統一在這裡控制：
     *
     * Emergency → 開
     * 其他狀態   → 關
     *
     * 這樣離開 Emergency 後，
     * 下一輪一定會自動關閉，不會殘留。
     */
    const bool isEmergency =
        humanState
        == HumanState::Emergency;

    if(isEmergency)
    {
        buzzer.on();
    }
    else
    {
        buzzer.off();

        /*
         * 離開 Emergency 後，
         * 下次從第一則警告訊息重新開始。
         */
        emergencyMessage = 0;
    }


    switch(humanState)
    {
        case HumanState::Alone:
        {
            lcd.print(
                "Drama Pi",
                "Watching..."
            );

            lights.setMode(
                StatusLightMode::Safe
            );

            servo.home();

            break;
        }


        case HumanState::Nearby:
        {
            lcd.print(
                "Human detected",
                "I see you..."
            );

            lights.setMode(
                StatusLightMode::Safe
            );

            servo.home();

            break;
        }


        case HumanState::Approaching:
        {
            lcd.print(
                "Approaching",
                "Please slow down"
            );

            lights.setMode(
                StatusLightMode::Warning
            );

            servo.startScanning();

            break;
        }


        case HumanState::Close:
        {
            lcd.print(
                "PERSONAL SPACE",
                "BACK AWAY"
            );

            lights.setMode(
                StatusLightMode::Danger
            );

            servo.startScanning();

            break;
        }


        case HumanState::PersonalSpace:
        {
            lcd.print(
                "PERSONAL SPACE",
                "NO HARASSMENT"
            );

            lights.setMode(
                StatusLightMode::Danger
            );

            servo.startScanning();

            break;
        }


        case HumanState::Emergency:
        {
            switch(emergencyMessage)
            {
                case 0:
                {
                    lcd.print(
                        "EMERGENCY",
                        "HR notified"
                    );

                    break;
                }


                case 1:
                {
                    lcd.print(
                        "SOCIAL CUES",
                        "IGNORED"
                    );

                    break;
                }


                case 2:
                {
                    lcd.print(
                        "HR NOTIFIED",
                        "HR DOES NOTHING"
                    );

                    break;
                }
            }

            emergencyMessage =
                (emergencyMessage + 1)
                % 3;

            lights.setMode(
                StatusLightMode::Emergency
            );

            servo.startEmergencyShake();

            std::cout
                << "\n🚨 EMERGENCY\n"
                << "HR not care.\n\n";

            break;
        }
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
    /*
     * ============================
     * Software components
     * ============================
     */

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


    /*
     * ============================
     * Hardware components
     * ============================
     */

    DistanceSensor distanceSensor(
        7,
        8
    );

    LCD1602 lcd(0x27);

    lcd.init();


    /*
     * Separate red and green status LEDs.
     */
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


    /*
     * Matrix buttons:
     *
     * Red button  = GPIO25 / Physical Pin 22
     * Blue button = GPIO11 / Physical Pin 23
     *
     * Both buttons are active-low.
     */
    /*
     * 實測為 active-high：
     * 未按 = LOW，按下 = HIGH。
     */
    Button redButton(
        25,
        false
    );

    Button blueButton(
        11,
        false
    );


    /*
     * RGB LED channel mapping confirmed by testing:
     *
     * Red   = GPIO12 / Physical Pin 32
     * Green = GPIO6  / Physical Pin 31
     * Blue  = GPIO5  / Physical Pin 29
     *
     * RGB LED is active-high:
     * 1 = on, 0 = off.
     */
    RGBLed matrixLed(
        12,
        6,
        5
    );

    MatrixController matrixController(
        redButton,
        blueButton,
        matrixLed
    );

    matrixController.init();


    /*
     * ============================
     * Startup sequence
     * ============================
     */

    lcd.print(
        "Drama Pi",
        "Bonjour..."
    );

    musicBuzzer.playMelody(
        StartupMelody,
        112,
        0
    );


    /*
     * ============================
     * Initial Matrix choice
     * ============================
     */

    lcd.print(
        "Choose RED or",
        "BLUE button"
    );

    servo.home();
    buzzer.off();

    int breathingBrightness =
        BreathingMinimum;

    int breathingDirection =
        BreathingStep;

    while(true)
    {
        matrixController.updateButtons();

        if(matrixController.buttonTriggered())
        {
            const MatrixMode selectedMode =
                matrixController.triggeredMode();

            /*
             * 開機時必須選擇 Red 或 Blue。
             * 同時按下造成的 Neutral 不會結束等待。
             */
            if(selectedMode != MatrixMode::Neutral)
            {
                showMatrixModeChange(
                    selectedMode,
                    lcd
                );

                matrixController.showModeColor();

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(
                        MatrixMessageDurationMs
                    )
                );

                break;
            }

            lcd.print(
                "Choose RED or",
                "BLUE button"
            );
        }

        /*
         * 等待選擇時，RGB 綠色緩慢呼吸。
         */
        matrixLed.setBrightness(
            0,
            breathingBrightness,
            0
        );

        breathingBrightness +=
            breathingDirection;

        if(
            breathingBrightness
            >= BreathingMaximum
        )
        {
            breathingBrightness =
                BreathingMaximum;

            breathingDirection =
                -BreathingStep;
        }
        else if(
            breathingBrightness
            <= BreathingMinimum
        )
        {
            breathingBrightness =
                BreathingMinimum;

            breathingDirection =
                BreathingStep;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                ChoiceLoopDelayMs
            )
        );
    }


    /*
     * ============================
     * Personality
     * ============================
     */

    Personality personality =
        Personality::Dramatic;

    if(argc > 1)
    {
        personality =
            parsePersonality(
                argv[1]
            );
    }


    /*
     * ============================
     * Runtime state
     * ============================
     */

    TimePoint lastReminder =
        Clock::now();

    TimePoint closeStart{};

    TimePoint matrixMessageUntil{};

    int emergencyMessage = 0;


    /*
     * ============================
     * Main loop
     * ============================
     */

    while(true)
    {
        const auto now =
            Clock::now();


        /*
         * 每次重新按下紅色或藍色按鈕，
         * 即使模式相同，也重新顯示 LCD 與 RGB 回饋。
         */
        matrixController.updateButtons();

        if(matrixController.buttonTriggered())
        {
            showMatrixModeChange(
                matrixController.triggeredMode(),
                lcd
            );

            matrixController.showModeColor();

            matrixMessageUntil =
                now
                + std::chrono::milliseconds(
                    MatrixMessageDurationMs
                );

            buzzer.off();
            servo.home();
        }


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


        /*
         * Raw HumanState comes directly from distance.
         */
        const HumanState rawDetectedState =
            humanDetector.detect(
                distance
            );


        /*
         * Blue Pill mode softens all non-Alone states
         * to Nearby.
         *
         * Neutral and Red Pill retain the full state.
         */
        const HumanState detectedState =
            matrixController.applyMode(
                rawDetectedState
            );


        /*
         * Close lasting ten seconds becomes Emergency.
         */
        const HumanState humanState =
            resolveHumanState(
                detectedState,
                now,
                closeStart
            );


        const bool showingMatrixMessage =
            matrixMessageUntil
                .time_since_epoch()
                .count()
                != 0
            && now < matrixMessageUntil;


        if(showingMatrixMessage)
        {
            /*
             * 這 1.5 秒內固定顯示剛按下的模式顏色，
             * 不讓 HumanState 蓋過 LCD 或 RGB。
             */
            matrixController.showModeColor();
            buzzer.off();
            servo.home();
        }
        else
        {
            matrixMessageUntil =
                TimePoint{};

            matrixController.updateColor(
                humanState
            );
        }


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
            matrixController.mode(),
            distance,
            drama
        );


        if(!showingMatrixMessage)
        {
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
        }


        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                MainLoopDelayMs
            )
        );
    }

    return 0;
}