cmake_minimum_required(VERSION 3.16)

project(DramaPi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)


# ============================
# Main DramaPi application
# ============================

add_executable(drama-pi
    src/main.cpp

    src/CommandRunner.cpp

    src/TemperatureMonitor.cpp
    src/CpuMonitor.cpp
    src/MemoryMonitor.cpp
    src/DiskMonitor.cpp
    src/UptimeMonitor.cpp

    src/DramaEngine.cpp
    src/DramaLibrary.cpp
    src/ReminderLibrary.cpp
    src/MoodEngine.cpp

    src/EventDetector.cpp

    # Sensors
    src/DistanceSensor.cpp

    # Hardware reactions
    src/LCD1602.cpp
    src/Led.cpp
    src/Servo.cpp
    src/PassiveBuzzer.cpp

    src/HumanDetector.cpp
    src/HumanInteraction.cpp
    src/RGBLed.cpp
    src/Buzzer.cpp
)


target_link_libraries(drama-pi
    gpiodcxx
    gpiod
    lgpio
)



# ============================
# LED test
# ============================

add_executable(led-blink
    src/led_blink.cpp
)


target_link_libraries(led-blink
    gpiodcxx
    gpiod
)

add_executable(lcd-test
    src/lcd_test.cpp
    src/LCD1602.cpp
)



# ============================
# Mood randomizer
# ============================

add_executable(mood-randomizer
    src/mood_randomizer.cpp
    src/Led.cpp
    src/LCD1602.cpp
    src/Servo.cpp
)


target_link_libraries(mood-randomizer
    gpiodcxx
    gpiod
)



# ============================
# Ultrasonic test
# ============================

add_executable(ultrasonic-test
    src/ultrasonic_test.cpp
    src/DistanceSensor.cpp
)


target_link_libraries(ultrasonic-test
    gpiodcxx
    gpiod
)

add_executable(passive-buzzer-test
    src/passive_buzzer_test.cpp
    src/PassiveBuzzer.cpp
)
target_link_libraries(passive-buzzer-test
    gpiodcxx
    gpiod
    lgpio
)

add_executable(lgpio-buzzer-test
    src/lgpio_buzzer_test.cpp
)

target_link_libraries(lgpio-buzzer-test
    lgpio
)