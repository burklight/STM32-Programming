
#include "Base.h"     // Basic definitions
#include "mcuconf.h"
#include "encoder.h"
#include "lcd.h"

volatile int32_t count = 0;

void initEncoder(void){
	//GPIO PA1 i PA3 com entrades digitals amb Pull-Up
	GPIOA->MODER &= (~BIT2)&(~BIT3)&(~BIT6)&(~BIT7); 	//entrades

	GPIOA->PUPDR |= BIT2|BIT6;							//pull-UP
	GPIOA->PUPDR &= (~BIT3)&(~BIT7);

	//Activate the SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//Assign port PA1 and PA3 to EXTI1 and EXTI3
	SYSCFG->EXTICR[0] &= (~SYSCFG_EXTICR1_EXTI1)&(~SYSCFG_EXTICR1_EXTI3);

	//Acctivate EXTI1 and EXTI3 on rising and falling edge
	EXTI->RTSR |= EXTI_RTSR_TR1|EXTI_RTSR_TR3;
	EXTI->FTSR |= EXTI_FTSR_TR1|EXTI_RTSR_TR3;

	//Clear the pending interrupt flag at EXTI1 and EXTI3
	EXTI->PR = EXTI_PR_PR1|EXTI_PR_PR3;

	//Enable ONLY the EXTI1 interrupt
	EXTI->IMR |= EXTI_IMR_MR1;

	//Enable EXTI1 and EXTI3 at NVIC
	nvicEnableVector(EXTI1_IRQn,CORTEX_PRIORITY_MASK(STM32_EXT_EXTI1_IRQ_PRIORITY));
	nvicEnableVector(EXTI3_IRQn,CORTEX_PRIORITY_MASK(STM32_EXT_EXTI3_IRQ_PRIORITY));
}

CH_IRQ_HANDLER(EXTI1_IRQHandler)
 {
 CH_IRQ_PROLOGUE();

 //Disable the EXTI1 interrupt
 EXTI->IMR &= (~EXTI_IMR_MR1);

 //Erase the pending interrupt flag
 EXTI->PR = EXTI_PR_PR1;

 if(((GPIOA->IDR)&BIT1)!=BIT1){
	 if(((GPIOA->IDR)&BIT3)==BIT3) count++;
	 else count--;
 }

 //Enable the EXTI3 interrupt
 EXTI->IMR |= EXTI_IMR_MR3;

 //Clear the pending interrupt flag at EXTI3
 EXTI->PR = EXTI_PR_PR3;

 CH_IRQ_EPILOGUE();
 }

CH_IRQ_HANDLER(EXTI3_IRQHandler)
 {
 CH_IRQ_PROLOGUE();

 //Disable the EXTI3 interrupt
 EXTI->IMR &= (~EXTI_IMR_MR3);

 //Erase the pending interrupt flag
 EXTI->PR = EXTI_PR_PR3;

 /*if(((GPIOA->IDR)&BIT3)!=BIT3){
 	 if(((GPIOA->IDR)&BIT1)==BIT1) count--;
 	 else count++;
  }*/

 //Enable the EXTI1 interrupt
 EXTI->IMR |= EXTI_IMR_MR1;

 //Clear the pending interrupt flag at EXTI1
 EXTI->PR = EXTI_PR_PR1;

 CH_IRQ_EPILOGUE();
 }
