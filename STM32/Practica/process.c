/**************************************************************

 p r o c e s s . c

 Introduction to processes source file

***************************************************************/

#include "Base.h"      // Basic definitions
#include "lcd.h"       // LCD module header file
#include "process.h"   // This module header file

/************** FOUR THREAD SIMPLE EXAMPLE *********************/

// Working area for the childs threads
static WORKING_AREA(waChild1,128);
static WORKING_AREA(waChild2,128);
static WORKING_AREA(waChild3,128);

// Function prototype for the child thread
// Needed because thChild is referenced before its definition
// It is not included in process.h because it's a static function
static msg_t thChild(void *arg);

// Test for simple four thread operation
// This function:
//     - Initializes the system
//     - Creates a child thread that blinks the orange LED
//	- Creates a child thread that blinks the green LED
//	- Creates a child thread that blinks the red LED
//     - Blinks the blue LED
void test4threads(void)
 {
 // Basic system initialization
 baseInit();

 // Creation of the void pointer to run different process with the same function
 uint32_t x1, x2, x3;
 x1=1;
 x2=2;
 x3=3;
 void* thread1 = &x1;
 void* thread2 = &x2;
 void* thread3 = &x3;

 // Child thread creation
 chThdCreateStatic(waChild1, sizeof(waChild1), NORMALPRIO, thChild, thread1);
 chThdCreateStatic(waChild2, sizeof(waChild2), NORMALPRIO, thChild, thread2);
 chThdCreateStatic(waChild3, sizeof(waChild3), NORMALPRIO, thChild, thread3);

 while (TRUE)
     {
	 // Turn on blue LED using BSRR
	 (LEDS_PORT->BSRR.H.set)=BLUE_LED_BIT;

	 // Pause
 	 busyWait(1);

 	 // Turn off blue LED using BSRR
 	 (LEDS_PORT->BSRR.H.clear)=BLUE_LED_BIT;

 	 // Pause
     	busyWait(1);
     }
 }


// Child threads that bliks the orange, green and red LEDs
static msg_t thChild(void *arg)
 {
 if(*(int *) arg==1){
  while (TRUE)
    {
 	// Turn on orange LED using BSRR
	(LEDS_PORT->BSRR.H.set)=ORANGE_LED_BIT;

	// Pause
	busyWait(2);

	// Turn off orange LED using BSRR
	(LEDS_PORT->BSRR.H.clear)=ORANGE_LED_BIT;

	// Pause
	busyWait(2);
    }
 } else if (*(int *) arg==2){
  while (TRUE)
    {
 	// Turn on green LED using BSRR
	(LEDS_PORT->BSRR.H.set)=GREEN_LED_BIT;

	// Pause
	busyWait(4);

	// Turn off gren LED using BSRR
	(LEDS_PORT->BSRR.H.clear)=GREEN_LED_BIT;

	// Pause
	busyWait(4);
    }
 } else if (*(int *) arg==3){
  while (TRUE)
    {
 	// Turn on red LED using BSRR
	(LEDS_PORT->BSRR.H.set)=RED_LED_BIT;

	// Pause
	busyWait(8);

	// Turn off red LED using BSRR
	(LEDS_PORT->BSRR.H.clear)=RED_LED_BIT;

	// Pause
	busyWait(8);
    }
 }
 return 0;
 }

// Busy waits in a thread doing some operations
// The greater n, the longer the wait
void busyWait(uint32_t n)
 {
 uint32_t i;
 volatile uint32_t x=0;
 for(i=0;i<n*1000000;i++)
	    x=(x+2)/3;
 }


/************ TWO THREAD SIMPLE EXAMPLE ENDS *******************/



