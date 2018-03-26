/*
 * comms.c
 *
 *  Created on: 29/04/2016
 *      Author: marc.de.cea
 */


#include "Base.h"     // Basic definitions
#include "comms.h"
#include "lcd.h"





static void GPIO_ModePushPull(GPIO_TypeDef *port, int32_t linia, int32_t speed)
 {
	// 1st step: modify MODER register to set OUTPUT mode
	(port->MODER) = ((port->MODER) & (~BIT(2*linia + 1))) | (BIT(2*linia));

	// 2nd step: modify OTYPER register to select PUSH-PULL mode
	(port->OTYPER) = (port->OTYPER)&(~BIT(linia));

	// 3rd step: modify OSPEEDR register to set the working speed
	switch(speed)
	{
		case (2):
		 (port->OSPEEDR) = (port->OSPEEDR) & (~BIT(2*linia)) & (~BIT(2*linia + 1));
		 break;

		case (25):
		 (port->OSPEEDR) = ((port->OSPEEDR) & (~BIT(2*linia + 1))) | (BIT(2*linia));
		 break;

		case (50):
		 (port->OSPEEDR) = ((port->OSPEEDR) | (BIT(2*linia + 1))) & (~BIT(2*linia));
		 break;

		default:
		 (port->OSPEEDR) = (port->OSPEEDR) | (BIT(2*linia)) | (BIT(2*linia + 1));
	}


	// 4th step: modify the PUPDR register to set no pull-up/pull-down resistors
	(port->PUPDR) = (port->PUPDR) & (~BIT(2*linia)) & (~BIT(2*linia + 1));


	// 5th step: set the output to '0' (modify ODR register)
	(port->BSRR.H.clear) = BIT(linia);
 }


static void GPIO_ModeInput(GPIO_TypeDef *port, int32_t linia)
 {
	// 1st step: modify MODER register to set INPUT mode
	(port->MODER) = ((port->MODER) & (~BIT(2*linia + 1))) & (~BIT(2*linia));

	// 2nd step: modify the PUPDR register to no pull up nor pull down
	(port->PUPDR) = ((port->PUPDR) & (~BIT(2*linia))) & (~BIT(2*linia + 1));

 }



// Working area for the child threads
static WORKING_AREA(clkThread,128);

// Binary semaphore to sinchronize tx_clk and bit transmition
BinarySemaphore   semafC;

// Variable to tell to the clock generation Thread to change status
volatile int32_t clk_generate = 0;

static void clkGeneration(void){

	while (1){

		if (clk_generate){

			// Genarte the clock by toggling between high and low values
			CLK_PORT->ODR = CLK_PORT->ODR ^ BIT(CLK_TX);

			if ( ((CLK_PORT->ODR >> CLK_TX ) & 1) == 0){
				// It is a falling edge --> The data has to be transmitted
				// Wake up the sending thread with the semaphore
				chBSemSignal(&semafC);

			} else {
				// Prevent the transmitter while clk is high
				chBSemWait(&semafC);
			}

			DELAY_US(T);

		} else {

			// In the idle state, CLK_TX is high
			CLK_PORT->ODR = CLK_PORT->ODR | BIT(CLK_TX);
		}
	}

}

