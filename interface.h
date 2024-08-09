#ifndef _INTERFACE_H_
#define _INTERFACE_H_


#include <Arduino.h>

//just comment improper one or both to get v67 board definitions
#define BOARD_VERSION 6
//#define BOARD_VERSION 7

#ifndef BOARD_VERSION
#define BOARD_VERSION 67    //pached v6 board
#endif


#if 6 == BOARD_VERSION


#define UART1_RX    PA10
#define UART1_TX    PA9
#define UART2_RX    PA_3_ALT1
#define UART2_TX    PA_2_ALT1
#define UART4_RX    PA1
#define UART4_TX    PA0
#define UART5_RX    PB4
#define UART5_TX    PB3

#define WIRE_SDA    PB7
#define WIRE_SCL    PB6

#define ENABLE_UAR2 PA12    //USB_DP
#define ENABLE_UAR4 PA11    //USB_DM
#define ENABLE_UAR5 PA8

#define ENABLE_I2CA PB5
#define ENABLE_I2CB PA15
#define ENABLE_I2CC PA14

#define RADIO_PPS   PA7
#define RADIO_GPIO2 PA6
#define RADIO_WAKE  PA5
#define RADIO_GPIO1 PA4

#define RADIO_UART  1
#define RADIO_RX    UART1_RX
#define RADIO_TX    UART1_TX

#define SSENS1_UART 2
#define SSENS1_RX   UART2_RX
#define SSENS1_TX   UART2_TX

#define SSENS2_UART 4
#define SSENS2_RX   UART4_RX
#define SSENS2_TX   UART4_TX

#define SSENS5_UART 5
#define SSENS5_RX   UART5_RX
#define SSENS5_TX   UART5_TX

#define IWIRE_I2C   0   //default aka 1st


#elif 67 == BOARD_VERSION


#define UART1_RX    PA10
#define UART1_TX    PA9
#define UART2_RX    PA_3_ALT1
#define UART2_TX    PA_2_ALT1
#define UART4_RX    PA1
#define UART4_TX    PA0
#define UART5_RX    PB4
#define UART5_TX    PB3

#define WIRE_SDA    PB7
#define WIRE_SCL    PB6

#define ENABLE_UAR2 PB0
#define ENABLE_UAR4 PB1
#define ENABLE_UAR5 PA8

#define ENABLE_I2CA PB5
#define ENABLE_I2CB PA15
#define ENABLE_I2CC PA14

#define RADIO_PPS   PA7
#define RADIO_GPIO2 PA6
#define RADIO_WAKE  PA5
#define RADIO_GPIO1 PA4

#define RADIO_UART  1
#define RADIO_RX    UART1_RX
#define RADIO_TX    UART1_TX

#define SSENS1_UART 2
#define SSENS1_RX   UART2_RX
#define SSENS1_TX   UART2_TX

#define SSENS2_UART 4
#define SSENS2_RX   UART4_RX
#define SSENS2_TX   UART4_TX

#define SSENS5_UART 5
#define SSENS5_RX   UART5_RX
#define SSENS5_TX   UART5_TX

#define IWIRE_I2C   0   //default aka 1st


#else                   //v7 board


#define UART1_RX    PA10
#define UART1_TX    PA9
#define UART2_RX    PA_3_ALT1
#define UART2_TX    PA_2_ALT1
#define UART4_RX    PA1
#define UART4_TX    PA0
#define UART5_RX    PB4
#define UART5_TX    PB3

#define WIRE_SDA    PB7
#define WIRE_SCL    PB6

#define ENABLE_UAR2 PB0
#define ENABLE_UAR4 PB1
#define ENABLE_UAR5 PA8

#define ENABLE_I2CA PB5
#define ENABLE_I2CB PA15
#define ENABLE_I2CC PA14

#define RADIO_PPS   PA7
#define RADIO_GPIO2 PA6
#define RADIO_WAKE  PA5
#define RADIO_GPIO1 PA4

#define RADIO_UART  4
#define RADIO_RX    UART4_RX
#define RADIO_TX    UART4_TX

#define SSENS1_UART 2
#define SSENS1_RX   UART2_RX
#define SSENS1_TX   UART2_TX

#define SSENS2_UART 1
#define SSENS2_RX   UART1_RX
#define SSENS2_TX   UART1_TX

#define SSENS5_UART 5
#define SSENS5_RX   UART5_RX
#define SSENS5_TX   UART5_TX

#define IWIRE_I2C   0   //default aka 1st


#endif // BOARD_VERSION


#endif // _INTERFACE_H_

