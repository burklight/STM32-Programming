#include "Base.h"
#include "keyboard.h"

void initADC(void){
	//Activate the peripheric clock. 
	//ADC1EN is APB2ENR 8th bit.
	(RCC->APB2ENR) |= RCC_APB2ENR_ADC1EN;

	//For 3V of supply the frequency is between 30 and 36MHz.
	//As fpclk = 60MHz we should work with fpclk/4. Which means that
	//we need the value of ADCPRE = 01 (16th and 17th bits).
	(ADC->CCR) |= BIT16;
	(ADC->CCR) &= (~BIT17);

	//Lecture time to ensure a stable value before converting it.
	(ADC1->SMPR2) = ((ADC1->SMPR2)|BIT25)&(~BIT24)&(~BIT26); 	//8th channel 010 -> 28 CYCLES
	(ADC1->SMPR1) |= BIT20|BIT19|BIT18; 		//16th channel (more than 10us)
	(ADC1->SMPR1) |= BIT23|BIT22|BIT21; 		//17th channel (more than 10us)

	//Activate temperature sensor (TSVREFE: BIT23) 
	(ADC->CCR) |= BIT23;

	//Activate analog mode (GPIOB->MODER = 11)
	//Disconnect it's push-pull (GPIOB->OTYPER = 0000000000000000)
	(GPIOB->MODER) |= BIT0|BIT1;
	(GPIOB->OTYPER) &= (~BIT0)&(~BIT2)&(~BIT3)&(~BIT4)&(~BIT5)&(~BIT6)&(~BIT7)&(~BIT8);
	(GPIOB->OTYPER) &= (~BIT9)&(~BIT10)&(~BIT11)&(~BIT12)&(~BIT13)&(~BIT14)&(~BIT15);

	//Activate the ADC (ADC->CR2 ADON: BIT0)
	(ADC1->CR2) |= BIT0;
}

int32_t readIN8(void){
	//Activate the 8 channel conversion (ADC1->SQR3 : 00010)
	(ADC1->SQR3)=((ADC1->SQR3)|BIT3)&(~BIT4)&(~BIT2)&(~BIT1)&(~BIT0);

	//Start conversion (ADC1->CR2 SWSTART: BIT30)
	(ADC1->CR2) |= BIT30;

	//Wait until the conversion ends (ADC->SR EOC = 1)
	while(((ADC1->SR)&BIT1)==0);

	//return value
	int32_t res = (ADC1->DR);
	return res;
}

int32_t readChannel(int32_t channel){
	//Activate the chosen channel
	(ADC1->SQR3)=((ADC1->SQR3)&(~BIT3)&(~BIT4)&(~BIT2)&(~BIT1)&(~BIT0))^channel;

	//Start conversion (ADC1->CR2 SWSTART: BIT30)
	(ADC1->CR2) |= BIT30;

	//Wait until the conversion ends (ADC->SR EOC = 1)
	while(((ADC1->SR)&BIT1)==0);

	//return value
	int32_t res = (ADC1->DR);
	return res;
}

int32_t readT(void){
	int32_t T, temp, Vs;
	temp = readChannel(16);
	//Knowing that Vdd = 3V and the ADC values go from 0 to 4095
	//We convert both Vs and T to be able to show the result 
	//We multiply for 10000 and then divide for 100 to avoid errors and work 
	//with integers (for example because V25 = 0.76V)
	Vs=30000*temp/4095; 
	T=250000+400*(Vs-7600); 
	return T/1000;
}

int32_t readVdd(void){
	//We use the typical reference voltage 1.21V
	int32_t Vdd;
	Vdd=1210*4096/readChannel(17);
	return Vdd;
}
