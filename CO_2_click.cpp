#include "YP_iM284A2.h" //global data
#include "CO_2_click.h"
#include "Pipes.h"
#include "Pipeline.h"
//#include "CircularBuffer.h"

/*
#define ENABLE_CO_2         ENABLE_I2CA
#define I2C_ADDR_CO_2       0x48
#define I2C_ADDR_CO_2_ADC   0x4D

#define MEAS_WAIT_CO_2      300     //ms wait before measurement
*/


static uint32_t ReadyTime = 0;
//static float    CO_ppm;


#define I2C_SYMBOL_TO       2   //ms, very long time for 100k speed, it can not be 1!


//Wire -> uz kopeeju h/cpp
uint8_t i2c_master_read( uint8_t addr, uint8_t *data_buf, uint8_t len ) {

    uint32_t    ReadyTime;

    if ( !data_buf ) return 2;                  //pointer?
     
    Wire.requestFrom( addr, len );
    setTimeOut( &ReadyTime, len * 1 + 1 );      //wait for a reply
    
    while( !TimedOut( &ReadyTime ) && len ) {
        if ( Wire.available() ) {
            *data_buf++ = Wire.read();
            --len;
            if ( !len ) {
                return 0;
            }
        }
    }

    return 3;                                   //TO

}


uint8_t i2c_master_write_then_read( uint8_t addr, uint8_t reg, uint8_t *data_buf, uint8_t len ) {

    uint32_t    ReadyTime;

    if ( !data_buf ) return 2;                  //pointer?
     
    Wire.beginTransmission( addr );             //transmit to device
    Wire.write( reg );                          //sends one byte
    Wire.endTransmission();                     //stop transmitting

    setTimeOut( &ReadyTime, len * 1 + 1 );      //wait for a reply
    Wire.requestFrom( addr, len );
    
    while( !TimedOut( &ReadyTime ) && len ) {
        if ( Wire.available() ) {
            *data_buf++ = Wire.read();
            --len;
            if ( !len ) {
                return 0;
            }
        }
    }

    return 3;                                   //TO

}


//CO_2_Click specific
uint8_t co2_generic_write_byte( uint8_t reg, uint8_t data ) {

    if ( ( reg < 0x01 ) || ( reg > 0x01 && reg < 0x10 ) || ( reg > 0x12 ) ) {
        return 1;
    }

    //co2_enable( ctx, CO2_DEVICE_EN );           //RST should be to the ground
    Wire.beginTransmission( I2C_ADDR_CO_2 );    //transmit to device
    Wire.write( reg );                          //sends one byte
    Wire.write( data );                         //sends one byte
    Wire.endTransmission();                     //stop transmitting
    //co2_enable( ctx, CO2_DEVICE_DIS );        //RST should be to the ground

    return 0;
}

uint8_t co2_generic_read_byte( uint8_t reg, uint8_t *data_buf ) {

    if ( ( reg > 0x01 && reg < 0x10 ) || ( reg > 0x12 ) ) {
        return 1;
    }

    //co2_enable( ctx, CO2_DEVICE_EN );
    return i2c_master_write_then_read( I2C_ADDR_CO_2, reg, data_buf, 1 );
    //co2_enable( ctx, CO2_DEVICE_DIS );

}


uint8_t co2_wait_i2c_ready ( void ) {

    uint8_t     i2c_ready;

    uint32_t    ReadyTime;
    setTimeOut( &ReadyTime, 3 );                //wait for a reply
    
    do {
        co2_generic_read_byte( CO2_STATUS_REG, &i2c_ready );
        if( TimedOut( &ReadyTime ) ) {
            break;
        }
    } while ( !( i2c_ready & 0x01 ) );

    if( !( i2c_ready & 0x01 ) ) {
        printf("CO 2 Click not ready\r\n");
        return 3;
    }

    return 0;

}


uint8_t co2_read_adc( uint16_t *data_out ) {

    uint8_t     result;
    uint8_t     buff_data[ 2 ];
    uint16_t    tmp;
    uint8_t     read_addr;

    read_addr = 0;
    
    result = i2c_master_read( ADC_DEVICE_ADDR, buff_data, 2 );

    tmp = buff_data[ 0 ];
    tmp <<= 8;
    tmp |= buff_data[ 1 ];
    
    *data_out = tmp;
    return result;

}


uint8_t co2_get_co2_ppm( float* co2_data ) {

    uint8_t     result;
    uint16_t    adc_data;
    float       temp;
    
    result = co2_read_adc( &adc_data );
    
    temp = ( float )adc_data / 4095;
    temp *= 1000;

    *co2_data = temp;
    return result;
}


