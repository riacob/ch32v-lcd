#include "ch32v003fun.h"
#include <stdio.h>
#include "lcd.h"

int main()
{
	SystemInit();

	// Enable GPIOs
	funGpioInitAll();
	lcd_init();
	lcd_setcursor(0, 0);
	lcd_printstr("Hello World");
	lcd_setcursor(0, 1);
	lcd_printstr("Second line :)");
	lcd_setcursor(0, 2);
	lcd_printstr("Third line :)");
	lcd_setcursor(0, 3);
	lcd_printstr("Fourth line :)");

	// lcd_poff();

	while (1)
	{
		funDigitalWrite(PC13, 1);
		Delay_Ms(500);
		funDigitalWrite(PC13, 0);
		Delay_Ms(500);
	}
}
