#ifndef _I2C_FACE_H_
#define _I2C_FACE_H_


#include "YP_iM284A2.h"     //global data

#ifndef I2C_SYMBOL_TO
#define I2C_SYMBOL_TO 2     //ms, very long time for 100k speed, it can not be 1!
#endif

//Return:
//0 - OK
//2 - wrong data pointer (nullptr)
//3 - TO
uint8_t i2c_master_read( uint8_t addr, uint8_t *data_buf, uint8_t len );
uint8_t i2c_master_write_byte( unit8_t i2c_addr, unit8_t reg, unit8_t data );
uint8_t i2c_master_write_then_read( uint8_t addr, uint8_t reg, uint8_t *data_buf, uint8_t len );


#endif // _I2C_FACE_H_
