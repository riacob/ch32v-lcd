#include "ch32v003fun.h"
#include <stdio.h>
#include "lcd.h"

int main()
{
	SystemInit();
	funGpioInitAll();

	lcd_test();
}
