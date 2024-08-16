#include "YP_iM284A2.h" //global data
#include "CO_2_click.h"
//#include "Pipes.h"
#include "Pipeline.h"
//#include "CircularBuffer.h"
#include "i2c_face.h"

/*
#define ENABLE_CO_2         ENABLE_I2CA
#define I2C_ADDR_CO_2       0x48
#define I2C_ADDR_CO_2_ADC   0x4D

#define MEAS_WAIT_CO_2      300     //ms wait before measurement
*/


static uint32_t ReadyTime = 0;                      //mainly for global pipes timing
//static float    CO_ppm;


/* CO_2_Click specific: check the targetet reg in[ 1, 10, 11, 12 ] */

uint8_t co2_generic_write_byte( uint8_t reg, uint8_t data ) {

    if ( ( reg < 0x01 ) || ( reg > 0x01 && reg < 0x10 ) || ( reg > 0x12 ) ) {
        return 1;                                   //wrong reg
    }

    //co2_enable( ctx, CO2_DEVICE_EN );             //RST should be to the ground
    i2c_master_write_byte( I2C_ADDR_CO_2, reg, data );
    //co2_enable( ctx, CO2_DEVICE_DIS );            //RST should be to the ground

    return 0;
}

uint8_t co2_generic_read_byte( uint8_t reg, uint8_t *data_buf ) {

    if ( ( reg > 0x01 && reg < 0x10 ) || ( reg > 0x12 ) ) {
        return 1;                                   //wrong reg
    }

    //co2_enable( ctx, CO2_DEVICE_EN );
    return i2c_master_write_then_read( I2C_ADDR_CO_2, reg, data_buf, 1 );
    //co2_enable( ctx, CO2_DEVICE_DIS );

}


uint8_t co2_wait_i2c_ready ( void ) {

    uint8_t     i2c_ready;

    uint32_t    ReadyTime;
    setTimeOut( &ReadyTime, 3 );                    //wait for a reply
    
    do {
        co2_generic_read_byte( CO2_STATUS_REG, &i2c_ready );
        if( TimedOut( &ReadyTime ) ) {
            break;
        }
    } while ( !( i2c_ready & 0x01 ) );

    if( !( i2c_ready & 0x01 ) ) {
        printf("CO 2 Click is not ready\r\n");
        Present[I2CAIN] = 0;                        //device is not responding
        return 3;
    }

    Present[I2CAIN] = 1;                            //device is responding
    return 0;

}


uint8_t co2_read_adc( uint16_t *data_out ) {

    uint8_t     result;
    uint8_t     buff_data[ 2 ];
    uint16_t    tmp;
    uint8_t     read_addr;

    read_addr = 0;
    
    result = i2c_master_read( I2C_ADDR_ADC, buff_data, 2 );

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
    digitalWrite( ENABLE_I2CA, LOW );               //off
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
        return StatusCode::OK;                      //continue immediately
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

    error = i2c_master_test( address );

    if ( !error ) {
      nDevices++;
      printf("I2C device found at address 0x%02X\r\n", address );
    } else if ( 4 == error ) {
      printf("Unknown error at address 0x%02X\r\n", address );
    }
  }
  if ( nDevices == 0 ) printf("No I2C devices found\r\n");
  else                 printf("Done\r\n");

  return StatusCode::OK;                            //continue immediately

}


StatusCode check_CO_2( ByteArray* pin, ByteArray* pout ) {

    if ( i2c_master_test( I2C_ADDR_CO_2 ) ) {
        Present[I2CAIN] = 0;                        //device is not responding
        return StatusCode::NEXT;
    }

    if ( i2c_master_test( I2C_ADDR_ADC ) ) {
        Present[I2CAIN] = 0;                        //device is not responding
        return StatusCode::NEXT;
    }
    
    Present[I2CAIN] = 1;                            //device is responding
    return StatusCode::NEXT;

}


