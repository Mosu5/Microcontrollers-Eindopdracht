#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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


// Main program: Counting on T1
int main( void ) {
	DDRB = 0xFF;					// set PORTB for compare output
	DDRC = 0xFF;					// set PORTA for output in main program
	timer1Init();					// it is running now!!
	init_4bits_mode();
	
	DDRA = 0x00;
	
	
	lcd_clear();
	lcd_write_string("Button 2");
	
	while (1) {

		
		DDRC = 0xFF; // set all Port A pins as an output
		
		PORTC = 0b00100000; // write 1 (high) to PORTA.5 (trigger pin )
		wait(10); // keep high signal output for 10µs
		PORTC = 0x00; // write 0 (low) to PORTA.5
		
		DDRC = 0x00;  // Configure all Port A pins as an input
		while (PINC & 0x00){
			
		}
		int waitTime = PINC;  // Wait until PINC.4 (echo pin) has 1 (high) value
		if(PINC & BIT (4))   // Once PINC.4 is high, while loop will break and this line will be execute
		{
									// every 100 ms (busy waiting)
				lcd_clear();					
				lcd_write_integer(PINC);
				wait(100);
		}

	}
}