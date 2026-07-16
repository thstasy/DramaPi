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
#include "PassiveBuzzer.h"

#include "HumanDetector.h"
#include "HumanState.h"
#include "HumanInteraction.h"
#include "Buzzer.h"
#include "StartupMelody.h"

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


    // Active buzzer: GPIO22 / Physical Pin 15
    Buzzer buzzer(22);
    buzzer.off();


    // Passive buzzer: GPIO19 / Physical Pin 35
    // lgpio version initializes inside the constructor.
    // Do NOT call musicBuzzer.init().
    PassiveBuzzer musicBuzzer(19);


    // Servo: GPIO18 / Physical Pin 12
    Servo servo;
    servo.init();


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
        120,
        2
    );

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

    int emergencyMessage = 0;     

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

            servo.home();

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

            servo.home();
            break;



        case HumanState::Approaching:

            lcd.print(
                "Approaching",
                "Please slow down"
            );


            greenLed.off();
            redLed.on();

            buzzer.off();

            servo.startScanning();
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


            servo.startScanning();
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

                    buzzer.alarm(
                        3000,
                        20
                    );

                    std::cout
                    << "\n🚨 EMERGENCY\n"
                  //  << "HUMAN HAS IGNORED ALL SOCIAL CUES.\n"
                  //  << "HR notified.\n"
                    << "HR not care.\n\n";


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


            servo.startScanning();
            buzzer.off();


            break;



        case HumanState::Emergency:


            // lcd.print(
            //     "EMERGENCY",
            //     "HR notified"
            // );
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

            emergencyMessage++;

            if(emergencyMessage >= 3)
            {
                emergencyMessage = 0;
            }



            redLed.on();
            greenLed.off();


            buzzer.on();
            servo.startScanning();



            break;

            }
            servo.update();

        /*
         * ============================
         * Independent reminder
         * ============================
         */

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
