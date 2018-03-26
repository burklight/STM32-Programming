/****************************************************

 l c d . c

 P2 LCD Functions

*****************************************************/

#include "Base.h"  // Basic definitions
#include "lcd.h"   // LCD definitions

// Some of the function need to be completed
// as is requested on the manual

/***************** PRIVATE FUNCTIONS TO BE IMPLEMENTED *************
 In C, all functions are public by default.
 Those functions, as they are not needed outside of lcd.c
 won't have prototypes on lcd.h.
 They will also be declared static so that the compiler can
 optimize their usage knowing they are not used outside this file.
****************************************************************/


// Initializes DB7..DB4, RS, E i BL in Push-Pull output mode
// and sets the ouptuts to "0"
static void lcdGPIOInit(void)
 {
	uint32_t count;
	/*for(count=9;count<16;count++){
		GPIOE->MODER = (GPIOE->MODER)|BIT(2*count);
		GPIOE->MODER = (GPIOE->MODER)&(~BIT(2*count+1));
	    (GPIOE->BSRR.H.clear) = BIT(count);
	}*/

	//Improved optioal implementation
	for(count=9;count<16;count++){
		GPIO_ModePushPull(&(GPIOE->MODER),count);
	}

 }

GPIO_ModePushPull(GPIO_TypeDef *port, int32_t linia)
{
	(*(int32_t *)port) = (*(int32_t *)port)|BIT(2*linia);
	(*(int32_t *)port) = (*(int32_t *)port)&(~BIT(2*linia+1));
	(*(int32_t *)port) &= ~BIT(linia);
}

// Send 4 bits to the LCD and generates an enable pulse
//     nibbleCmd : Bits 3..0 Nibble to send to DB7...DB4
//     RS        : TRUE (RS="1")   FALSE (RS="0")
static void lcdNibble(int32_t nibbleCmd,int32_t RS)
 {
	uint32_t i, var;
	var = LCD_DB4_BIT;

	for(i=0;i<4;i++){
		if(nibbleCmd%2)
				(GPIOE->BSRR.H.set) = var;
	    else
				(GPIOE->BSRR.H.clear) = var;
		nibbleCmd = nibbleCmd/2;
		var = var << 1;
	}

	if(RS)
		(GPIOE->BSRR.H.set) = LCD_RS_BIT;
	else
		(GPIOE->BSRR.H.clear) = LCD_RS_BIT;

	DELAY_US(10);
	(GPIOE->BSRR.H.set) = LCD_E_BIT;
	DELAY_US(10);
	(GPIOE->BSRR.H.clear) = LCD_E_BIT;
	DELAY_US(10);


 
 }


/********** PUBLIC FUNCTIONS TO IMPLEMENT ************************
 Those functions will be used outside of lcd.c so there will
 be prototypes for them in lcd.h.
******************************************************************/

// Backlight configuration
//     on evaluates to TRUE   Turn on backlight
//     on evaluates to FALSE  Turn off backlight
void LCD_Backlight(int32_t on)
 {
	if(on)
		(GPIOE->BSRR.H.set) = BIT(LCD_BL_PAD);
	else
		(GPIOE->BSRR.H.clear) = BIT(LCD_BL_PAD);
 }

// Clear the LCD and set the cursor position at 0,0
void LCD_ClearDisplay(void)
 {
	lcdNibble(0,0);
	lcdNibble(1,0);
	//DELAY_US(40);
	SLEEP_MS(5);
 }

// Configures the display
//     If Disp is TRUE turn on the display, if not, turns off
//     If Cursor is TRUE show the cursor, if not, hides it
//     If Blink is TRUE turn on blinking, if not, deactivate blinking
void LCD_Config(int32_t Disp,int32_t Cursor,int32_t Blink)
 {
	int32_t mode = 8;
	if(Disp)
		mode = mode + 4;
	if(Cursor)
		mode = mode + 2;
	if(Blink)
		mode = mode + 1;

	lcdNibble(0,0);
	lcdNibble(mode,0);
	DELAY_US(40);
 }

