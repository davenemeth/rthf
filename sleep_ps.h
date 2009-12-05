/***** ***** *****
* Function name: SIGNAL(SIG_OVERFLOW0)
*
* Overview: 
*	Timer0 interrupt handler. 
*	Called, when timer0 overflows.
***** ***** *****/

SIGNAL(SIG_OVERFLOW0);


/***** ***** *****
* Function name: void sleep_ps (long time);
*
* Overview:
*	The sleep function.
*
* Input:
*	 time - should be given in millisec.
***** ***** *****/

void sleep_ps (uint32_t time);

/***** defines for the function *****/

// timer/counter oscillator frequency
#define tc_osc_freq 32768

// timer0 overflow time in usec
#define period_1024 ( (uint32_t)256*1000*1000/(tc_osc_freq/1024) )
//#define period_256 	( (uint32_t)256*1000*1000/(tc_osc_freq/256) )
//#define period_128 	( (uint32_t)256*1000*1000/(tc_osc_freq/128) )
//#define period_64 	( (uint32_t)256*1000*1000/(tc_osc_freq/64) )
//#define period_32 	( (uint32_t)256*1000*1000/(tc_osc_freq/32) )
//#define period_8 	( (uint32_t)256*1000*1000/(tc_osc_freq/8) )
//#define period_1 	( (uint32_t)256*1000*1000/(tc_osc_freq) )


/***** ***** *****
* Function name: void Timer0_Init(void);
*
* Overview: 
*	Timer0 initializer function. 
*	Sets the timer0 prescaler, the initial value, and enables the interrupt
* Input:
*	initValue
*	prescale
***** ***** *****/

void Set_Timer0(uint8_t initValue, uint8_t prescale);
