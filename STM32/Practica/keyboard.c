#include "Base.h"
#include "keyboard.h"
#include "lcd.h"

volatile int32_t int_key=-1;

void initKeyboard(void){
	uint32_t i;

	for(i=0;i<4;i++){
		(GPIOD->MODER) |= BIT(2*i);
		(GPIOD->OTYPER) |= BIT(i);
	}

	for(i=6;i<10;i++){
		(GPIOD->PUPDR) |= BIT(2*i);
	}
}


int32_t readKeyboard(void){
	//Just to remember it ODR (output) and IDR (input)

	uint32_t i, j, k;
	
	//We put all the rows to '1'
	for(i=0; i<4; i++){
		(GPIOD->ODR) |= BIT(i);
	}

	for(j=0; j<4; j++){
		//We prove row by row searching any '0' in a column
		(GPIOD->ODR) &= (~BIT(j));
		//We wait in order to don't have problems in the reading
		DELAY_US(10);
		for(k=6; k<10; k++){
			//If the column is '0' we set the value
			if(!((GPIOD->IDR)&BIT(k))){
				(GPIOD->ODR) |= BIT(j);
				//We return the value with the codification proposed
				return(4*j+k-6);
			}
		}
		//If this row does not match, we put it again to '1'
		(GPIOD->ODR) |= BIT(j);
	}
	return -1;
}

void intConfigKeyboard(void){

	uint32_t i;
	
	//Activate the SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//We put all the rows to '0' in order to be able to detect
	for(i=0; i<4; i++){
		(GPIOD->ODR) &= (~BIT(i));
	}

	//We put as interruption lines 6 to 9
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PD;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PD;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PD;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PD;

	for(i=6; i<10; i++){
		//Enable the EXTI6, EXTI7, EXTI8 and EXTI9 interrupt
		EXTI->IMR |= BIT(i);
		//Acctivate EXTI6, EXTI7, EXTI8 and EXTI9 on falling edge
		EXTI->RTSR &= (~BIT(i));
		EXTI->FTSR |= BIT(i);
	}

	//Clear the pending interrupt flag at EXTI6, EXTI7, EXTI8 and EXTI9
	EXTI->PR = BIT(6)|BIT(7)|BIT(8)|BIT(9);

	//Enable EXTI6, EXTI7, EXTI8 and EXTI9 at NVIC
	nvicEnableVector(EXTI9_5_IRQn,CORTEX_PRIORITY_MASK(STM32_EXT_EXTI5_9_IRQ_PRIORITY));
}

CH_IRQ_HANDLER(EXTI9_5_IRQHandler)
 {
 CH_IRQ_PROLOGUE();

 uint32_t i;

 //Mask the interruptions
 for(i=0; i<10; i++){
	(EXTI->IMR) &= (~BIT(i));
 }

 //Read the key
 int_key = readMultikey();
 
 //Erase the pending interrupt flag
 EXTI->PR = BIT(6)|BIT(7)|BIT(8)|BIT(9);

 //We put all the rows to '0' in order to be able to detect
 for(i=0; i<4; i++){
	(GPIOD->ODR) &= (~BIT(i));
 }
 
 for(i=6; i<10; i++){
	//Enable the EXTI6, EXTI7, EXTI8 and EXTI9 interrupt
	EXTI->IMR |= BIT(i);
 }

 CH_IRQ_EPILOGUE();
 }


