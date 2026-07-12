#include "LCD1602.h"
#include <iostream>


int main()
{

    LCD1602 lcd(0x27);

    lcd.init();

    lcd.print(
        "Drama Pi",
        "HELLO"
    );


    while(true){}

}