/****************************************************

 l c d . h

 P2 LCD Header file

 LCD Connection information is on the lab Board file

 On board version 1.2 R/W is always at "0" so the
 LCD can be written but cannot be read.

*****************************************************/

#ifndef _LCD_H   // Definition so that the file
#define _LCD_H   // is only included once

// LCD Size
#define LCD_ROWS     2       // Number of rows:     2
#define LCD_COLUMNS 16       // Number of columns: 16


/**************  Public function prototypes ***************/

// Backlight configuration
//     on evaluates to TRUE   Turn on backlight
//     on evaluates to FALSE  Turn off backlight
void LCD_Backlight(int32_t on);

// Init the LCD in 4 bits mode
void LCD_Init(void);

// Clear the LCD and set the cursor position at 0,0
void LCD_ClearDisplay(void);

// Configures the display
//     If Disp is TRUE turn on the display, if not, turns off
//     If Cursor is TRUE show the cursor, if not, hides it
//     If Blink is TRUE turn on blinking, if not, deactivate blinking
void LCD_Config(int32_t Disp,int32_t Cursor,int32_t Blink);

// Set the cursor at the given position
//    col: Columnn (0..LCD_COLUMNS-1)
//    row: Row     (0..LCD_ROWS-1)
void LCD_GotoXY(int32_t col,int32_t row);

// Send a character to the LCD at the current position
//     car: Charater to send
void LCD_SendChar(char car);

// Send a string to the LCD at the current position
//     string: String to send
void LCD_SendString(char *string);


#endif //_LCD_H