void initComms(){

	// Initilaize the LCD
	LCD_Init();

	// Child thread creation. This child thread generates the clock in transmission mode
	chThdCreateStatic(clkThread, sizeof(clkThread), NORMALPRIO, clkGeneration, NULL);

	// TX and CLK-TX have to be configured as Push-Pull output
	GPIO_ModePushPull(COMMS_PORT, TX_PIN, 0);
	GPIO_ModePushPull(CLK_PORT, CLK_TX, 0);

	// RX and CLK_RX as inputs with no pull-up nor pull-down
	GPIO_ModeInput(COMMS_PORT, RX_PIN);
	GPIO_ModeInput(CLK_PORT, CLK_RX);

	// Configure interruption of RX_CLK and RX pin
	// Activate SYSCFG clock
	(RCC->APB2ENR) = (RCC->APB2ENR | RCC_APB2ENR_SYSCFGEN);

	//EXTI13: Assign the port B (clk port) to the external interrupt line EXTI5. With the & we are erasing the 4 bits that were
	// previously indicating which port was assigned, and with the | we are
	// indicating the new assigned port
	(SYSCFG->EXTICR[3]) = (SYSCFG->EXTICR[3] & (~SYSCFG_EXTICR4_EXTI13)) | SYSCFG_EXTICR4_EXTI13_PC;

	// Enable interruption lines
	(EXTI->IMR) = (EXTI->IMR) | EXTI_IMR_MR13;

	// Enable interrupts of RX_Pin at both edges (for synchronization)
	(EXTI->RTSR) = (EXTI->RTSR) | EXTI_RTSR_TR13;
	(EXTI->FTSR) = (EXTI->FTSR) | EXTI_FTSR_TR13;

	// Erase any pending interrupt flag
	(EXTI->PR) = EXTI_PR_PR13;

	// Enable the vector that manages the interruptions of line 13 (RX_PIN)
	nvicEnableVector(EXTI15_10_IRQn , CORTEX_PRIORITY_MASK(STM32_EXT_EXTI10_15_IRQ_PRIORITY));

	// Set intial values of TX and CLK-TX
	CLK_PORT->ODR = CLK_PORT->ODR | BIT(CLK_TX);
	COMMS_PORT->ODR = COMMS_PORT->ODR | BIT(TX_PIN);

	// Initializes the semaphore as not taken
	chBSemInit(&semafC,FALSE);

}




// Send a string to the receiver
void sendMessage(char *string){


	// Indicate the start of a message by creating a falling edge before the
	// clk generation
	COMMS_PORT->ODR = COMMS_PORT->ODR & (~BIT(TX_PIN));

	// Activate the clk modfying clk_generate
	clk_generate = 1;

	char send; // char to send at a given moment

	int32_t i;

	// Send the message
	while (*string){

		// Send the char bit by bit
		 send = *string++;


		 for (i = 0; i<8; i++){

			 chBSemWait(&semafC);

			 if ( ((send>>i)&1) == 0){
				 COMMS_PORT->ODR = (COMMS_PORT->ODR & (~BIT(TX_PIN)));
			 } else {
				 COMMS_PORT->ODR = (COMMS_PORT->ODR | BIT(TX_PIN));
			 }

			 chBSemSignal(&semafC);
		 }
	}

	// Send the EOL sign (0xFF)
	for (i = 0; i<8; i++){

		chBSemWait(&semafC);
		COMMS_PORT->ODR = (COMMS_PORT->ODR | BIT(TX_PIN));
		chBSemSignal(&semafC);
	}

	// Deactivate the clk
	clk_generate = 0;

	// Indicate the end of the message with a rising edge after the clk deactivation
	COMMS_PORT->ODR = COMMS_PORT->ODR | BIT(TX_PIN);

}







/********************* ISR FUNCTIONS ******************************
Those functions should never be called directly or indirectly from
the main program. They are automatically called by the NVIC
interrupt subsystem.
*******************************************************************/




// Global volatile variable to store the received message
int8_t received_message[MAX_LENGTH];
volatile int8_t *current_pos = received_message;


// Process the received message and show it on the LCD screen
static void processMessage(void){

	char message[MAX_LENGTH/8];
	char aux = 0;

	LCD_SendString("k"); // just for debug
	// Iterate over every char in the string and write it using
	// the function LCD_SendChar
	// Put the pointer at the initial position of the array
	current_pos = received_message;

	// We know that the message has finished when the value if the int8 in the
	// array is greater than 2
	int32_t counter_char = 0;
	int32_t counter_string = 0;

	// Our EOL sign is 11111111 = 0xFF
	while (aux != 0xFF){

		aux = aux | (*current_pos<<counter_char);
        counter_char = (counter_char + 1)%8;
		LCD_SendChar(aux); // just for debu
		LCD_SendChar(' '); // just for debug
        if (counter_char == 0){
        	message[counter_string] = aux;
        	aux = 0;
        	counter_string++;
        }

		current_pos++;
	 }

	// We have to remove the EOL sign
	message[counter_string] = 0;

	LCD_SendString(message);

}

