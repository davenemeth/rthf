/*
**************************************************************************************************************
*                 Long time, power-save sleep function for the mitmót
*
* filename:	altatas.c
* created:	David Nemeth, 2009 
*
* prerequisites:
*	- WinAVR 20071221 is installed in the root of drive C (C:\WinAVR)
*
* function:	
*	- uses the Low-Frequency Crystal Oscillator in power-save mode
*	- calculates the most efficient sleep periods (by changing the prescaler settings)
*
**************************************************************************************************************
*/

#include "mcu_avr_atmega128_api.h" 	// MCU API   
#include "sleep_ps.h"
#include <avr/sleep.h>
#include <stdint.h>


uint8_t sleep_counter=0;

/********  Timer0 overflow IT Service Routine  ***************************/
SIGNAL(SIG_OVERFLOW0) // Timer0 overflow
{
	sleep_counter++;
}

/********  function sleep_ps  ***************************/
void sleep_ps (uint32_t time) {
	
	uint32_t time_us;
	uint32_t time_us_rem;
	uint8_t count_1024;
	uint8_t initValue_1024;
	uint8_t initValue_32;

	// change timer/counter0 clock source to 32k crystal
	ASSR = _BV(AS0);
	
	// set timer0
	Set_Timer0(0, 0x07);

	// set sleep mode to power-save mode
	MCUCR = _BV(SM0) | _BV(SM1);
	
	time_us = 1000*time;

	// wait for TCCR0 and TCNT0 to be ready (both 0)
	while ( 0 != ( ASSR & (_BV(TCN0UB) | _BV(TCR0UB)) ) ) {
		;
	}
	
	// calculate the number of full periods spent in sleep mode (TCCR0=0x07)
	count_1024 = time_us / period_1024;
	time_us_rem = time_us % period_1024;
	
	// calculate the initValues for the remaining time
	// it should be calculated before the "long sleep" to be able to change TCNT0 with minimum latency
	// +8 => +250ms				   --->
	// truncate => -0ms...-31.25ms ---> remainder is between 218.75ms and 250ms
	// initValue for the slow counter sleep
	initValue_1024 = 255 - (time_us_rem * 256) / period_1024 + 8;
	// initValue for the  fast counter sleep
	initValue_32 = ( (time_us_rem * 256) % period_1024) * 32;
	
	// enable interrupts
	sei();

	// sleep for the calculated time
	for (uint8_t i=0; i<count_1024; i++) {
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}

	// set the freshly calculated initValue
	TCNT0 = initValue_1024;

	// wait for TCNT0 to be ready (TCN0UB == 0)
	while ( 0 != (ASSR & _BV(TCN0UB)) ) {
			;
		 }
	// sleep for the remaining time (slow counter)
	sleep_enable();
	sleep_cpu();
	sleep_disable();

	Set_Timer0(initValue_32, 0x03);
	// wait for TCNT0 to be ready (TCN0UB == 0)
		// wait for TCCR0 and TCNT0 to be ready (both 0)
	while ( 0 != ( ASSR & (_BV(TCN0UB) | _BV(TCR0UB)) ) ) {
		;
	}

	// sleep for the remaining time (fast counter)
	sleep_enable();
	sleep_cpu();
	sleep_disable();

	// turn timer0 off
	TCCR0 = 0;

	// wait for TCCR0 to be ready (TCR0UB == 0)
	while ( 0 != (ASSR & _BV(TCR0UB)) ) {
			;
		 }

	// disable the 32k source
	ASSR = ~_BV(AS0);
}

/********  Timer0 initialisation  ***************************/
void Set_Timer0(uint8_t initValue, uint8_t prescale)
{
   TCNT0 = initValue;			// set the counter initial value
   TCCR0 = prescale;			// set TIMER0 prescaler to CLK/1024
   TIMSK = _BV(TOIE0);			// enable TIMER0 overflow interrupt
}
