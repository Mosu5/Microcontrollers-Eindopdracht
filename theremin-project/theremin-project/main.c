#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcdDisplay.h"

#define BIT(x) (1 << (x))
#define INTERVAL 2273

unsigned int sCount = 0, minutes = 0, hours = 0;

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1); // library function (max 30 ms at 8MHz)
	}
}

// Initialize timer 1: fast PWM at pin PORTB.6 (hundredth ms)
void timer1Init(void)
{
	ICR1 = INTERVAL;	  // TOP value for counting = INTERVAL*us
	OCR1A = INTERVAL / 2; // compare value in between
	TCCR1A = 0b10000010;  // timer, compare output at OC1A=PB5
	TCCR1B = 0b00011010;  // fast PWM, TOP = ICR1, prescaler=8 (1MHz), RUN
}

void toggle_buzz()
{
	PORTA ^= BIT(0);
}

#define TRIG PD5
#define ECHO PD4

int main(void)
{
	uint16_t duration;
	DDRD |= (1 << TRIG);
	DDRD &= ~(1 << ECHO);
	DDRA = 0xFF;
	DDRB = 0x00;

	// initialize lcd
	init_4bits_mode();
	lcd_clear();
	lcd_write_string("Hello World!");

	while (1)
	{
		// listen to a button press on PORTB.0
		// if button is pressed, init lcd
		if (
			(PINB & (1 << 0)) == 0)
		{
			init_4bits_mode();
			_delay_ms(10000);
			lcd_clear();
			lcd_write_string("Hello World!");
		}

		PORTD |= (1 << TRIG); // set PORTD.5 to output high
		_delay_us(40);
		PORTD &= ~(1 << TRIG);
		_delay_us(25000);

		// when pulse starts high start timer
		// stop  timer when pulse starts low
		// save duration for buzz

		// empty while loop until pulse starts high
		while ((PIND & (1 << ECHO)) == 0)
			;

		// start timer
		timer1Init();

		// empty while loop until pulse starts low
		while ((PIND & (1 << ECHO)) != 0)
			;

		// save the duration of the pulse in duration
		duration = ICR1;

		// write duration to lcd
		lcd_clear();
		lcd_write_string("duration: ");
		lcd_write_integer(duration);

		// toggle buzz with duration as delay
		toggle_buzz();
		wait(duration);
		toggle_buzz();
	}
}