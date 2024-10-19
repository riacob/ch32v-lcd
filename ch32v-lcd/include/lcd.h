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

/* Common functions */
void lcd_init();
void lcd_printchar(char c);
void lcd_setcursor(uint8_t col, uint8_t row);
void lcd_poff();
void lcd_printstr(char *str);

/* Character LCD functions */


/* Graphical LCD functions */

#endif /* _LCD_H */