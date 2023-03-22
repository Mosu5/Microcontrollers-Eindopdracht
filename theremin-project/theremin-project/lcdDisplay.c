/*
 * lcdDisplay.c
 *
 * Created: 09/02/2023 11:55:16
 * Author : Peter
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include "lcdDisplay.h"
#include <string.h>
#include <stdio.h>

void writeInitCommand_strobe_e(unsigned char byte)
{
	byte &= 0xF0;
	PORTC = byte | (1<<LCD_E);	// E high
	_delay_ms(1);				// nodig
	PORTC &= ~(1<<LCD_E);  		// E low
	_delay_ms(1);				// nodig?
}

void write2NibbleCommand_strobe_e(unsigned char byte)
{
	unsigned char tmpByte = byte;

	// First nibble.
	byte &= 0xF0;
	PORTC = byte | (1<<LCD_E);	// E high
	_delay_ms(1);				// nodig
	PORTC &= ~(1<<LCD_E);  		// E low
	_delay_ms(1);				// nodig?
	
	// Second nibble
	byte = (tmpByte<<4);
	byte &= 0xF0;
	PORTC = byte | (1<<LCD_E);	// E high
	_delay_ms(1);				// nodig
	PORTC &= ~(1<<LCD_E);  		// E low
	_delay_ms(1);				// nodig?
}

void write2NibbleData_Strobe_E(unsigned char byte)
{
	unsigned char tmpByte = byte;

	// First nibble.
	byte = byte & 0xF0;
	byte |= (1<<LCD_RS);  		// RS high
	PORTC = byte | (1<<LCD_E);	// E high
	_delay_ms(1);				// nodig
	PORTC &= ~(1<<LCD_E);  		// E low
	_delay_ms(1);				// nodig?

	// Second nibble
	byte = (tmpByte<<4);
	byte &= 0xF0;
	byte |= (1<<LCD_RS);  		// RS high
	PORTC = byte | (1<<LCD_E);	// E high
	_delay_ms(1);				// nodig
	PORTC &= ~(1<<LCD_E);  		// E low
	_delay_ms(1);				// nodig?
}

void init_4bits_mode(void)
{
	// Step 2 (table 12)
	// function set
	writeInitCommand_strobe_e(0x20);

	// Step 3 (table 12)
	// function set
	write2NibbleCommand_strobe_e(0x28);

	// Step 4 (table 12)
	// Display on/off control
	write2NibbleCommand_strobe_e(0x0F);

	// Step 4 (table 12)
	// Entry mode set
	write2NibbleCommand_strobe_e(0x06);
}

void lcd_write_string(char *str)
{
	// of met een for:
	for(;*str; str++){
		write2NibbleData_Strobe_E(*str);
	}
}

void lcd_write_integer(int data)
{
	char data_out[16];
	memset(data_out, 0, sizeof(data_out[0]) * 16);
	sprintf(data_out, "%s %u", "Counter: " , data);
	lcd_write_string(data_out);
}

void lcd_clear()
{
	write2NibbleCommand_strobe_e (0x01);
}
