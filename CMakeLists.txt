cmake_minimum_required(VERSION 3.16)

project(DramaPi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(drama-pi
    src/main.cpp
    src/CommandRunner.cpp
    src/TemperatureMonitor.cpp
    src/DramaEngine.cpp
    src/CpuMonitor.cpp
    src/MoodEngine.cpp
    src/MemoryMonitor.cpp
    src/DiskMonitor.cpp
    src/UptimeMonitor.cpp
    src/EventDetector.cpp
    src/DramaLibrary.cpp
    src/ReminderLibrary.cpp
)

add_executable(led-blink
    src/led_blink.cpp
)

target_link_libraries(led-blink
    gpiodcxx
    gpiod
)

add_executable(mood-randomizer
    src/mood_randomizer.cpp
    src/Led.cpp
    src/LCD1602.cpp
)
