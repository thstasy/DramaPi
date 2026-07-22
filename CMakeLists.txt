cmake_minimum_required(VERSION 3.16)

project(DramaPi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    Threads
    REQUIRED
)


set(
    DRAMAPI_SOURCES

    src/main.cpp

    src/hardware/Buzzer.cpp
    src/hardware/DistanceSensor.cpp
    src/hardware/LCD1602.cpp
    src/hardware/Led.cpp
    src/hardware/PassiveBuzzer.cpp
    src/hardware/RGBLed.cpp
    src/hardware/Servo.cpp
    src/hardware/StatusLights.cpp

    src/drama/DramaEngine.cpp
    src/drama/DramaLibrary.cpp
    src/drama/MoodEngine.cpp
    src/drama/ReminderLibrary.cpp

    src/human/HumanDetector.cpp
    src/human/HumanInteraction.cpp

    src/system/CommandRunner.cpp
    src/system/CpuMonitor.cpp
    src/system/DiskMonitor.cpp
    src/system/EventDetector.cpp
    src/system/MemoryMonitor.cpp
    src/system/TemperatureMonitor.cpp
    src/system/UptimeMonitor.cpp
)


# ============================
# Main application
# ============================

add_executable(
    drama-pi

    ${DRAMAPI_SOURCES}
)

target_include_directories(
    drama-pi
    PRIVATE
    include
)

target_link_libraries(
    drama-pi
    PRIVATE
    gpiodcxx
    gpiod
    lgpio
    Threads::Threads
)


# ============================
# Tests
# ============================

add_executable(
    test_led

    tests/test_led.cpp
)

target_include_directories(
    test_led
    PRIVATE
    include
)

target_link_libraries(
    test_led
    PRIVATE
    gpiodcxx
    gpiod
)


add_executable(
    test_lcd

    tests/test_lcd.cpp
    src/hardware/LCD1602.cpp
)

target_include_directories(
    test_lcd
    PRIVATE
    include
)


add_executable(
    test_ultrasonic

    tests/test_ultrasonic.cpp
    src/hardware/DistanceSensor.cpp
)

target_include_directories(
    test_ultrasonic
    PRIVATE
    include
)

target_link_libraries(
    test_ultrasonic
    PRIVATE
    gpiodcxx
    gpiod
)


add_executable(
    test_passive_buzzer

    tests/test_passive_buzzer.cpp
    src/hardware/PassiveBuzzer.cpp
)

target_include_directories(
    test_passive_buzzer
    PRIVATE
    include
)

target_link_libraries(
    test_passive_buzzer
    PRIVATE
    gpiodcxx
    gpiod
    lgpio
)


add_executable(
    demo_mood

    tests/demo_mood.cpp
    src/hardware/Led.cpp
    src/hardware/LCD1602.cpp
    src/hardware/Servo.cpp
)

target_include_directories(
    demo_mood
    PRIVATE
    include
)

target_link_libraries(
    demo_mood
    PRIVATE
    gpiodcxx
    gpiod
    lgpio
    Threads::Threads
)


# ============================
# Tools
# ============================

add_executable(
    calibrate_servo

    tools/calibrate_servo.cpp
)

target_link_libraries(
    calibrate_servo
    PRIVATE
    lgpio
)