/* HW Setup*/
void setup_CO_2_HW( void ) {
    pinMode( ENABLE_CO_2, OUTPUT );
    digitalWrite( ENABLE_I2CA, LOW );   //off
}


/* Processors */
StatusCode powerOn_CO_2( ByteArray* pin, ByteArray* pout ) {
    Ready[I2CAIN] = 0;
    digitalWrite( ENABLE_I2CA, HIGH );
    setTimeOut( &ReadyTime, POWER_ON_WAIT_CO_2 );   //wait after enable
    return StatusCode::NEXT;                        //continue on next iteration
}


StatusCode waitOn_CO_2( ByteArray* pin, ByteArray* pout ) {
    if ( TimedOut( &ReadyTime ) ) {
        return StatusCode::OK;                  //continue immediately
    }
    return StatusCode::REPEAT;
}


StatusCode Scan_I2C( ByteArray* pin, ByteArray* pout ) {

  byte error, address;
  int nDevices;

  printf("Scanning...\r\n");

  nDevices = 0;
  for( address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission( address );
    error = Wire.endTransmission();

    if ( !error ) {
      nDevices++;
      printf("I2C device found at address 0x%02X\r\n", address );
    } else if ( 4 == error ) {
      printf("Unknown error at address 0x%02X\r\n", address );
    }
  }
  if ( nDevices == 0 ) printf("No I2C devices found\r\n");
  else                 printf("Done\r\n");

  return StatusCode::OK;                        //continue immediately

}


StatusCode app_init_CO_2( ByteArray* pin, ByteArray* pout ) {
//void application_init ( void ) {

    //Delay_ms ( 500 );   //POWER_ON_WAIT_CO_2

    Wire.beginTransmission( I2C_ADDR_CO_2 );
    co2_generic_write_byte( CO2_LOCK_REG, CO2_WRITE_MODE );
    co2_generic_write_byte( CO2_MODECN_REG, CO2_STANDBY_MODE );
    co2_generic_write_byte( CO2_TIACN_REG,
        CO2_3500_OHM_TIA_RES | CO2_100_OHM_LOAD_RES );
    co2_generic_write_byte( CO2_REFCN_REG,
        CO2_VREF_EXT | CO2_50_PERCENTS_INT_ZERO | CO2_BIAS_POL_NEGATIVE | CO2_0_PERCENTS_BIAS );
    Wire.endTransmission();
    
    //debug---vvv
    printf( "CO 2 Cick is initialized\r\n" );
    //debug---^^^

    setTimeOut( &ReadyTime, SETUP_WAIT_CO_2 );  //wait after enable
    return StatusCode::NEXT;                    //continue on next iteration

}


StatusCode app_task_CO_2( ByteArray* pin, ByteArray* pout ) {
//void application_task ( void ) {

    //float co2_value;
   
    if ( co2_wait_i2c_ready() ) {
        return StatusCode::ERROR;               //sensor TO
    };
    if ( co2_get_co2_ppm( (float*)pout ) ) {
        return StatusCode::ERROR;               //sensor TO
    };
    Ready[I2CAIN] = 1;

    //Delay_ms ( 300 );                         //in power off delay
    return StatusCode::OK;                      //continue immediately

}


StatusCode powerOff_CO_2( ByteArray* pin, ByteArray* pout ) {
    digitalWrite( ENABLE_I2CA, LOW );
    ReadyTime += 2500;                          //wait for the next cycle
    return StatusCode::NEXT;                    //continue on next iteration
}


void setup_CO_2_SW( void ) {
    if ( pPipelines[I2CAIN] ) {
        printf("Making pPipelines[I2CAIN]:\r\n");

        printf("powerOn_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, powerOn_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

        printf("waitOn_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, waitOn_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

#define SCAN_I2C_DEVS 0
#if SCAN_I2C_DEVS

        printf("Scan_I2C");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, Scan_I2C, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

#else

        printf("app_init_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, app_init_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

        printf("waitOn_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, waitOn_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

        printf("app_task_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, app_task_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

        printf("CO_ppm buffer");
        pPipelines[I2CAIN]->setOutputBuffer( 
            pPipelines[I2CAIN]->getPipeCount(),
            (ByteArray*)( pMeasurements->data() + CO_2_OFFS ) );
        if ( (ByteArray*)( pMeasurements->data() + CO_2_OFFS ) !=
            pPipelines[I2CAIN]->getOutputBuffer( pPipelines[I2CAIN]->getPipeCount() ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

#endif

        printf("powerOff_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, powerOff_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

        printf("waitOn_CO_2");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, waitOn_CO_2, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

        printf("pPipelines[I2CAIN] is ready!\r\n");
    }
}
