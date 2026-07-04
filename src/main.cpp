#include<array>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<memory>
#include<string>
#include "CommandRunner.h"
#include "TemperatureMonitor.h"
#include "DramaEngine.h"

int main()
{
    std::cout << "============================\n";
    std::cout << "🎭 Drama Pi\n";
    std::cout << "============================\n";

    TemperatureMonitor temperatureMonitor;
    DramaEngine dramaEngine;

    double temp = temperatureMonitor.getTemperature();

    std::cout << "CPU Temp: " << temp << " °C\n";
    std::cout << "Drama Pi: "
              << dramaEngine.commentOnTemperature(temp)
              << "\n";

    return 0;
}
