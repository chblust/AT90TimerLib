#include <avr/interrupt.h>
#include "timer.h"

// Records if the timer has triggered an overflow or not
static uint8_t timeElapsed = 0;

// Millisecond interval
static uint16_t timerLength = 10;

void initTimer()
{
	// Ensure global interrupts are enabled
	sei();

	// Set prescaler to clock/1024
	uint8_t tccr1b = 0;
	tccr1b |= ( (1<<CS12) | (1<<CS10) );
	TCCR1B = tccr1b;
}

void setTimer( uint16_t ms )
{
	// Enable 16 bit timer interrupt
	TIMSK1 |= (1<<TOIE1);

	// "Wind up" the timer by the amount of ticks of the clock per the given millisecond value
	TCNT1 = 0 - (ms / .128);

	// Set the saved interval to the millisecond value for this timer to be repeated.
	timerLength = ms;
}

uint8_t getTimeElapsed()
{
	uint8_t ret = timeElapsed;
	
	timeElapsed = 0;
	
	return ret;
}

ISR( TIMER1_OVF_vect )
{
	timeElapsed = 1;

	TCNT1 = 0 - (timerLength / .128);
}