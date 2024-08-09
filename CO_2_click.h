#ifndef _CO_2_CLICK_H_
#define _CO_2_CLICK_H_


#include <interface.h>
#include <Wire.h>

extern TwoWire Wire;

//atkariibaa, kur piesleegts
#define ENABLE_CO_2     ENABLE_I2CA
#define I2C_ADDR_CO_2   0x48

#define ENA_WAIT_CO_2   500     //ms wait after enable
#define MEAS_WAIT_CO_2  300     //ms wait before measurement
//#define READY_CO_2      300     //ms or some sensors they read more freuently

void setup_CO_2_HW( void );
void setup_CO_2_SW( void );


#endif // _CO_2_CLICK_H_
