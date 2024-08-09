#ifndef _YP_iM284A2_H_
#define _YP_iM284A2_H_


#include "interface.h"  //pin definitions

#include <stdint.h>

extern volatile uint32_t mySysTick;
extern volatile uint32_t lastIOticks[3];

void setTimeOut( uint32_t* pTimeOutTime, uint32_t timeout );
bool TimedOut( uint32_t* pTimeOutTime );

#include "Pipeline.h"

/* Buffers and Pipelines */
/*
0 - SerialUSB: PC, debug
1 - Serial1:   Lora module
2 - Serial2:   UART2
3 - not used, just for comfort
4 - Serial4:   UART4
5 - Serial5:   UART5
6 - I2CA
7 - I2CB
8 - I2CC
*/
#define SERIALUSB   0
#define SERIAL1IN   1
#define SERIAL2IN   2
#define SERIAL4IN   4
#define SERIAL5IN   5
#define I2CAIN      6
#define I2CBIN      7
#define I2CCIN      8
#define SERIAL1OUT 11
#define SERIAL2OUT 12
#define SERIAL4OUT 14
#define SERIAL5OUT 15
#define CPIPELINES 16

extern Pipeline* pPipelines[CPIPELINES];

//The Data Report size
#define UAR2_SIZE 80
#define UAR4_SIZE 4
#define UAR5_SIZE 4
#define I2CA_SIZE 2
#define I2CB_SIZE 2
#define I2CC_SIZE 2
#define DATA_SIZE ( UAR2_SIZE + UAR4_SIZE + UAR5_SIZE + I2CA_SIZE + I2CB_SIZE + I2CC_SIZE )

#endif // _YP_iM284A2_H_
