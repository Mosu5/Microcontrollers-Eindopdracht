#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcdDisplay.h"

#define BIT(x) (1 << (x))
#define INTERVAL 2273

void wait(int ms);
void wait_us(int us);
void buzzerOn(uint16_t duration);
void set_buzzer_pitch(uint16_t pitch);
void turn_off_buzzer();
unsigned int sCount = 0, minutes = 0, hours = 0;

#define TRIG PD5
#define ECHO PD4

#define BUZZ PA0

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

	int watchdog = 0;
	duration = 0;
	
	set_buzzer_pitch(440);

	while (1)
	{
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
			_delay_us(1);
		}

		// when the echo is low, stop counting, set the buzzer frequency and reset the duration
		if (watchdog < 20000)
		{
			buzzerOn(duration);
		}
		duration = 0;
		_delay_us(30000); // wait until echo times out
	}
}

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		wait(1); // library function (max 30 ms at 8MHz)
	}
}

void wait_us(int us)
{
	for (int i = 0; i < us; i++)
	{
		_delay_us(1); // library function (max 30 ms at 8MHz)
	}
}

// buzzerOn(): turns the buzzer on for a certain duration
void buzzerOn(uint16_t duration)
{
	// set the buzzer frequency
	set_buzzer_pitch(duration);

	// turn the buzzer on
	PORTA |= (1 << BUZZ);

	// wait for the duration
	wait_us((int)duration);

	// turn the buzzer off
	PORTA &= ~(1 << BUZZ);
}

// // set_buzzer_pitch(): sets the buzzer frequency in Hz (pitch)
// void set_buzzer_pitch(uint16_t pitch)
// {
// 	// set the buzzer frequency
// 	OCR1A = INTERVAL / pitch;
// }

void set_buzzer_pitch(uint16_t pitch)
{
	// Set up Timer/Counter0 for PWM output on OC0A (PD6)
	DDRD |= (1 << PD6);
	TCCR1A = (1 << COM1A0) | (2 << WGM00);
	TCCR1B = (3 << CS00);

	// Set the frequency
	OCR1A = F_CPU / (2 * pitch) - 1;

	// turn the buzzer on
	PORTA |= (1 << BUZZ);
}

void turn_off_buzzer()
{
	// turn the buzzer off
	PORTA &= ~(1 << BUZZ);
}