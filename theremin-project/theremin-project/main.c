/*
 * theremin-project.c
 *
 * Created: 22-3-2023 08:45:33
 * Author : moust
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define BIT(x) (1 << (x))

unsigned int sCount=0, minutes=0, hours=0;


void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms(1);
	}
}

// Initialize timer 1: fast PWM at pin PORTB.6 (hundredth ms)
void timer1Init( void ) {
	OCR1A = 0;					// RED, default, off
	OCR1B = 0;					// GREEN, default, off
	OCR1C = 0;					// BLUE, default, off
	TCCR1A = 0b10101001;		// compare output OC1A,OC1B,OC1C
	TCCR1B = 0b00001011;		// fast PWM 8 bit, prescaler=64, RUN
}

void setPWM( unsigned char pulse ) {
	OCR1A = pulse;
}


int main()
{
	DDRC = 0xFF;					// set PORTB for compare output
	timer1Init();
	wait(100);

	while (1) {
		int delta = 1;
		setPWM (0);

		for (int red = 0; red<=440; red+=delta) {
			setPWM( red );				// 8-bits PWM on pin OCR1a
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		for (int red = 255; red>=0; red-=delta) {
			setPWM( red );				// 8-bits PWM on pin OCR1a
			delta -= 2;					// progressive steps down
			wait(100);					// delay of 100 ms (busy waiting)
		}
		setPWM( 0 );
		delta = 1;
		wait(100);

		
	}
	return 0;
}