#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcdDisplay.h"

#define BIT(x)			(1 << (x))
#define INTERVAL  		2273

unsigned int sCount=0, minutes=0, hours=0;

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );				// library function (max 30 ms at 8MHz)
	}
}

// Initialize timer 1: fast PWM at pin PORTB.6 (hundredth ms)
void timer1Init( void ) {
	ICR1 = INTERVAL;				// TOP value for counting = INTERVAL*us
	OCR1A = INTERVAL/2;				// compare value in between
	TCCR1A = 0b10000010;			// timer, compare output at OC1A=PB5
	TCCR1B = 0b00011010;			// fast PWM, TOP = ICR1, prescaler=8 (1MHz), RUN
}

void toggle_buzz( ) {
	PORTA ^= BIT(0);
}

#define TRIG PC5
#define ECHO PC4

int main(void)
{
	uint16_t duration;
	DDRC |= (1 << TRIG);
	DDRC &= ~(1 << ECHO);
	DDRA = 0xFF;
	
/*	
	init_4bits_mode();
	lcd_clear();
	lcd_write_string("hi");
*/

	while(1)
	{
		PORTC |= (1 << TRIG); // set PORTC.5 to output high
		_delay_us(40);  
		PORTC &= ~(1 << TRIG);
		_delay_us(25000);
/*		while(!(PINC & (1 << ECHO)));
		TCNT1 = 0;
		TCCR1C |= (1 << CS10);

		while(PINC & (1 << ECHO));
		TCCR1C = 0;


		duration = TCNT1 / 58;

		toggle_buzz();
		wait(duration);
		toggle_buzz();
		wait(duration);
		
		wait(500);
		
		printf((char *) duration);
		*/
	}
}