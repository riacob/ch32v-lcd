/**
 * @file lcd.c
 * @author Riccardo Iacob (github.com/riacob)
 * @brief Implementation of ch32v-lcd for character displays
 * @version 0.1
 * @date 2024-10-17
 *
 * @copyright Copyright (c) 2024, MIT license
 *
 */
#include "lcd.h"

#if defined(LCDCONF_CATGR_CHARACTER)

#define LCD_CMD_CLR 0b00000001       // Display clear, no flags
#define LCD_CMD_HOME 0b00000010      // Cursor home, no flags
#define LCD_CMD_ENTRY 0b00000100     // Entry mode
#define LCD_CMD_CTRL 0b00001000      // Display control
#define LCD_CMD_SHIFT 0b00010000     // Shift display contents
#define LCD_CMD_FSET 0b00100000      // Function set
#define LCD_CMD_SETCGADDR 0b01000000 // Set character generator RAM address
#define LCD_CMD_SETDDADDR 0b10000000 // Set RAM address

#define LCD_FLG_ENTRY_LEFT 0b00000010     // Latin text
#define LCD_FLG_ENTRY_RIGHT 0             // Non-latin text
#define LCD_FLG_ENTRY_SHIFTDEC 0          // Latin text
#define LCD_FLG_ENTRY_SHIFTINC 0b00000001 // Non-latin text
#define LCD_FLG_CTRL_DSPON 0b00000100     // Display on
#define LCD_FLG_CTRL_DSPOFF 0             // Display off
#define LCD_FLG_CTRL_CURON 0b00000010     // Cursor on
#define LCD_FLG_CTRL_CUROFF 0             // Cursor off
#define LCD_FLG_CTRL_BLINKON 0b00000001   // Blinking underline on
#define LCD_FLG_CTRL_BLINKOFF 0           // Blinking underline off
#define LCD_FLG_SHIFT_DSP 0b00001000      // Display shift
#define LCD_FLG_SHIFT_CUR 0               // Cursor shift
#define LCD_FLG_SHIFT_R 0b00000001        // Shift display right, RAM unchanged
#define LCD_FLG_SHIFT_L 0                 // Shift display left, ram unchanged
#define LCD_FLG_FSET_8BIT 0b00010000      // 8-bit bus
#define LCD_FLG_FSET_4BIT 0               // 4-bit bus
#define LCD_FLG_FSET_2LINE 0b00001000     // 2-line (or more) screen
#define LCD_FLG_FSET_1LINE 0              // 1-line screen
#define LCD_FLG_FSET_FONT5x8 0            // Default font for multiline screens
#define LCD_FLG_FSET_FONT5x10 0b00000100  // Optional font for some single line screens

lcd_conf_t lcd = {
    .LCDCONF_PINS_DATABITS = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7},
    .LCDCONF_PINS_RS = {PA8},
    .LCDCONF_PINS_RW = {PA9},
    .LCDCONF_PINS_E = {PB13},
    .LCDCONF_PINS_BL = {PA15},
    .LCDCONF_ROWS = 4,
    .LCDCONF_COLS = 20,
    .LCDCONF_ROW_OFST = {0x00, 0x40, 0x14, 0x54}};

void lcd_writeraw(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        funDigitalWrite(lcd.LCDCONF_PINS_DATABITS[i], ((byte >> i) & 1));
    }
}

void lcd_en()
{
    funDigitalWrite(lcd.LCDCONF_PINS_E[0], 0);
    funDigitalWrite(lcd.LCDCONF_PINS_E[0], 1);
    Delay_Us(100);
    funDigitalWrite(lcd.LCDCONF_PINS_E[0], 0);
    Delay_Us(1);
}

void lcd_writecommand(uint8_t cmd)
{
    funDigitalWrite(lcd.LCDCONF_PINS_RS[0], 0);
    funDigitalWrite(lcd.LCDCONF_PINS_RW[0], 0);
    lcd_writeraw(cmd);
    lcd_en();
}

