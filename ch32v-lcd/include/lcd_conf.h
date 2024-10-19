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

/* Display category selection */
#define LCDCONF_CATGR_CHARACTER // Character only
//#define LCDCONF_CATGR_GRAPHICAL // Graphical

/* Display selection */
//#define LCDCONF_TYPE_G_12864_GENERIC 1 // 128x64px, graphical
//#define LCDCONF_TYPE_C_1602_GENERIC_4BIT 1 // 16x02c, character, 4 bit bus
//#define LCDCONF_TYPE_C_1602_GENERIC_8BIT 1 // 16x02c, character, 8 bit bus
#define LCDCONF_TYPE_C_2004_GENERIC_8BIT 1 // 20x04c, character, 8 bit bus
//#define LCDCONF_TYPE_C_1602_GENERIC_4BIT 1 // 16x02c, character, 4 bit bus
//#define LCDCONF_TYPE_C_0800_GENERIC_8BIT 1 // 08x00c, character, 8 bit bus
//#define LCDCONF_TYPE_C_0800_GENERIC_4BIT 1 // 08x00c, character, 4 bit bus

/* Font selection for single line displays */
//#define LCDCONF_FONT_5x11 1 // Default font is 5x8

/* Advanced settings */
#define LCDCONF_MAX_DATABITS 8 // Max data bus width
#define LCDCONF_MAX_RS 1       // Max number of RS lines
#define LCDCONF_MAX_RW 1       // Max number of RW lines
#define LCDCONF_MAX_E 1        // Max number of E lines
#define LCDCONF_MAX_BL 1       // Max number of backlight lines
#define LCDCONF_MAX_CS 2       // Max number of CS lines
#define LCDCONF_MAX_ROWS 4     // Max number of rows
 
#endif /* _LCD_CONF_H */