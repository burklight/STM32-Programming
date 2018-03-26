/*************************************************************

 B a s e . c

 Version 1.1 (9/6/2015)

 *************************************************************/

#include "Base.h"           // Basic definitions

/********************* GPT 5 *********************************/

// Timer 5 (GPT5) is configured for short busy delays

// Configures timer 5 (GPT5)
static const GPTConfig gpt5cfg = {
  1000000,    /* 1MHz clock for Timer 5 */
  NULL,       /* No Callback */
  0           /* Dier field */
};

// This function returns after the time is elapsed
// Timer granularity is 10us
// Time is rounded in excess in 10us steps
//      time: Wait time in microseconds
void gptDelayUs(uint32_t time)
 {
 uint32_t count;

 for(count=0;count<time;count+=10)
	 gptPolledDelay(&GPTD5,10);

 }

/********************* GPT 9 *********************************/

// Timer 9 (GPT9) is configured for short busy delays
// this timer is independent on GPT5
// That way two thread can use different timers

// Configures timer 9 (GPT5)
static const GPTConfig gpt9cfg = {
  1000000,    /* 1MHz clock for Timer 9 */
  NULL,       /* No Callback */
  0           /* Dier field */
};

// This function returns after the time is elapsed
// Timer granularity is 10us
// Time is rounded in excess in 10us steps
//      time: Wait time in microseconds
void gptDelayUs2(uint32_t time)
 {
 uint32_t count;

 for(count=0;count<time;count+=10)
	 gptPolledDelay(&GPTD9,10);

 }

/************** Initialization ****************************/

// General initialization
void baseInit(void)
 {
 // HAL initialization
 halInit();

 // RTOS Kernel initialization
 chSysInit();

 // GPT 5 timer initialization
 gptStart(&GPTD5, &gpt5cfg);

 // GPT 9 timer initialization
 gptStart(&GPTD9, &gpt9cfg);
 }




