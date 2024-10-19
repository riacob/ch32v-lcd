#include "ch32v003fun.h"
#include <stdio.h>
#include "lcd.h"

int main()
{
	SystemInit();
	funGpioInitAll();

	lcd_conf_t lcd_cfg = {
		.LCDCONF_PINS_DATABITS = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7},
		.LCDCONF_PINS_RS = {PA8},
		.LCDCONF_PINS_RW = {PA9},
		.LCDCONF_PINS_E = {PB13},
		.LCDCONF_PINS_BL = {PA15},
		.LCDCONF_ROWS = 4,
		.LCDCONF_COLS = 20,
		.LCDCONF_ROW_OFST = {0x00, 0x40, 0x14, 0x54}
	};
	lcd_setconf(&lcd_cfg);
	lcd_init();
	lcd_setcursor(0, 0);
	lcd_printstr("Hello World");
	lcd_setcursor(0, 1);
	lcd_printstr("Second line :)");
	lcd_setcursor(0, 2);
	lcd_printstr("Third line :)");
	lcd_setcursor(0, 3);
	lcd_printstr("Fourth line :)");

}
