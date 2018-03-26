/*************************************************************

   m a i n . c

   Practica 1

 *************************************************************/

#include "Base.h"     // Basic definitions
#include "lcd.h"      //LCD Module Header File
#include "accel.h"    //accelerometer module
#include "int.h"	  //interruption module
#include "keyboard.h"
#include "analog.h"
#include "encoder.h"
#include "comms.h"

volatile uint32_t i=0;

// Function that blinks the green LED
void ledBlink(void)
 {
 while(1)
   {

  	  // Turn on the green LED using the BSRR register
	 (LEDS_PORT->BSRR.H.set)=BIT(GREEN_LED_PAD);


   	   // Wait 200ms
   	   SLEEP_MS(200);


   	  // Turn off the green LED using the BSRR register
   	  (LEDS_PORT->BSRR.H.clear)=BIT(GREEN_LED_PAD);


   	   // Wait 200ms
   	   SLEEP_MS(200);
  }
 }

 // Function that blinks the green, orange, red and blue LED alternatively
 void ledSequence(void)
  {
  while(1)
    {

 	 i++;
   switch(i%4){
   	  // Turn on the green, orange, red or blue LED using the BSRR register alternatively
   case 0:  (LEDS_PORT->BSRR.H.set)=BIT(GREEN_LED_PAD);
   	  break;
   case 1:  (LEDS_PORT->BSRR.H.set)=BIT(ORANGE_LED_PAD);
   	  break;
   case 2: (LEDS_PORT->BSRR.H.set)=BIT(RED_LED_PAD);
   	  break;
   case 3: (LEDS_PORT->BSRR.H.set)=BIT(BLUE_LED_PAD);
   	  break;
   }


    	   // Wait 500ms
    	   SLEEP_MS(500);


    	switch(i%4){
    	  // Turn off the green, orange, red or blue LED using the BSRR register alternatively
    	  case 0:  (LEDS_PORT->BSRR.H.clear)=BIT(GREEN_LED_PAD);
    	  	  break;
    	  case 1:  (LEDS_PORT->BSRR.H.clear)=BIT(ORANGE_LED_PAD);
    	  	  break;
    	  case 2: (LEDS_PORT->BSRR.H.clear)=BIT(RED_LED_PAD);
    	  	  break;
    	  case 3: (LEDS_PORT->BSRR.H.clear)=BIT(BLUE_LED_PAD);
    	  	  break;
    	  }

    	   // Wait 200ms
    	   SLEEP_MS(200);
   }
 }


 int main(void)
  {
	 baseInit();
	 initComms();
	 while(1);
  }

