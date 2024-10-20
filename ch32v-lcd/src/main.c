#include "ch32v003fun.h"
#include <stdio.h>
#include "lcd.h"



int main()
{
	SystemInit();
	funGpioInitAll();

	Delay_Ms(3000);
	printf("LCD TEST START\n");
	lcd_test();
	printf("LCD TEST END\n");
}

