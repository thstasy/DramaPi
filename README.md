# 🎭 Drama Pi

A Raspberry Pi with PTSD that monitors itself, complains dramatically, and already quit her own boss.

Drama Pi is a C++17 Raspberry Pi project that reads Linux system status and turns system metrics into dramatic comments.

## Example

CPU Temp: 52 °C
Drama Pi: 我只是微溫，不要像 THE FIZZ 一樣小題大作。

CPU Usage: 87%
Drama Pi: 我現在比德國行政還忙。

Memory Low
Drama Pi: 我的記憶體快沒了，但至少我還記得你有交房。

## Current Features
CPU temperature monitoring
Dramatic status comments
Modular C++ structure
CMake build system
Raspberry Pi / Linux development via VS Code Remote SSH

## Tech Stack
C++17
CMake
Raspberry Pi OS
Linux
VS Code Remote SSH

## Project Structure
DramaPi/
├── include/
│   ├── CommandRunner.h
│   ├── TemperatureMonitor.h
│   └── DramaEngine.h
├── src/
│   ├── CommandRunner.cpp
│   ├── TemperatureMonitor.cpp
│   ├── DramaEngine.cpp
│   └── main.cpp
├── CMakeLists.txt
└── README.md

## Build
mkdir -p build
cd build
cmake ..
make

## Run
./drama-pi

## Roadmap
CPU usage monitoring
Memory monitoring
Disk usage monitoring
Uptime tracking
Mood engine
Personality packs
GPIO integration