// EXTI5 ISR
// Associated to a receiving message
CH_IRQ_HANDLER(EXTI9_5_IRQHandler)
 {

 CH_IRQ_PROLOGUE();

  // Start of the ISR code

  //LCD_SendString("1"); // just for debug

  // Disable the interrupt
  (EXTI->IMR) = (EXTI->IMR & ~EXTI_IMR_MR5);

   // Clear all the interrupt flags (just in case)
   (EXTI->PR) = EXTI_PR_PR5;


   // If the interruptions is triggered by the clk, we can assure we only have to read the bit
   // So read it
   int32_t bit = (COMMS_PORT->IDR) >> RX_PIN;
   *current_pos = bit;
   current_pos++;

   // Enable the interrupts again
   // Line 5
   (EXTI->IMR) = (EXTI->IMR) | EXTI_IMR_MR5;

   // Clear all the interrupt flags (just in case)
   (EXTI->PR) = EXTI_PR_PR5;

 // End of the ISR code
 CH_IRQ_EPILOGUE();

 }






// Interrupt associated to the RX line
CH_IRQ_HANDLER(EXTI15_10_IRQHandler)
 {

 CH_IRQ_PROLOGUE();

  // Start of the ISR code

  //LCD_SendString("2"); // just for debug

  // Disable the interrupt
  (EXTI->IMR) = (EXTI->IMR & ~EXTI_IMR_MR13);

   // Clear all the interrupt flags (just in case)
   (EXTI->PR) = EXTI_PR_PR13;

   // Differentiate if it is a rising or a falling edge



   if (( (CLK_PORT->IDR >> CLK_RX) & 1)== 1) {

	   if (((((COMMS_PORT->IDR) >> RX_PIN) & 1) == 0)) {

		   // If it is a falling edge and CLK_RX is high --> Start sign

		   // Activate interrupts of RX_CLK

		   //EXTI5: Assign the port B (clk port) to the external interrupt line EXTI5. With the & we are erasing the 4 bits that were
			// previously indicating which port was assigned, and with the | we are
			// indicating the new assigned port
			(SYSCFG->EXTICR[1]) = (SYSCFG->EXTICR[1] & (~SYSCFG_EXTICR2_EXTI5)) | SYSCFG_EXTICR2_EXTI5_PB;
			// Enable interrupt line
			(EXTI->IMR) = (EXTI->IMR) | EXTI_IMR_MR5;
			// Enable interrupt of clk at rising edges
			(EXTI->RTSR) = (EXTI->RTSR) | EXTI_RTSR_TR5;
			// Erase any pending interrupt flag
			(EXTI->PR) = EXTI_PR_PR5;
			nvicEnableVector(EXTI9_5_IRQn , CORTEX_PRIORITY_MASK(STM32_EXT_EXTI5_9_IRQ_PRIORITY ));

			// Clear the previous message (setting the pointer to the initial position of memory)
			current_pos = received_message;

		} else {

			// If it is a rising edge and CLK_RX is high --> End sign
			// Process  the message and show it
			processMessage();

		}
   }



   // Enable the interrupts again
   // Line 13
   (EXTI->IMR) = (EXTI->IMR) | EXTI_IMR_MR13;

   // Clear all the interrupt flags (just in case)
   (EXTI->PR) = EXTI_PR_PR13;




 // End of the ISR code
 CH_IRQ_EPILOGUE();

 }



