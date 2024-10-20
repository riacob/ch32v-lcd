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
#include <stdio.h>

typedef struct
{
    uint8_t LCDCONF_PINS_DATABITS[8]; // DB[0:7]
    uint8_t LCDCONF_PINS_RS[1];       // Data/Command select
    uint8_t LCDCONF_PINS_RW[1];       // Read/Write select
    uint8_t LCDCONF_PINS_E[1];        // Enable
    uint8_t LCDCONF_PINS_BL[1];       // Backlight
    uint8_t LCDCONF_ROWS;             // Number of rows (or y pixels)
    uint8_t LCDCONF_COLS;             // Number of columns (or x pixels)
    uint32_t LCDCONF_INIT_DEL;        // Initialization delay, uS
    uint32_t LCDCONF_CMD_DEL;         // Delay after commands, uS
    uint32_t LCDCONF_DAT_DEL;         // Delay after data, uS
    uint32_t LCDCONF_PUL_DEL;         // Pulse duration, uS
#ifdef LCDCONF_CATGR_CHARACTER
    uint8_t LCDCONF_ROW_OFST[4]; // Memory offsets for each row
#endif
#ifdef LCDCONF_CATGR_GRAPHICAL
    uint8_t LCDCONF_PINS_CS[2];  // Column select
    uint8_t LCDCONF_PINS_RST[1]; // Reset
    uint8_t LCDCONF_POL_CS;      // Active polarity of CS signal
    uint8_t LCDCONF_POL_RST;     // Active polarity of RST signal
    uint8_t LCDCONF_POL_E;       // Active polarity of E signal
    uint8_t LCDCONF_POL_RS;      // Active polarity of RS signal
    uint8_t LCDCONF_POL_RW;      // Active polarity of RE signal
#endif
} lcd_conf_t;

void lcd_test(); // Run test function. Requires LCDCONF_TESTMODE in lcd_conf.h
void lcd_init();
void lcd_printchar(char c);
void lcd_pon();
void lcd_poff();
void lcd_printstr(char *str);
void lcd_setconf(lcd_conf_t *conf);
void lcd_clr();

#ifdef LCDCONF_CATGR_CHARACTER
void lcd_setcursor(uint8_t col, uint8_t row);
#endif

#ifdef LCDCONF_CATGR_GRAPHICAL
void lcd_draw_dot(uint8_t x, uint8_t y);  // draw to screen
void lcdb_draw_dot(uint8_t x, uint8_t y); // draw to buffer
void lcd_writebuffer();
#endif

#endif /* _LCD_H */