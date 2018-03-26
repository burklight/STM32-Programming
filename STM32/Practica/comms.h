/*
 * comms.h
 *
 *  Full duplex serial synchronous communication
 *
 */

#ifndef COMMS_H_
#define COMMS_H_

#define COMMS_PORT GPIOC   // Communications port
#define CLK_PORT GPIOB     // Clocks port

#define TX_PIN   15
#define RX_PIN   13

#define CLK_TX    7
#define CLK_RX    5

#define MAX_LENGTH   256 // Maximum length of a message (256 bits)

#define T   1000  // Clk period (in us)

void initComms(void); // Initialization of the board to communicate between boards

void sendMessage(char *string); // Send a string to the receiver


#endif /* COMMS_H_ */
