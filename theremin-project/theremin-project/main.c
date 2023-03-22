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

void wait(int ms);
void wait_us(int us);
void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void blocking_buzzer(int duration);

#define TRIG PD5
#define ECHO PD4
#define BUZZ PA4

// Global variables
uint16_t busy;
uint16_t watchdog;
uint16_t duration;

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}

void wait_us(int us)
{
	for (int i = 0; i < us; i++)
	{
		_delay_us(1);
	}
}

void buzzer_init(void)
{
	DDRA |= (1 << BUZZ);
}

void buzzer_on(void)
{
	PORTA |= (1 << BUZZ);
}

void buzzer_off(void)
{
	PORTA &= ~(1 << BUZZ);
}

void blocking_buzzer(int duration)
{
	busy = 1;
	buzzer_on();
	wait(duration / 2);
	buzzer_off();
	wait(duration / 2);
	busy = 0;
}

// Main function
int main(void)
{
	// init global variables
	busy = 0;
	watchdog = 0;
	duration = 0;

	DDRD |= (1 << TRIG);
	DDRD &= ~(1 << ECHO);
	DDRA = 0xFF;
	DDRB = 0x00;

	while (1)
	{
		if (busy)
		{
			continue;
		}
		PORTD |= (1 << TRIG); // set PORTD.5 to output high
		_delay_us(40);
		PORTD &= ~(1 << TRIG);
		_delay_us(40);

		// while echo is high, count
		while (PIND & (1 << ECHO))
		{
			watchdog++;
			if (watchdog > 20000)
			{
				break;
			}
			duration++;
			_delay_us(100);
		}

		// when the echo is low, stop counting, set the buzzer frequency and reset the duration
		if (watchdog < 20000)
		{
			blocking_buzzer(duration * 10);
		}
		duration = 0;
		//_delay_us(30000); // wait until echo times out
	}
}