// Set the cursor at the given position
//    col: Columnn (0..LCD_COLUMNS-1)
//    row: Row     (0..LCD_ROWS-1)
void LCD_GotoXY(int32_t col,int32_t row)
 {
	uint32_t pos;
	pos = 64*row + col;

	uint8_t hnum;
	uint8_t lnum;

	//DB7 is always "1"
	hnum = (((uint8_t)pos)/16) + 8;
	lnum = ((uint8_t)pos)%16;

	//we send the high bits and then the low bits
	lcdNibble((uint32_t)hnum,0);
	lcdNibble((uint32_t)lnum,0);

	//we wait to avoid problems
	DELAY_US(40);

 }

// Send a character to the LCD at the current position
//     car: Charater to send
void LCD_SendChar(char car)
 {
	uint8_t hnum;
	uint8_t lnum;

	//we take the first 4 bits and last 4 bits of the character
	hnum = ((uint8_t)car)/16;
	lnum = ((uint8_t)car)%16;

	//we send the high bits and then the low bits
	lcdNibble((uint32_t)hnum,1);
	lcdNibble((uint32_t)lnum,1);

	//we wait to avoid problems
	DELAY_US(40);

 
 }

// Send a string to the LCD at the current position
//     string: String to send
void LCD_SendString(char *string)
 {
	char c = *string;
	while(c  != '\0')
	{
	LCD_SendChar(c);
	string++;
	c=*string;
	}


 }

/********** PUBLIC FUNCTIONS ALREADY IMPLEMENTED ***************
 Those functions are provided to reduce the codding effort
 for the LCD module
****************************************************************/

// Init the LCD in 4 bits mode
// This function depends on the implementation of the
// private functions lcdSendNibble and lcdGPIOInit
void LCD_Init(void)
 {
 // Initializes the LCD GPIOs
 lcdGPIOInit();

 // Wait 50ms for vdd to stabilize
 SLEEP_MS(50);

 // Init command 1
 lcdNibble(0x03,0);
 SLEEP_MS(10); // Wait > 4.1ms

 // Repeat command
 lcdNibble(0x03,0);

 // Wait > 100us
 DELAY_US(500);

 // Repeat command
 lcdNibble(0x03,0);

 // Init command 2
 lcdNibble(0x02,0);

 // Mode 4 bits 2 lines 5x8 chars
 lcdNibble(0x08,0);
 lcdNibble(0x02,0);
 DELAY_US(50);

 // Display on blink on and cursor on
 lcdNibble(0x00,0);
 lcdNibble(0x0E,0);
 DELAY_US(50);

 // Clear Display
 lcdNibble(0x00,0);
 lcdNibble(0x01,0);
 SLEEP_MS(5);

 // Cursor set direction
 lcdNibble(0x00,0);
 lcdNibble(0x06,0);
 DELAY_US(50);

 // Set ram address
 lcdNibble(0x08,0);
 lcdNibble(0x00,0);
 DELAY_US(50);

 // Send OK
 /*
 lcdNibble(0x04,1);
 lcdNibble(0x0F,1);
 DELAY_US(50);
 lcdNibble(0x04,1);
 lcdNibble(0x0B,1);
 DELAY_US(50);
 */
 }

void LCD_CustomChar(int32_t pos,uint8_t *data)
{
int32_t mode = 64, i;
if(pos%2)
	mode += 8;
pos = pos/2;
if(pos%2)
	mode += 16;
pos = pos/2;
if(pos%2)
	mode += 32;

uint8_t hnum;
uint8_t lnum;

//we pass the data in each matrix address
for(i=0;i<8;i++)
{

//we take the first 4 bits and last 4 bits of the mode
hnum = ((uint8_t)mode)/16;
lnum = ((uint8_t)mode)%16;

//we send the high bits and then the low bits
 lcdNibble((uint32_t)hnum,0);
 lcdNibble((uint32_t)lnum,0);

DELAY_US(40);

//Pass the data
 lcdNibble(data[i]>>4,1);
 lcdNibble(data[i]&15,1);

DELAY_US(40);

mode++;
}

//return to DDRAM
lcdNibble(8,0);
lcdNibble(0,0);
LCD_GotoXY(0,0);
}






