/*
 * lcdDisplay.c
 *
 * Created: 09/02/2023 11:55:16
 * Author : Peter
 */ 

#ifndef _LCDDISPLAY_H
#define _LCDDISPLAY_H


#define LCD_E 	3
#define LCD_RS	2

void writeInitCommand_strobe_e(unsigned char byte);
void write2NibbleCommand_strobe_e(unsigned char byte);
void write2NibbleData_Strobe_E(unsigned char byte);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_integer(int data);
void lcd_clear();

#endif	/* lcdDisplay.h */