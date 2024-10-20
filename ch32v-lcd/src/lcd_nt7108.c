/**
 * @file lcd_graph.c
 * @author Riccardo Iacob (github.com/riacob)
 * @brief
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "lcd.h"

#ifdef LCDCONF_CATGR_GRAPHICAL

#define LCD_CMD_PWR 0b00111110      // Power control
#define LCD_CMD_SETYADDR 0b01000000 // Set y address (addr 0-63)
#define LCD_CMD_SETXADDR 0b10111000 // Set x address (page 0-7)
#define LCD_CMD_SETZADDR 0b11000000 // Set z address (display start line 0-63)

#define LCD_FLG_PWR_ON 0b00000001 // Power on
#define LCD_FLG_PWR_OFF 0         // Power off

#if defined(LCDCONF_TYPE_G_WG12864A_8BIT)
lcd_conf_t lcd = {
    .LCDCONF_PINS_DATABITS = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7},
    .LCDCONF_PINS_RS = {PA8},
    .LCDCONF_PINS_RW = {PA9},
    .LCDCONF_PINS_E = {PB13},
    .LCDCONF_PINS_BL = {PB6},
    .LCDCONF_ROWS = 64,
    .LCDCONF_COLS = 128,
    .LCDCONF_PINS_CS = {PB12, PB14},
    .LCDCONF_PINS_RST = {PA15},
    .LCDCONF_INIT_DEL = 10000,
    .LCDCONF_CMD_DEL = 50,
    .LCDCONF_DAT_DEL = 50,
    .LCDCONF_PUL_DEL = 5,
    .LCDCONF_POL_CS = 1,
    .LCDCONF_POL_RST = 0,
    .LCDCONF_POL_E = 1,
    .LCDCONF_POL_RS = 1,
    .LCDCONF_POL_RW = 1};
#define LCD_CS_NONE 0
#define LCD_CS1 1
#define LCD_CS2 2
#define LCD_CS_BOTH 5
#else
lec_conf_t lcd = {0};
#warn "Selected display module has no default configuration. Use lcd_setconf() before calling lcd_init(), otherwise the display won't work. This warning is permament."
#endif

uint8_t **lcd_buffer = NULL;

void lcd_writeraw(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        funDigitalWrite(lcd.LCDCONF_PINS_DATABITS[i], ((byte >> i) & 1));
    }
}

void lcd_en()
{
    funDigitalWrite(lcd.LCDCONF_PINS_E[0], !lcd.LCDCONF_POL_E);
    funDigitalWrite(lcd.LCDCONF_PINS_E[0], lcd.LCDCONF_POL_E);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
    funDigitalWrite(lcd.LCDCONF_PINS_E[0], !lcd.LCDCONF_POL_E);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
}

void lcd_rst()
{
    funDigitalWrite(lcd.LCDCONF_PINS_RST[0], !lcd.LCDCONF_POL_RST);
    funDigitalWrite(lcd.LCDCONF_PINS_RST[0], lcd.LCDCONF_POL_RST);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
    funDigitalWrite(lcd.LCDCONF_PINS_RST[0], !lcd.LCDCONF_POL_RST);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
}

void lcd_cs1()
{
    funDigitalWrite(lcd.LCDCONF_PINS_CS[0], lcd.LCDCONF_POL_CS);
    funDigitalWrite(lcd.LCDCONF_PINS_CS[1], !lcd.LCDCONF_POL_CS);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
}

void lcd_cs2()
{
    funDigitalWrite(lcd.LCDCONF_PINS_CS[0], !lcd.LCDCONF_POL_CS);
    funDigitalWrite(lcd.LCDCONF_PINS_CS[1], lcd.LCDCONF_POL_CS);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
}

void lcd_csnone()
{
    funDigitalWrite(lcd.LCDCONF_PINS_CS[0], !lcd.LCDCONF_POL_CS);
    funDigitalWrite(lcd.LCDCONF_PINS_CS[1], !lcd.LCDCONF_POL_CS);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
}

void lcd_csboth()
{
    funDigitalWrite(lcd.LCDCONF_PINS_CS[0], lcd.LCDCONF_POL_CS);
    funDigitalWrite(lcd.LCDCONF_PINS_CS[1], lcd.LCDCONF_POL_CS);
    Delay_Us(lcd.LCDCONF_PUL_DEL);
}

void lcd_writecommand(uint8_t cmd, uint8_t cs)
{
    funDigitalWrite(lcd.LCDCONF_PINS_RS[0], !lcd.LCDCONF_POL_RS);
    funDigitalWrite(lcd.LCDCONF_PINS_RW[0], !lcd.LCDCONF_POL_RW);
    switch (cs)
    {
    case LCD_CS_BOTH:
    {
        lcd_csboth();
        break;
    }
    case LCD_CS1:
    {
        lcd_cs1();
        break;
    }
    case LCD_CS2:
    {
        lcd_cs2();
        break;
    }
    default:
    {
        return;
    }
    }
    lcd_writeraw(cmd);
    lcd_en();
    lcd_csnone();
    Delay_Us(lcd.LCDCONF_CMD_DEL);
}

void lcd_writedata(uint8_t dat, uint8_t cs)
{
    funDigitalWrite(lcd.LCDCONF_PINS_RS[0], lcd.LCDCONF_POL_RS);
    funDigitalWrite(lcd.LCDCONF_PINS_RW[0], !lcd.LCDCONF_POL_RW);
    switch (cs)
    {
    case LCD_CS_BOTH:
    {
        lcd_csboth();
        break;
    }
    case LCD_CS1:
    {
        lcd_cs1();
        break;
    }
    case LCD_CS2:
    {
        lcd_cs2();
        break;
    }
    default:
    {
        return;
    }
    }
    lcd_writeraw(dat);
    lcd_en();
    lcd_csnone();
    Delay_Us(lcd.LCDCONF_DAT_DEL);
}

void lcd_readflg()
{
    uint8_t bsy, on, rst;
    funDigitalWrite(lcd.LCDCONF_PINS_RS[0], !lcd.LCDCONF_POL_RS);
    funDigitalWrite(lcd.LCDCONF_PINS_RW[0], lcd.LCDCONF_POL_RW);
    funPinMode(lcd.LCDCONF_PINS_DATABITS[7], GPIO_CFGLR_IN_FLOAT);
    funPinMode(lcd.LCDCONF_PINS_DATABITS[5], GPIO_CFGLR_IN_FLOAT);
    funPinMode(lcd.LCDCONF_PINS_DATABITS[4], GPIO_CFGLR_IN_FLOAT);
    lcd_cs1();
    lcd_en();
    bsy = funDigitalRead(lcd.LCDCONF_PINS_DATABITS[7]);
    on = funDigitalRead(lcd.LCDCONF_PINS_DATABITS[5]);
    rst = funDigitalRead(lcd.LCDCONF_PINS_DATABITS[4]);
    lcd_csnone();
    printf("LCD STATUS CS1: %s, %s, %s\n", bsy ? "BUSY" : "READY", on ? "OFF" : "ON", rst ? "RESET" : "NORMAL");
    lcd_cs2();
    lcd_en();
    bsy = funDigitalRead(lcd.LCDCONF_PINS_DATABITS[7]);
    on = funDigitalRead(lcd.LCDCONF_PINS_DATABITS[5]);
    rst = funDigitalRead(lcd.LCDCONF_PINS_DATABITS[4]);
    lcd_csnone();
    printf("LCD STATUS CS2: %s, %s, %s\n", bsy ? "BUSY" : "READY", on ? "OFF" : "ON", rst ? "RESET" : "NORMAL");
    funPinMode(lcd.LCDCONF_PINS_DATABITS[7], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_DATABITS[5], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_DATABITS[4], GPIO_CFGLR_OUT_10Mhz_PP);
}

void lcd_init()
{
    /* Configure GPIO */
    funPinMode(lcd.LCDCONF_PINS_RS[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_RW[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_E[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_BL[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_RST[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_CS[0], GPIO_CFGLR_OUT_10Mhz_PP);
    funPinMode(lcd.LCDCONF_PINS_CS[1], GPIO_CFGLR_OUT_10Mhz_PP);
    for (uint8_t i = 0; i < 8; i++)
    {
        funPinMode(lcd.LCDCONF_PINS_DATABITS[i], GPIO_CFGLR_OUT_10Mhz_PP);
    }
    /* Init sequence */
    Delay_Ms(lcd.LCDCONF_INIT_DEL);
    lcd_rst();
    lcd_readflg();
    lcd_writecommand(LCD_CMD_SETZADDR, LCD_CS_BOTH); // Set start line to 0
    lcd_writecommand(LCD_CMD_SETXADDR, LCD_CS_BOTH);
    lcd_writecommand(LCD_CMD_SETYADDR, LCD_CS_BOTH);
    lcd_writecommand(LCD_CMD_PWR | LCD_FLG_PWR_ON, LCD_CS_BOTH); // Turn on display
    lcd_readflg();
    lcd_clr();
}

void lcd_clr()
{
    /*lcd_cs1(); // Left side
    for (int i = 0; i < 8; i++)
    {
        lcd_writedata(0); // Clear page
        lcd_en();
        lcd_writecommand(LCD_CMD_SETXADDR | i); // Increment page
    } // Move to the next page

    lcd_cs2(); // Right side
    for (int i = 0; i < 8; i++)
    {
        lcd_writedata(0); // Clear page
        lcd_en();
        lcd_writecommand(LCD_CMD_SETXADDR | i); // Increment page
    } // Move to the next page

    lcd_csnone(); // Clear selection*/
}

void lcd_draw_dot(uint8_t x, uint8_t y)
{
    // todo set actual address
    lcd_writecommand(LCD_CMD_SETXADDR|1, LCD_CS1);
    lcd_writecommand(LCD_CMD_SETYADDR, LCD_CS1);
    lcd_writedata(255, LCD_CS1); // fill a page at 0,0
    lcd_csnone();
}

#endif