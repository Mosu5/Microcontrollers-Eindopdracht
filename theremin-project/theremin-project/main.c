#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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


	while (1) {
		// do something else
		wait(100);					// every 100 ms (busy waiting)
		PORTC ^= BIT(0);			// toggle bit 7 PORTA
	}
}