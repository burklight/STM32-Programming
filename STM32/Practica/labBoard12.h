/***************************************************************

 b o a r d 1 2 . h

 Description of board version 1.2

 v1.0 (1/7/2015)

 ***************************************************************/

/************ PERIPHERALS ON STM32F4 DISCOVERY *****************/

/* Four active high LEDs on board */
/* Hal initialization in baseInit() configures the */
/* LEDs in output push-pull mode                   */

#define LEDS_PORT      GPIOD   // LEDs are on port D

#define GREEN_LED_PAD  12      // Green LED Pad  (Left)
#define ORANGE_LED_PAD 13      // Orange LED Pad (Up)
#define RED_LED_PAD    14      // Red LED Pad    (Right)
#define BLUE_LED_PAD   15      // Blue LED Pad   (Down)

#define GREEN_LED_BIT  (1<<12) // Green LED Bit  (Left)
#define ORANGE_LED_BIT (1<<13) // Orange LED Bit (Up)
#define RED_LED_BIT    (1<<14) // Red LED Bit    (Right)
#define BLUE_LED_BIT   (1<<15) // Blue LED Bit   (Down)

/* The board includes one user button */
/* There is a pull-down resistor so it is active at high level */
/* Hal initialization configures the button in input mode      */

#define BUTTON_PORT    GPIOA   // Button is on Port A
#define BUTTON_PAD     0       // Button Pad
#define BUTTON_BIT     1       // Button Bit

/* LIS302DL Accelerometer */
/* It communicates using SPI channel #1                   */
/* Hal initialization configures SPI1 pads to communicate */
/* with the device.                                       */
/* It also configures the INT lines in input mode         */

#define SPI1_PORT       GPIOA
#define SPI1_SCK_PAD    5
#define SPI1_MOSI_PAD   7
#define SPI1_MISO_PAD   6

#define LIS_CS_PORT     GPIOE
#define LIS_CS_PAD      3
#define LIS_CS_BIT      (1<<3)

#define LIS_INT_PORT    GPIOE
#define LIS_INT1_PAD    0
#define LIS_INT2_PAD    1
#define LIS_INT1_BIT    (1)
#define LIS_INT2_BIT    (1<<1)

/* MP45DT02 Microphone */
/* It needs a clock input and generates signal     */
/* encoded in pulse density modulation (PDM)       */
/* The microphone can be read using I2S channel #2 */

#define MIC_CLK_PORT     GPIOB
#define MIC_CLK_PAD      10
#define MIC_CLK_BIT      (1<<10)

#define MIC_PDM_PORT     GPIOC
#define MIC_PDM_PAD      3
#define MIC_PDM_BIT      (1<<3)

/* PC3 can also be used to provide analog mono input at AIN4 */
/* In fact the mic output can be directly routed to AIN4     */
#define AUDIO_AIN4_PORT   GPIOC
#define AUDIO_AIN4_PAD    3
#define AUDIO_AIN4_BIT    (1<<3)

/* CS43L22 Audio DAC */
/* This device is an audio DAC with digital and analog    */
/* input that is connected to a 3.5mm earphone connection */
/* included in the board                                  */

// Reset line for the DAC is in PD4
#define AUDIO_RESET_PORT    GPIOD
#define AUDIO_RESET_PAD     4
#define AUDIO_RESET_BIT     (1<<4)

/* I2C channel #1 configuration port */
#define AUDIO_I2C1_PORT     GPIOB
#define AUDIO_I2C1_SCL_PAD  6
#define AUDIO_I2C1_SDA_PAD  9

/* I2S channel #3 provide digital audio input */
#define AUDIO_I2S3_PORT         GPIOC  // Main I2S3 port
#define AUDIO_I2S3_MCK_PAD      7      // Master clock
#define AUDIO_I2S3_SCK_PAD      10     // Serial clock
#define AUDIO_I2S3_SD_PAD       12     // Serial data

#define AUDIO_I2S3_LRCK_PORT    GPIOA   // WS (LR) Port
#define AUDIO_I2S3_LRCK_PAD     4       // WS (LR) Pad

/* PA4 can also be used to provide analog mono input at AIN1      */
/* That enables using the PA4 DAC output to generate analog sound */
/* but it cannot be done at the same time that I2S3 is operating  */
/* as PA4 is also used to generate the WS signal                  */
#define AUDIO_AIN1_PORT    GPIOA   // AIN1 DAC Port
#define AUDIO_AIN1_PAD     4       // AIN1 DAC Pad
#define AUDIO_AIN1_BIT     (1<<4)  // AIN1 DAC Bit


/****************** PERIPHERALS ON V1.2 BOARD ********************/

/* LCD peripheral */

#define LCD_PORT      GPIOE    // LCD is on Port E

#define LCD_BL_PAD     9    // Backlight Pad (Active high)
#define LCD_E_PAD     10    // Enable Pad
#define LCD_RS_PAD    11    // RS Pad
#define LCD_DB4_PAD   12    // Data 4 Pad
#define LCD_DB5_PAD   13    // Data 5 Pad
#define LCD_DB6_PAD   14    // Data 6 Pad
#define LCD_DB7_PAD   15    // Data 7 Pad

#define LCD_BL_BIT    (1<<9)    // Backlight Bit (Active high)
#define LCD_E_BIT     (1<<10)   // Enable Bit
#define LCD_RS_BIT    (1<<11)   // RS Bit
#define LCD_DB4_BIT   (1<<12)   // Data 4 Bit
#define LCD_DB5_BIT   (1<<13)   // Data 5 Bit
#define LCD_DB6_BIT   (1<<14)   // Data 6 Bit
#define LCD_DB7_BIT   (1<<15)   // Data 7 Bit

/* Keyboard */

#define KEY_PORT     GPIOD  // Keyboard is on port D

#define KEY_ROW1_PAD  0
#define KEY_ROW2_PAD  1
#define KEY_ROW3_PAD  2
#define KEY_ROW4_PAD  3
#define KEY_COL1_PAD  6
#define KEY_COL2_PAD  7
#define KEY_COL3_PAD  8
#define KEY_COL4_PAD  9

#define KEY_ROW1_BIT  1
#define KEY_ROW2_BIT  (1<<1)
#define KEY_ROW3_BIT  (1<<2)
#define KEY_ROW4_BIT  (1<<3)
#define KEY_COL1_BIT  (1<<6)
#define KEY_COL2_BIT  (1<<7)
#define KEY_COL3_BIT  (1<<8)
#define KEY_COL4_BIT  (1<<9)

/* Potentiometer */

#define POT_PORT  GPIOB  // Pot is on port B
#define POT_PAD   0      // Pot is on pad 0
#define POT_BIT   1      // Pot bit is 1

/* Encoder */

#define ENC_PORT    GPIOA // Encoder is on port A
#define ENC_A_PAD   1          // Line A on pad 1
#define ENC_B_PAD   3          // Line B on pad 3
#define ENC_A_BIT   (1<<1)     // Line A bit
#define ENC_B_BIT   (1<<3)     // Line B bit

