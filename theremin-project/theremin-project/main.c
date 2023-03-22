/*
This code is for the main function of the program.
It will keep the buzzer going until the user presses the button. (on/off switch)
Also, it will sent out a pulse to the sensor and then wait for the echo to come back.
it will then calculate the distance and change the pitch of the buzzer accordingly.
*/

#define F_CPU 8000000UL // 8 MHz clock speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define pins
#define TRIG PD5
#define ECHO PD4
#define BUZZ PA4

// Define variables
volatile int distance = 0;
volatile int distanceHigh = 0;

int lengthOfScale = 0;

int note = 0;

// A Minor pentatonic scale
int scale[] = {
	147, 165, 196, 220, 262, 294, 330, 392, 440,
	523, 587, 659, 784, 880, 1047, 1175, 1319, 1568,
	1760, 2093, 2349};

// Main function
int main(void)
{
	DDRD |= (1 << TRIG);  // Set TRIG as output
	DDRD &= ~(1 << ECHO); // Set ECHO as input
	DDRA |= (1 << BUZZ);  // Set BUZZ as output

	while (1)
	{

		// Turn the buzzer on
		PORTA |= (1 << BUZZ);

		// wait for 15ms
		_delay_ms(15);

		// Turn the buzzer off
		PORTA &= ~(1 << BUZZ);

		// wait for 15ms
		_delay_ms(15);
	}
}
