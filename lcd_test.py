import smbus
import time

I2C_ADDR = 0x27
LCD_WIDTH = 16

LCD_CHR = 1
LCD_CMD = 0

LCD_LINE_1 = 0x80
LCD_LINE_2 = 0xC0

ENABLE = 0b00000100
BACKLIGHT = 0x08

bus = smbus.SMBus(1)

def lcd_byte(bits, mode):
    high = mode | (bits & 0xF0) | BACKLIGHT
    low = mode | ((bits << 4) & 0xF0) | BACKLIGHT

    bus.write_byte(I2C_ADDR, high)
    lcd_toggle_enable(high)

    bus.write_byte(I2C_ADDR, low)
    lcd_toggle_enable(low)

def lcd_toggle_enable(bits):
    time.sleep(0.0005)
    bus.write_byte(I2C_ADDR, bits | ENABLE)
    time.sleep(0.0005)
    bus.write_byte(I2C_ADDR, bits & ~ENABLE)
    time.sleep(0.0005)

def lcd_init():
    lcd_byte(0x33, LCD_CMD)
    lcd_byte(0x32, LCD_CMD)
    lcd_byte(0x06, LCD_CMD)
    lcd_byte(0x0C, LCD_CMD)
    lcd_byte(0x28, LCD_CMD)
    lcd_byte(0x01, LCD_CMD)
    time.sleep(0.005)

def lcd_string(message, line):
    message = message.ljust(LCD_WIDTH, " ")
    lcd_byte(line, LCD_CMD)
    for char in message[:LCD_WIDTH]:
        lcd_byte(ord(char), LCD_CHR)

lcd_init()
lcd_string("DramaPi v0.1", LCD_LINE_1)
lcd_string("Judging...", LCD_LINE_2)
