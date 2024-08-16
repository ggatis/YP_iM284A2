#include "i2c_face.h"


static uint32_t ReadyTime = 0;


uint8_t i2c_master_test( uint8_t address ) {
    Wire.beginTransmission( address );
    return Wire.endTransmission();
}

uint8_t i2c_master_read( uint8_t addr, uint8_t *data_buf, uint8_t len ) {

    if ( !data_buf ) return 2;                  //pointer?
     
    Wire.requestFrom( addr, len );
    setTimeOut( &ReadyTime, len * ( I2C_SYMBOL_TO - 1 ) );  //wait for a reply
    
    while( !TimedOut( &ReadyTime ) && len ) {
        if ( Wire.available() ) {
            *data_buf++ = Wire.read();
            --len;
            if ( !len ) {
                return 0;                       //OK
            }
        }
    }

    return 3;                                   //TO

}


uint8_t i2c_master_write_byte( uint8_t i2c_addr, uint8_t reg, uint8_t data ) {

    Wire.beginTransmission( i2c_addr );         //transmit to device
    Wire.write( reg );                          //sets register
    Wire.write( data );                         //sends data
    Wire.endTransmission();                     //stop transmitting

    return 0;                                   //OK (not ACK?)

}


uint8_t i2c_master_write_then_read( uint8_t addr, uint8_t reg, uint8_t *data_buf, uint8_t len ) {

    if ( !data_buf ) return 2;                  //pointer?
     
    Wire.beginTransmission( addr );             //transmit to device
    Wire.write( reg );                          //sends one byte
    Wire.endTransmission();                     //stop transmitting

    Wire.requestFrom( addr, len );
    setTimeOut( &ReadyTime, len * ( I2C_SYMBOL_TO - 1 ) );  //wait for a reply
    
    while( !TimedOut( &ReadyTime ) && len ) {
        if ( Wire.available() ) {
            *data_buf++ = Wire.read();
            --len;
            if ( !len ) {
                return 0;                       //OK
            }
        }
    }

    return 3;                                   //TO

}
