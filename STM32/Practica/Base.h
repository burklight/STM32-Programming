/*********************************************************************
  B a s e . h

  Version 1.1 (9/6/2015)

 Main include file:
    - Includes references to HAL and OS includes
    - Includes bitfield definitions
    - Includes STM32F4 Discovery definitions

 Additional data:
    - MCU frequency is configured using the main PLL
      The mcuconf.h file includes the configuration information
      PLL input is HSE clock at 8MHz
         fvco=fpll_in*PLLN/PLLM = 8MHz*336/8=336MHz
         HCLK=fpll_out=fvco/PLLP = 336MHz/2=168MHz
         fusb_sdio_rng=fvco/PLLQ = 336MHz/7=48MHz
    - APB1 clock frequency depends on prescaler STM32_PPRE1
      Current value is STM32_PPRE1_DIV4 that corresponds to
         HCLK/4 = 168MHz / 4 = 42 MHz
    - APB2 clock frequency depends on prescaler STM32_PPRE2
      Current value is STM32_PPRE2_DIV2 that corresponds to
         HCLK/2 = 168MHz / 2 = 84 MHz
    - I2S clock frequency is generated on the PLLI2S PLL
      This PLL has the same input as the main PLL
         fvco=fpll_in*PLLI2SN/PLLM=8MHz*192/8=192MHz
         I2CCLK=fvco/PLLI2SR=192MHz/5=38.4MHz

*********************************************************************/

#ifndef _BASE_INCLUDE
#define _BASE_INCLUDE

// Other needed includes

#include "ch.h"             // Main ChibiOS include file
#include "hal.h"            // Main HAL include file
#include "labBoard12.h"     // Laboratory Board v1.2 definitions

/************************ Other definitions ***************************/

// Generic bit definitions

#define BIT(n) (1<<(n))

// Bit definitions in a 32 bit word

#define BIT0   0x01
#define BIT1   0x02
#define BIT2   0x04
#define BIT3   0x08
#define BIT4   0x10
#define BIT5   0x20
#define BIT6   0x40
#define BIT7   0x80
#define BIT8   0x100
#define BIT9   0x200
#define BIT10  0x400
#define BIT11  0x800
#define BIT12  0x1000
#define BIT13  0x2000
#define BIT14  0x4000
#define BIT15  0x8000
#define BIT16  0x10000
#define BIT17  0x20000
#define BIT18  0x40000
#define BIT19  0x80000
#define BIT20  0x100000
#define BIT21  0x200000
#define BIT22  0x400000
#define BIT23  0x800000
#define BIT24  0x1000000
#define BIT25  0x2000000
#define BIT26  0x4000000
#define BIT27  0x8000000
#define BIT28  0x10000000
#define BIT29  0x20000000
#define BIT30  0x40000000
#define BIT31  0x80000000

// ChibiOS blocking

#define DISABLE  chSysLock()
#define ENABLE   chSysUnlock()

// Delay definitions

// Busy waiting for the specified number of microseconds
#define DELAY_US(x) gptDelayUs(x)

// Busy waiting for the specified number of microseconds
// This function is independent on the previous one so
// it can be used at the same time in other thread
#define DELAY_US2(x) gptDelayUs2(x)

// Sleep for the indicated number of milliseconds
// In a multithreaded application the sleep makes the
// CPU available to other threads
#define SLEEP_MS(x) chThdSleep(x)

/********************* FUNCTION PROTOTYPES ******************/

/// Main initializations
// It has to be called at the start of main
void baseInit(void);

// Busy delay function that uses GPT5
void gptDelayUs(uint32_t time);

// Busy delay function that uses GPT9
void gptDelayUs2(uint32_t time);

#endif // Base.h



