#include "LCD1602.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;

namespace
{
constexpr uint8_t LCD_CHR = 1;
constexpr uint8_t LCD_CMD = 0;
constexpr uint8_t LCD_LINE_1 = 0x80;
constexpr uint8_t LCD_LINE_2 = 0xC0;
constexpr uint8_t ENABLE = 0b00000100;
constexpr uint8_t BACKLIGHT = 0x08;
constexpr int LCD_WIDTH = 16;
}

LCD1602::LCD1602(uint8_t address)
    : fd_(-1), address_(address)
{
    fd_ = open("/dev/i2c-1", O_RDWR);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to open /dev/i2c-1");
    }

    if (ioctl(fd_, I2C_SLAVE, address_) < 0) {
        throw std::runtime_error("Failed to connect to LCD1602");
    }
}

void LCD1602::writeByte(uint8_t data)
{
    if (write(fd_, &data, 1) != 1) {
        throw std::runtime_error("Failed to write to LCD1602");
    }
}

void LCD1602::pulseEnable(uint8_t data)
{
    std::this_thread::sleep_for(500us);
    writeByte(data | ENABLE);
    std::this_thread::sleep_for(500us);
    writeByte(data & ~ENABLE);
    std::this_thread::sleep_for(500us);
}

void LCD1602::send(uint8_t value, uint8_t mode)
{
    uint8_t high = mode | (value & 0xF0) | BACKLIGHT;
    uint8_t low  = mode | ((value << 4) & 0xF0) | BACKLIGHT;

    writeByte(high);
    pulseEnable(high);

    writeByte(low);
    pulseEnable(low);
}

void LCD1602::command(uint8_t value)
{
    send(value, LCD_CMD);
}

void LCD1602::character(char c)
{
    send(static_cast<uint8_t>(c), LCD_CHR);
}

void LCD1602::init()
{
    command(0x33);
    command(0x32);
    command(0x06);
    command(0x0C);
    command(0x28);
    clear();
}

void LCD1602::clear()
{
    command(0x01);
    std::this_thread::sleep_for(5ms);
}

void LCD1602::printLine(const std::string& text, uint8_t lineAddress)
{
    command(lineAddress);

    std::string padded = text.substr(0, LCD_WIDTH);
    padded.resize(LCD_WIDTH, ' ');

    for (char c : padded) {
        character(c);
    }
}

void LCD1602::print(const std::string& line1, const std::string& line2)
{
    printLine(line1, LCD_LINE_1);
    printLine(line2, LCD_LINE_2);
}