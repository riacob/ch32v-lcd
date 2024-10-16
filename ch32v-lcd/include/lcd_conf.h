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

/* Driver Selection */
//#define LCDCONF_TYPE_G_NT7108C_12864 1
#define LCDCONF_TYPE_C_1602
//#define LCDCONF_TYPE_C_2004

/* Interface Selection */
#define LCDCONF_INTERFACE_8BIT
//#define LCDCONF_INTERFACE_4BIT
//#define LCDCONF_INTERFACE_SPI



#define LCDCONF_DB 

#endif /* _LCD_CONF_H */