/**************************************************************

 i n t . c

 Interrupt test source file

***************************************************************/

#include "Base.h"     // Basic definitions
#include "int.h"      // This module header file
#include "mcuconf.h"
#include "lcd.h"
#include "accel.h"

/********** PUBLIC FUNCTIONS TO IMPLEMENT ************************
 Those functions will be used outside of lcd.c so there will
 be prototypes for them in lcd.h.
******************************************************************/

volatile int switchFlag=0;

// Configures the button (PA0) ISR
// PA0 is already configured as input in the halinit() call
// This function must:
//
//    * Activate the SYSCFG clock
//
//    * Assigna port A to EXTI0
//
//    * Enable the EXTI0 interrupt
//    * Acctivate EXTI0 on rising edge
//    * Clear the pending interrupt flag at EXTI0
//
//    * Enable EXTI0 at NVIC
void interruptTest(void)
 {
	//Activate the SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//Assign port A to EXTI0
	SYSCFG->EXTICR[0] &= (~SYSCFG_EXTICR1_EXTI0);

	//Enable the EXTI0 interrupt
	EXTI->IMR |= EXTI_IMR_MR0;

	//Acctivate EXTI0 on rising edge
	EXTI->RTSR |= EXTI_RTSR_TR0;
	EXTI->FTSR &= (~EXTI_FTSR_TR0);

	//Clear the pending interrupt flag at EXTI0
	EXTI->PR = EXTI_PR_PR0;

	//Enable EXTI0 at NVIC
	nvicEnableVector(EXTI0_IRQn,CORTEX_PRIORITY_MASK(STM32_EXT_EXTI0_IRQ_PRIORITY));

	char string[20];
	LCD_GotoXY(0,0);
	LCD_SendString("Y = ");

	while(1){
		switchFlag = 0;

		//Waiting for the botton to be pressed
		while(!switchFlag);

		itoa(((int32_t) readAccel(0x2B,1)),string,10);
		LCD_GotoXY(4,0);
		LCD_SendString("    ");
		LCD_GotoXY(4,0);
		LCD_SendString(string);
		//DELAY_US(40);
	}
 }


/********************* ISR FUNCTIONS ******************************
Those functions should never be called directly or indirectly from
the main program. They are automatically called by the NVIC
interrupt subsystem.
*******************************************************************/

// EXTI 0 ISR
// Associated to the user button
// Tasks to do:
//    * Erase the pending interrupt flag
//    * Change the green led status
//    * Activate the flag so that the accelerometer Y axis is read
CH_IRQ_HANDLER(EXTI0_IRQHandler)
 {
 CH_IRQ_PROLOGUE();
 
 //Erase the pending interrupt flag
 EXTI->PR = EXTI_PR_PR0;

 //Change the green led status
 LEDS_PORT->ODR^=GREEN_LED_BIT;

 //Activate the flag so that the accelerometer Y axis is read
 switchFlag = 1;

 CH_IRQ_EPILOGUE();
 }

