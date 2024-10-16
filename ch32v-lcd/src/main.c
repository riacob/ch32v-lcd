#include "ch32v003fun.h"
#include <stdio.h>
#include "lcd.h"

int main()
{
	SystemInit();

	// Enable GPIOs
	funGpioInitAll();

	funPinMode(PC13, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);

	while (1)
	{
		funDigitalWrite(PC13, 1);
		Delay_Ms(500);
		funDigitalWrite(PC13, 0);
		Delay_Ms(500);
	}
}
