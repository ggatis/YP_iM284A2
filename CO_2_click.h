#ifndef _CO_2_CLICK_H_
#define _CO_2_CLICK_H_


#include <interface.h>
#include <Wire.h>

extern TwoWire Wire;

//atkariibaa, kur piesleegts
#define ENABLE_CO_2         ENABLE_I2CA
#define I2C_ADDR_CO_2       0x48
#define I2C_ADDR_CO_2_ADC   0x4D

#define POWER_ON_WAIT_CO_2  500     //ms wait after power on
#define SETUP_WAIT_CO_2     1000    //ms wait after sensor setup before measurement
#define MEAS_WAIT_CO_2      300     //ms wait before next measurement
//#define READY_CO_2          300     //ms, some sensors they read more frequently

void setup_CO_2_HW( void );
void setup_CO_2_SW( void );

//Error Code
#define CO2_RETVAL          uint8_t
#define CO2_OK              0x00
#define CO2_INIT_ERROR      0xFF

//Modes
#define CO2_WRITE_MODE      0x00
#define CO2_READ_ONLY_MODE  0x01


#endif // _CO_2_CLICK_H_