void writeKeyboard(void){
	char keys[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	int32_t prev = 0;
	while(1){
		while(int_key==-1);
		int32_t aux_key = int_key;
		LCD_GotoXY(prev,0);
		LCD_SendChar(' ');
		LCD_GotoXY(aux_key,0);
		LCD_SendChar(keys[aux_key]);
		prev = aux_key;
		int_key = -1;
	}
}

void polling(void){
	char keys[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	int32_t prev = 0;
	while(1){
		SLEEP_MS(500);
		int32_t aux_key = readKeyboard();
		LCD_GotoXY(prev,0);
		LCD_SendChar(' ');
		LCD_GotoXY(aux_key,0);
		LCD_SendChar(keys[aux_key]);
		prev = aux_key;
		int_key = 32;
	}
}

int32_t readMultikey(void){
	//Just to remember it ODR (output) and IDR (input)

	int32_t i, j, k, sum = 0;

	//We put all the rows to '1'
	for(i=0; i<4; i++){
		(GPIOD->ODR) |= BIT(i);
	}

	for(j=0; j<4; j++){
		//We prove row by row searching any '0' in a column
		(GPIOD->ODR) &= (~BIT(j));
		//We wait in order to don't have problems in the reading
		DELAY_US(10);
		for(k=6; k<10; k++){
			//If the column is '0' we set the value
			if(!((GPIOD->IDR)&BIT(k))){
				(GPIOD->ODR) |= BIT(j);
				//We return the value with the codification proposed
				sum |= BIT(4*j+k-6);
			}
		}
		//If this row does not match, we put it again to '1'
		(GPIOD->ODR) |= BIT(j);
	}
	return sum;
}

void writeMultikey(void){
	char keys[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	int32_t i = 0;
	while(1){
		while(int_key==-1);
		int32_t aux_key = int_key;
		LCD_ClearDisplay();
		for(i=0;i<16;i++){
			if((aux_key & BIT(i)) == BIT(i)){
				LCD_GotoXY(i,0);
				LCD_SendChar(keys[i]);
			}
		}
		int_key = -1;
	}
}

void calculator(void){
	int32_t cont=0;
	int32_t neg=0;
	int32_t num1=1;
	int32_t num2=1;
	int32_t res;
	int32_t op=1;
	int32_t key=-1;
	int32_t delay=150;
	char data[16];
	char keys[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	while(1){
		LCD_ClearDisplay();
		LCD_GotoXY(0,0);
		num1=1;
		num2=1;
		//1st number introduction
		do{
			key=readKeyboard();
			SLEEP_MS(delay);
		}while(key==-1);
		if(keys[key]>='0' && keys[key]<='9'){
			num1=keys[key]-'0';
			itoa(num1,data,10);
			LCD_SendString(data);
			cont++;
		}
		while(keys[key]>='0' && keys[key]<='9'){
			do{
				key=readKeyboard();
				SLEEP_MS(delay);
			}while(key==-1);
			if(keys[key]>='0' && keys[key]<='9'){
				num1=10*num1+keys[key]-'0';
				itoa(num1,data,10);
				LCD_GotoXY(0,0);
				LCD_SendString(data);
				cont++;
			}
		}

		//negative number
		while(keys[key]=='#'){
			num1*=-1;
			cont += ((neg++)%2);
			itoa(num1,data,10);
			LCD_GotoXY(0,0);
			LCD_SendString(data);
			LCD_SendChar(' ');
			do{
				key=readKeyboard();
				SLEEP_MS(delay);
			}while(key==-1);
		}

		LCD_SendChar(' ');
		cont++;

		//operation introduction
		while(keys[key]<'A' && keys[key]>'D'){
			do{
				key=readKeyboard();
				SLEEP_MS(delay);
			}while(key==-1);
		}

		while(keys[key]>='A' && keys[key]<='D'){
			do{
				if(keys[key]=='A'){
					LCD_GotoXY(cont,0);
					LCD_SendChar('+');
					op=1;
				}
				else if(keys[key]=='B'){
					LCD_GotoXY(cont,0);
					LCD_SendChar('-');
					op=2;
				}
				else if(keys[key]=='C'){
					LCD_GotoXY(cont,0);
					LCD_SendChar('*');
					op=3;
				}
				else if(keys[key]=='D'){
					LCD_GotoXY(cont,0);
					LCD_SendChar('/');
					op=4;
				}
				else if(keys[key]=='#'){
					num1*=-1;
					itoa(num1,data,10);
					LCD_GotoXY(0,0);
					LCD_SendString(data);
					cont += ((neg++)%2);
				}
				key=readKeyboard();
				SLEEP_MS(delay);
			}while(key==-1);
		}

		//2nd number introduction
		//do{
		//	key = readKeyboard();
		SLEEP_MS(delay);
		//}while(key==-1);
		if(keys[key]>='0' && keys[key]<='9'){
			num2=keys[key]-'0';
			itoa(num2,data,10);
			LCD_GotoXY(0,1);
			LCD_SendString(data);
		}
		while(keys[key]>='0' && keys[key]<='9'){
			do{
				key = readKeyboard();
				SLEEP_MS(delay);
			}while(key==-1);
			if(keys[key]>='0' && keys[key]<='9'){
				num2=10*num2+keys[key]-'0';
				itoa(num2,data,10);
				LCD_GotoXY(0,1);
				LCD_SendString(data);
			}
		}


		//after the calculation every key acts as an equal and the
		//result is shown
		if(op==1){
			res=num1+num2;
		}
		else if(op==2){
			res=num1-num2;
		}
		else if(op==3){
			res=num1*num2;
		}
		else if(op==4){
			res=num1/num2;
		}

		LCD_ClearDisplay();
		LCD_GotoXY(0,0);
		itoa(res,data,10);
		LCD_SendString("= ");
		LCD_SendString(data);
		cont=0;

		//we wait until any key is pressed
		do{
			key = readKeyboard();
			SLEEP_MS(delay);
		}while(key==-1);
	}

}

