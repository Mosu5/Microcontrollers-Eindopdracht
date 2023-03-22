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
void init_timer0(void);
void set_buzzer_pitch(int pitch);

#define TRIG PD5
#define ECHO PD4
#define BUZZ PA4

void init_timer1(void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x0B;
	TCNT1 = 0x0000;
}

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

void init_timer0(void)
{
	TCCR1A = 0x02;
	TCCR1B = 0x05;
	TCNT0 = 0x00;
}

void set_buzzer_pitch(int pitch)
{
	OCR1A = pitch;
}

// Main function
int main(void)
{
	uint16_t distance = 0;
	uint16_t pitch = 0;
	uint16_t watchdog = 0;
	uint16_t duration = 0;

	DDRD |= (1 << TRIG);
	DDRD &= ~(1 << ECHO);
	DDRA = 0xFF;
	DDRB = 0x00;

	while (1)
	{

		PORTD |= (1 << TRIG); // set PORTD.5 to output high
		_delay_us(40);
		PORTD &= ~(1 << TRIG);
		_delay_us(40);

		// while echo is high, count
		buzzer_on();
		while (PIND & (1 << ECHO))
		{
			buzzer_on();

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
			buzzer_off();
		}
		duration = 0;
		_delay_us(30000); // wait until echo times out
	}
}
