#ifndef _CO_2_CLICK_H_
#define _CO_2_CLICK_H_


#include <interface.h>

//atkariibaa, kur piesleegts
#define ENABLE_CO_2         ENABLE_I2CA
#define I2C_ADDR_CO_2       0x48
#define I2C_ADDR_ADC        0x4D

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

//Registers
#define CO2_STATUS_REG      0x00
#define CO2_LOCK_REG        0x01
#define CO2_TIACN_REG       0x10
#define CO2_REFCN_REG       0x11
#define CO2_MODECN_REG      0x12

//Modes
#define CO2_WRITE_MODE      0x00
#define CO2_READ_ONLY_MODE  0x01

//Resolution
#define CO2_EXT_TIA_RES             0x00
#define CO2_2750_OHM_TIA_RES        0x04
#define CO2_3500_OHM_TIA_RES        0x08
#define CO2_7K_OHM_TIA_RES          0x0C
#define CO2_14K_OHM_TIA_RES         0x10
#define CO2_35K_OHM_TIA_RES         0x14
#define CO2_120K_OHM_TIA_RES        0x18
#define CO2_350K_OHM_TIA_RES        0x1C
#define CO2_10_OHM_LOAD_RES         0x00
#define CO2_33_OHM_LOAD_RES         0x01
#define CO2_50_OHM_LOAD_RES         0x02
#define CO2_100_OHM_LOAD_RES        0x03

//Measurement Values
#define CO2_VREF_INT                0x00
#define CO2_VREF_EXT                0x80
#define CO2_20_PERCENTS_INT_ZERO    0x00
#define CO2_50_PERCENTS_INT_ZERO    0x20
#define CO2_67_PERCENTS_INT_ZERO    0x40
#define CO2_INT_ZERO_BYPASSED       0x60
#define CO2_BIAS_POL_NEGATIVE       0x00
#define CO2_BIAS_POL_POSITIVE       0x10
#define CO2_0_PERCENTS_BIAS         0x00
#define CO2_1_PERCENT_BIAS          0x01
#define CO2_2_PERCENTS_BIAS         0x02
#define CO2_4_PERCENTS_BIAS         0x03
#define CO2_6_PERCENTS_BIAS         0x04
#define CO2_8_PERCENTS_BIAS         0x05
#define CO2_10_PERCENTS_BIAS        0x06
#define CO2_12_PERCENTS_BIAS        0x07
#define CO2_14_PERCENTS_BIAS        0x08
#define CO2_16_PERCENTS_BIAS        0x09
#define CO2_18_PERCENTS_BIAS        0x0A
#define CO2_20_PERCENTS_BIAS        0x0B
#define CO2_22_PERCENTS_BIAS        0x0C
#define CO2_24_PERCENTS_BIAS        0x0D

//On/Off Modes
#define CO2_FET_DIS                 0x00
#define CO2_FET_EN                  0x80
#define CO2_DEEP_SLEEP_MODE         0x00
#define CO2_2_LEAD_MODE             0x01
#define CO2_STANDBY_MODE            0x02
#define CO2_3_LEAD_MODE             0x03
#define CO2_TEMP_MODE_TIA_OFF       0x06
#define CO2_TEMP_MODE_TIA_ON        0x07

#endif // _CO_2_CLICK_H_
