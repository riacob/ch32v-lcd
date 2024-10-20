/**
 * @file lcd_test.c
 * @author Riccardo Iacob (github.com/riacob)
 * @brief Implementation of ch32v-lcd test mode
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024, MIT license
 *
 */
#include "lcd.h"

void lcd_test()
{
#if defined(LCDCONF_TESTMODE)

#if defined(LCDCONF_CATGR_CHARACTER)

#if defined(LCDCONF_TYPE_C_2004_GENERIC_8BIT) || defined(LCDCONF_TYPE_C_2004_GENERIC_4BIT)
    lcd_init();
    lcd_setcursor(0, 0);
    lcd_printstr("Hello World");
    lcd_setcursor(0, 1);
    lcd_printstr("Second line :)");
    lcd_setcursor(0, 2);
    lcd_printstr("Third line :)");
    lcd_setcursor(0, 3);
    lcd_printstr("Fourth line :)");
#elif defined(LCDCONF_TYPE_C_1602_GENERIC_8BIT) || defined(LCDCONF_TYPE_C_1602_GENERIC_4BIT)
    lcd_init();
    lcd_setcursor(0, 0);
    lcd_printstr("Hello World");
    lcd_setcursor(0, 1);
    lcd_printstr("Second line :)");
#else
#error "Selected display type has no testing routine"
#endif

#elif defined(LCDCONF_CATGR_GRAPHICAL)

#if defined(LCDCONF_TYPE_G_WG12864A_8BIT)
    lcd_init();
    lcd_draw_dot(0, 0);
// lcd_writebuffer();
#else
#error "Selected display type has no testing routine"
#endif
#endif

#else
#error "Trying to run lcd_test() test while LCDCONF_TESTMODE is disabled"
#endif
}