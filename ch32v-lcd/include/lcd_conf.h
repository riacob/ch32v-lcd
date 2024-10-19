/**
 * @file lcd_conf.h
 * @author Riccardo Iacob (github.com/riacob)
 * @brief Configuration file for ch32v-lcd
 * @version 0.1
 * @date 2024-10-17
 * 
 * @copyright Copyright (c) 2024, MIT license
 * 
 */
#ifndef _LCD_CONF_H
#define _LCD_CONF_H

#include "ch32v003fun.h"

/* Display Selection */
//#define LCDCONF_TYPE_G_12864_GENERIC 1 // 128x64px, graphical
//#define LCDCONF_TYPE_C_1602_GENERIC_4BIT 1 // 16x02c, character, 4 bit bus
//#define LCDCONF_TYPE_C_1602_GENERIC_8BIT 1 // 16x02c, character, 8 bit bus
#define LCDCONF_TYPE_C_2004_GENERIC_8BIT 1 // 20x04c, character, 8 bit bus
//#define LCDCONF_TYPE_C_1602_GENERIC_4BIT 1 // 16x02c, character, 4 bit bus
//#define LCDCONF_TYPE_C_0800_GENERIC_8BIT 1 // 08x00c, character, 8 bit bus
//#define LCDCONF_TYPE_C_0800_GENERIC_4BIT 1 // 08x00c, character, 4 bit bus

/* Font Selection for single line displays */
//#define LCDCONF_FONT_5x11 1 // Default font is 5x8

/* Pin Definitions in C file */
/*const uint8_t LCDCONF_PINS_DATABITS[]={PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7}; // DB[0:7]
const uint8_t LCDCONF_PINS_RS[]={PA8}; // Data/Command select
const uint8_t LCDCONF_PINS_RW[]={PA9}; // Read/Write select
const uint8_t LCDCONF_PINS_E[]={PA10}; // Enable
const uint8_t LCDCONF_PINS_BL[]={PA15}; // Backlight*/
 
#endif /* _LCD_CONF_H */