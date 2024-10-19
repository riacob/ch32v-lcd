/**
 * @file lcd.h
 * @author Riccardo Iacob (github.com/riacob)
 * @brief ch32v-lcd library
 * @details Unified library for monochrome graphic and character LCDs
 * @version 0.1
 * @date 2024-10-17
 *
 * @copyright Copyright (c) 2024, MIT license
 *
 */
#ifndef _LCD_H
#define _LCD_H

#include "lcd_conf.h"

typedef struct
{
    uint8_t LCDCONF_PINS_DATABITS[8]; // DB[0:7]
    uint8_t LCDCONF_PINS_RS[1];       // Data/Command select
    uint8_t LCDCONF_PINS_RW[1];       // Read/Write select
    uint8_t LCDCONF_PINS_E[1];        // Enable
    uint8_t LCDCONF_PINS_BL[1];       // Backlight
    uint8_t LCDCONF_PINS_CS[2];       // Chip select
    uint8_t LCDCONF_ROWS;             // Number of rows
    uint8_t LCDCONF_COLS;             // Number of columns
    uint8_t LCDCONF_ROW_OFST[4];   // Memory offsets for each row
} lcd_conf_t;

void lcd_init();
void lcd_printchar(char c);
void lcd_setcursor(uint8_t col, uint8_t row);
void lcd_poff();
void lcd_printstr(char *str);
void lcd_setconf(lcd_conf_t *conf);

#endif /* _LCD_H */