StatusCode app_init_CO_2( ByteArray* pin, ByteArray* pout ) {
//void application_init ( void ) {

    //Delay_ms ( 500 );   //POWER_ON_WAIT_CO_2

    if ( Present[I2CAIN] ) {
    
        //Wire.beginTransmission( I2C_ADDR_CO_2 );
        //buutu jaaiet taapat, jo co2_generic_write_byte ietver Wire.beginTransmission( I2C_ADDR_CO_2 )
        co2_generic_write_byte( CO2_LOCK_REG, CO2_WRITE_MODE );
        co2_generic_write_byte( CO2_MODECN_REG, CO2_STANDBY_MODE );
        co2_generic_write_byte( CO2_TIACN_REG,
            CO2_3500_OHM_TIA_RES | CO2_100_OHM_LOAD_RES );
        co2_generic_write_byte( CO2_REFCN_REG,
            CO2_VREF_EXT | CO2_50_PERCENTS_INT_ZERO | CO2_BIAS_POL_NEGATIVE | CO2_0_PERCENTS_BIAS );
        //Wire.endTransmission();
    
        //debug---vvv
        //printf( "CO 2 Cick is initialized\r\n" );
        //debug---^^^

        setTimeOut( &ReadyTime, SETUP_WAIT_CO_2 );  //wait after enable
    
    } else {

        setTimeOut( &ReadyTime, 0 );                //continue immediately

    }

    return StatusCode::NEXT;                        //continue on next iteration

}


StatusCode app_task_CO_2( ByteArray* pin, ByteArray* pout ) {
//void application_task ( void ) {

    //float co2_value;

    if ( Present[I2CAIN] ) {
   
        if ( co2_wait_i2c_ready() ) {
            return StatusCode::ERROR;               //sensor TO
        };
        if ( co2_get_co2_ppm( (float*)pout ) ) {
            return StatusCode::ERROR;               //sensor TO
        };
        Ready[I2CAIN] = 1;

        //Delay_ms ( 300 );                         //in power off delay
    }

    return StatusCode::OK;                          //continue immediately

}


StatusCode powerOff_CO_2( ByteArray* pin, ByteArray* pout ) {
    digitalWrite( ENABLE_I2CA, LOW );
    ReadyTime += 2500;                              //wait for the next cycle
    return StatusCode::NEXT;                        //continue on next iteration
}

#define SCAN_I2C_DEVS 1
#define DYNAMIC_ALLOCATE 0

#if DYNAMIC_ALLOCATE
/*
Sketch uses 58948 bytes (29%) of program storage space. Maximum is 196608 bytes.
Global variables use 6660 bytes (32%) of dynamic memory, leaving 13820 bytes for local variables. Maximum is 20480 bytes.
*/
#else
/*
Sketch uses 59028 bytes (30%) of program storage space. Maximum is 196608 bytes.
Global variables use 6660 bytes (32%) of dynamic memory, leaving 13820 bytes for local variables. Maximum is 20480 bytes.
*/
static const InitRecord InitArray[] = {
    { nullptr,  powerOn_CO_2,   nullptr },
    { nullptr,  waitOn_CO_2,    nullptr },
#if SCAN_I2C_DEVS
    { nullptr,  Scan_I2C,       nullptr },
#else
    { nullptr,  check_CO_2,     nullptr },
    { nullptr,  app_init_CO_2,  nullptr },
    { nullptr,  waitOn_CO_2,    nullptr },
    { nullptr,  app_task_CO_2,  nullptr },
#endif

    { nullptr,  powerOff_CO_2,  nullptr },
    { nullptr,  waitOn_CO_2,    nullptr }
};

#endif

void setup_CO_2_SW( void ) {
    if ( pPipelines[I2CAIN] ) {

        printf("Making pPipelines[I2CAIN]:\r\n");

#if DYNAMIC_ALLOCATE

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

        printf("pPipelines[I2CAIN] is created!\r\n");

#else

        StatusCode status = pPipelines[I2CAIN]->Setup(
            InitArray,
            sizeof( InitArray ) / SIZE_OF_INITRECORD );

        if ( StatusCode::OK == status ) {

            //6. konvejierim app_task_CO_2 vajag buferi, kur nolikt float
            //un tas ir nosûtâmo datu masîvâ uint8_t*, ko pârkâsto par (ByteArray*)
            //pats masîvs inkapsulçts ByteArray objektâ
            printf("CO_ppm buffer");
            pPipelines[I2CAIN]->setOutputBuffer( 6,
                (ByteArray*)( pMeasurements->data() + CO_2_OFFS ) );
            if ( (ByteArray*)( pMeasurements->data() + CO_2_OFFS ) !=
                pPipelines[I2CAIN]->getOutputBuffer( 6 ) ) {
                printf(" <- Error");
            }
            printf("\r\n");

            printf("pPipelines[I2CAIN] is ready!\r\n");

        } else {

            printf("pPipelines[I2CAIN] is not ready!\r\n");

        }

#endif

    } else {

        printf("No pPipelines[I2CAIN] :(\r\n");

    }
}
