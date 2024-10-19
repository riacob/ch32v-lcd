/**
 * @file lcd.c
 * @author Riccardo Iacob (github.com/riacob)
 * @brief Implementation of ch32v-lcd for character displays
 * @version 0.1
 * @date 2024-10-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "lcd.h"
#include "assert.h"

const uint8_t LCDCONF_PINS_DATABITS[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7}; // DB[0:7]
const uint8_t LCDCONF_PINS_RS[] = {PA8};                                          // Data/Command select
const uint8_t LCDCONF_PINS_RW[] = {PA9};                                          // Read/Write select
const uint8_t LCDCONF_PINS_E[] = {PB13};                                          // Enable
const uint8_t LCDCONF_PINS_BL[] = {PA15};                                         // Backlight

#if defined(LCDCONF_TYPE_C_1602_GENERIC_8BIT) || defined(LCDCONF_TYPE_C_2004_GENERIC_8BIT)

#if defined(LCDCONF_TYPE_C_2004_GENERIC_8BIT) || defined(LCDCONF_TYPE_C_2004_GENERIC_4BIT)
const uint8_t LCDCONF_ROW_OFST[] = {0x00, 0x40, 0x14, 0x54};
const uint8_t LCDCONF_COLS = 20;
const uint8_t LCDCONF_ROWS = 4;
#elif defined(LCDCONF_TYPE_C_1602_GENERIC_8BIT) || defined(LCDCONF_TYPE_C_1602_GENERIC_8BIT)
const uint8_t LCDCONF_ROW_OFST[] = {0x00, 0x40};
const uint8_t LCDCONF_COLS = 16;
const uint8_t LCDCONF_ROWS = 2;
#endif

#define LCD_CMD_CLR 0b00000001
#define LCD_CMD_HOME 0b00000010
#define LCD_CMD_ENTRY 0b00000100
#define LCD_CMD_CTRL 0b00001000
#define LCD_CMD_SHIFT 0b00010000
#define LCD_CMD_FSET 0b00100000
#define LCD_CMD_SETCGADDR 0b01000000
#define LCD_CMD_SETDDADDR 0b10000000

#define LCD_FLG_ENTRY_LEFT 0b00000010
#define LCD_FLG_ENTRY_RIGHT 0
#define LCD_FLG_ENTRY_SHIFTDEC 0
#define LCD_FLG_ENTRY_SHIFTINC 0b00000001
#define LCD_FLG_CTRL_DSPON 0b00000100
#define LCD_FLG_CTRL_DSPOFF 0
#define LCD_FLG_CTRL_CURON 0b00000010
#define LCD_FLG_CTRL_CUROFF 0
#define LCD_FLG_CTRL_BLINKON 0b00000001
#define LCD_FLG_CTRL_BLINKOFF 0
#define LCD_FLG_SHIFT_DSP 0b00001000
#define LCD_FLG_SHIFT_CUR 0
#define LCD_FLG_SHIFT_R 0b00000001
#define LCD_FLG_SHIFT_L 0
#define LCD_FLG_FSET_8BIT 0b00010000
#define LCD_FLG_FSET_4BIT 0
#define LCD_FLG_FSET_2LINE 0b00001000
#define LCD_FLG_FSET_1LINE 0
#define LCD_FLG_FSET_FONT5x8 0
#define LCD_FLG_FSET_FONT5x10 0b00000100

void lcd_writeraw(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        funDigitalWrite(LCDCONF_PINS_DATABITS[i], ((byte >> i) & 1));
    }
}

void lcd_en()
{
    funDigitalWrite(LCDCONF_PINS_E[0], FUN_LOW);
    funDigitalWrite(LCDCONF_PINS_E[0], FUN_HIGH);
    Delay_Us(100);
    funDigitalWrite(LCDCONF_PINS_E[0], FUN_LOW);
    Delay_Us(1);
}

void lcd_writecommand(uint8_t cmd)
{
    funDigitalWrite(LCDCONF_PINS_RS[0], 0);
    funDigitalWrite(LCDCONF_PINS_RW[0], 0);
    lcd_writeraw(cmd);
    lcd_en();
}

void lcd_writedata(uint8_t dat)
{
    funDigitalWrite(LCDCONF_PINS_RS[0], 1);
    funDigitalWrite(LCDCONF_PINS_RW[0], 0);
    lcd_writeraw(dat);
    lcd_en();
}

void lcd_readram(uint8_t addr)
{
    funDigitalWrite(LCDCONF_PINS_RS[0], 1);
    funDigitalWrite(LCDCONF_PINS_RW[0], 1);
    lcd_en();
    // currently ignoring data
}

void lcd_init()
{
    /* Check configuration sanity */
    static_assert(sizeof(LCDCONF_PINS_RS) == 1, "");
    static_assert(sizeof(LCDCONF_PINS_RW) == 1, "");
    static_assert(sizeof(LCDCONF_PINS_E) == 1, "");
    static_assert(sizeof(LCDCONF_PINS_BL) == 1, "");
    static_assert(sizeof(LCDCONF_PINS_DATABITS) == 8, "");
    /* Configure GPIO */
    funDigitalWrite(LCDCONF_PINS_RW[0], 0);
    funDigitalWrite(LCDCONF_PINS_RS[0], 0);
    funDigitalWrite(LCDCONF_PINS_E[0], 0);
    funPinMode(LCDCONF_PINS_RS[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(LCDCONF_PINS_RW[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(LCDCONF_PINS_E[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(LCDCONF_PINS_BL[0], GPIO_CFGLR_OUT_10Mhz_PP);
    for (uint8_t i = 0; i < 8; i++)
    {
        funPinMode(LCDCONF_PINS_DATABITS[i], GPIO_CFGLR_OUT_10Mhz_PP);
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
    lcd_writedata(c); // Print char
    Delay_Us(50);
}

void lcd_setcursor(uint8_t col, uint8_t row)
{
    if (col > LCDCONF_COLS)
    {
        return;
    }
    if (row > LCDCONF_ROWS)
    {
        return;
    }
    lcd_writecommand(LCD_CMD_SETDDADDR | ((col + LCDCONF_ROW_OFST[row])));
    Delay_Us(50);
}

void lcd_poff()
{
    lcd_writecommand(LCD_CMD_CTRL | LCD_FLG_CTRL_DSPOFF | LCD_FLG_CTRL_CUROFF | LCD_FLG_CTRL_BLINKOFF);
}

void lcd_printstr(char *str)
{
    uint32_t i = 0;
    while (str[i] != '\0') {
        lcd_printchar(str[i++]);
    }
}

#endif