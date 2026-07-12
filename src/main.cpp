#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "CpuMonitor.h"
#include "DiskMonitor.h"
#include "DramaEngine.h"
#include "EventDetector.h"
#include "MemoryMonitor.h"
#include "SystemStatus.h"
#include "TemperatureMonitor.h"
#include "UptimeMonitor.h"
#include "Personality.h"
#include "ReminderLibrary.h"

#include "DistanceSensor.h"
#include "LCD1602.h"
#include "Led.h"
#include "Servo.h"

#include "HumanDetector.h"
#include "HumanState.h"
#include "HumanInteraction.h"
#include "Buzzer.h"


Personality parsePersonality(const std::string& name)
{
    if (name == "dramatic")
        return Personality::Dramatic;

    if (name == "corporate")
        return Personality::Corporate;

    if (name == "emo")
        return Personality::Emo;

    if (name == "sarcastic")
        return Personality::Sarcastic;

    if (name == "existential")
        return Personality::Existential;

    return Personality::Dramatic;
}


std::string personalityName(Personality personality)
{
    switch (personality)
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



int main(int argc, char* argv[])
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
        7,   // TRIG GPIO
        8    // ECHO GPIO
    );


    LCD1602 lcd(0x27);
    lcd.init();


    Led redLed(27);
    Led greenLed(17);

    Buzzer buzzer(22);

    Servo servo;
    servo.init();



    Personality personality =
        Personality::Dramatic;


    if(argc > 1)
    {
        personality =
            parsePersonality(argv[1]);
    }



    auto lastReminder =
        std::chrono::steady_clock::now();

    auto closeStart =
        std::chrono::steady_clock::time_point{};

    while(true)
    {

        /*
         * ============================
         * System monitoring
         * ============================
         */

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



        /*
         * ============================
         * Human detection
         * ============================
         */


        float distance =
            distanceSensor.distance();



        DramaEvent event =
            eventDetector.detect(status);
        
            

        HumanState humanState =
            humanDetector.detect(distance);

        humanInteraction.update(humanState);

        if(humanState == HumanState::Emergency)
        {
            event = DramaEvent::HumanTooClose;
        }
        else if(humanState != HumanState::Alone)
        {
            event = DramaEvent::HumanDetected;
        }        

        std::string drama =
            dramaEngine.comment(
                event,
                personality
            );

        /*
         * ============================
         * Terminal output
         * ============================
         */

        std::cout
            << "============================\n";

        std::cout
            << "🎭 Drama Pi\n";

        std::cout
            << "============================\n";


        std::cout
            << "Personality: "
            << personalityName(personality)
            << "\n";


        std::cout
            << "CPU Temp: "
            << status.cpuTemperature
            << " °C\n";


        std::cout
            << "CPU Usage: "
            << status.cpuUsage
            << " %\n";


        std::cout
            << "Memory Usage: "
            << status.memoryUsage
            << " %\n";


        std::cout
            << "Disk Usage: "
            << status.diskUsage
            << " %\n";


        std::cout
            << "Human Distance: "
            << distance
            << " cm\n";


        std::cout
            << "Drama Pi: "
            << drama
            << "\n";


        /*
         * ============================
         * Physical reaction
         * ============================
         */

/*
        switch(event)
        {

            case DramaEvent::HumanTooClose:

                lcd.print(
                    "PERSONAL SPACE",
                    "NO HARASSMENT"
                );


                redLed.on();
                // rgbLed.red();
                buzzer.off();
                servo.lookLeft();

                break;



            case DramaEvent::HumanDetected:


                lcd.print(
                    "Human detected",
                    "I see you..."
                );


                redLed.on();
                // rgbLed.yellow();
                buzzer.off();
                servo.lookRight();

                break;



            default:


                lcd.print(
                    "Drama Pi",
                    "Watching..."
                );


                redLed.off();
                greenLed.on();
                // rgbLed.green();
                buzzer.off();
                servo.lookCenter();

                break;
        }

*/

/*
 * ============================
 * Physical reaction
 * ============================
 */

        auto now =
            std::chrono::steady_clock::now();


        switch(humanState)
        {

        case HumanState::Alone:

            lcd.print(
                "Drama Pi",
                "Watching..."
            );

            greenLed.on();
            redLed.off();

            buzzer.off();

            servo.lookCenter();

            closeStart =
                std::chrono::steady_clock::time_point{};

            break;



        case HumanState::Nearby:

            lcd.print(
                "Human detected",
                "I see you..."
            );

            greenLed.on();
            redLed.on();

            buzzer.off();

            servo.lookRight();

            break;



        case HumanState::Approaching:

            lcd.print(
                "Approaching",
                "Please slow down"
            );


            greenLed.off();
            redLed.on();

            buzzer.off();

            servo.lookLeft();


            if(closeStart.time_since_epoch().count()==0)
            {
                closeStart = now;
            }

            break;



        case HumanState::Close:

            lcd.print(
                "PERSONAL SPACE",
                "BACK AWAY"
            );


            greenLed.off();
            redLed.on();


            servo.lookLeft();


            if(closeStart.time_since_epoch().count()==0)
            {
                closeStart = now;
            }


            {
                auto seconds =
                    std::chrono::duration_cast<
                        std::chrono::seconds
                    >
                    (now-closeStart)
                    .count();


                if(seconds >= 10)
                {

                    lcd.print(
                        "EMERGENCY",
                        "HR notified"
                    );


                    buzzer.on();


                    std::cout
                    << "\n🚨 EMERGENCY\n"
                    << "HUMAN HAS IGNORED ALL SOCIAL CUES.\n"
                    << "HR notified.\n"
                    << "HR did not care.\n\n";


                }
                else
                {
                    buzzer.off();
                }

            }


            break;



        case HumanState::PersonalSpace:


            lcd.print(
                "PERSONAL SPACE",
                "NO HARASSMENT"
            );


            redLed.on();
            greenLed.off();


            servo.lookLeft();


            buzzer.off();


            break;



        case HumanState::Emergency:


            lcd.print(
                "EMERGENCY",
                "HR notified"
            );


            redLed.on();
            greenLed.off();


            buzzer.on();


            servo.lookLeft();


            break;

        }

        /*
         * ============================
         * Independent reminder
         * ============================
         */


        // auto now =
        //     std::chrono::steady_clock::now();


        // auto minutes =
        //     std::chrono::duration_cast<
        //         std::chrono::minutes
        //     >(now - lastReminder)
        //     .count();

        auto minutes =
            std::chrono::duration_cast<
                std::chrono::minutes
            >(now - lastReminder)
            .count();


        if(minutes >= 5)
        {

            std::string reminder =
                reminderLibrary.reminder(
                    personality
                );


            std::cout
                << "⏰ Reminder: "
                << reminder
                << "\n\n";


            lcd.print(
                "REMINDER",
                reminder.substr(0,16)
            );


            lastReminder = now;
        }



        /*
         * ============================
         * Fast reaction loop
         * ============================
         */


        std::this_thread::sleep_for(
            std::chrono::milliseconds(300)
        );

    }


    return 0;
}
