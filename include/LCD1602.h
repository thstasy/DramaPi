#pragma once

#include <cstdint>
#include <string>

class LCD1602
{
public:
    explicit LCD1602(uint8_t address = 0x27);

    void init();
    void clear();
    void print(const std::string& line1, const std::string& line2);

private:
    int fd_;
    uint8_t address_;

    void writeByte(uint8_t data);
    void pulseEnable(uint8_t data);
    void send(uint8_t value, uint8_t mode);
    void command(uint8_t value);
    void character(char c);
    void printLine(const std::string& text, uint8_t lineAddress);
};