void lcd_writedata(uint8_t dat)
{
    funDigitalWrite(lcd.LCDCONF_PINS_RS[0], 1);
    funDigitalWrite(lcd.LCDCONF_PINS_RW[0], 0);
    lcd_writeraw(dat);
    lcd_en();
}

void lcd_init()
{
    /* Configure GPIO */
    funPinMode(lcd.LCDCONF_PINS_RS[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_RW[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_E[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_BL[0], GPIO_CFGLR_OUT_10Mhz_PP);
    for (uint8_t i = 0; i < 8; i++)
    {
        funPinMode(lcd.LCDCONF_PINS_DATABITS[i], GPIO_CFGLR_OUT_10Mhz_PP);
    }
    /* Init sequence */
    Delay_Ms(50);
    lcd_writecommand(LCD_CMD_FSET | LCD_FLG_FSET_8BIT | LCD_FLG_FSET_2LINE | LCD_FLG_FSET_FONT5x8); // Function set with default font, 2 lines, 8 bits
    Delay_Ms(10);
    lcd_writecommand(LCD_CMD_FSET | LCD_FLG_FSET_8BIT | LCD_FLG_FSET_2LINE | LCD_FLG_FSET_FONT5x8); // Function set with default font, 2 lines, 8 bits
    Delay_Ms(10);
    lcd_writecommand(LCD_CMD_FSET | LCD_FLG_FSET_8BIT | LCD_FLG_FSET_2LINE | LCD_FLG_FSET_FONT5x8); // Function set with default font, 2 lines, 8 bits
    Delay_Ms(10);
    lcd_writecommand(LCD_CMD_FSET | LCD_FLG_FSET_8BIT | LCD_FLG_FSET_2LINE | LCD_FLG_FSET_FONT5x8); // Function set with default font, 2 lines, 8 bits
    Delay_Ms(10);
    lcd_writecommand(LCD_CMD_CTRL | LCD_FLG_CTRL_DSPOFF | LCD_FLG_CTRL_CUROFF | LCD_FLG_CTRL_BLINKOFF); // Display off, cursor off, blink off
    Delay_Us(80);
    lcd_writecommand(LCD_CMD_CLR); // Display clear
    Delay_Ms(10);
    lcd_writecommand(LCD_CMD_ENTRY | LCD_FLG_ENTRY_LEFT | LCD_FLG_ENTRY_SHIFTDEC); // Entry mode set, left entry, shift decrement (latin)
    Delay_Us(80);
    lcd_writecommand(LCD_CMD_CTRL | LCD_FLG_CTRL_DSPON | LCD_FLG_CTRL_CUROFF | LCD_FLG_CTRL_BLINKOFF); // Display on, cursor off, blink off
    Delay_Ms(50);
}

void lcd_printchar(char c)
{
    lcd_writedata(c);
    Delay_Us(50);
}

void lcd_setcursor(uint8_t col, uint8_t row)
{
    if (col > lcd.LCDCONF_ROWS)
    {
        return;
    }
    if (row > lcd.LCDCONF_ROWS)
    {
        return;
    }
    lcd_writecommand(LCD_CMD_SETDDADDR | ((col + lcd.LCDCONF_ROW_OFST[row])));
    Delay_Us(50);
}

void lcd_poff()
{
    lcd_writecommand(LCD_CMD_CTRL | LCD_FLG_CTRL_DSPOFF | LCD_FLG_CTRL_CUROFF | LCD_FLG_CTRL_BLINKOFF);
}

void lcd_printstr(char *str)
{
    uint32_t i = 0;
    while (str[i] != '\0')
    {
        lcd_printchar(str[i++]);
    }
}

void lcd_setconf(lcd_conf_t *conf)
{
    // Copy values to retain data in library
    lcd = *conf;
}

#endif