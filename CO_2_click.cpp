#include "YP_iM284A2.h" //global data
#include "CO_2_click.h"
#include "Pipes.h"
#include "Pipeline.h"
//#include "CircularBuffer.h"

/*
#define ENABLE_CO_2     ENABLE_I2CA
#define I2C_ADDR_CO_2   0x48

#define MEAS_WAIT_CO_2  300     //ms wait before measurement
*/

void setup_CO_2_HW( void ) {
    pinMode( ENABLE_CO_2, OUTPUT );
}

static uint32_t ReadyTime = 0;


StatusCode powerOn_CO_2( ByteArray* pin, ByteArray* pout ) {
    digitalWrite( ENABLE_I2CA, HIGH );
    setTimeOut( &ReadyTime, ENA_WAIT_CO_2 );    //wait after enable
    return StatusCode::NEXT;                    //continue on next iteration
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

  return StatusCode::OK;                    //continue immediately

}

StatusCode powerOff_CO_2( ByteArray* pin, ByteArray* pout ) {
    digitalWrite( ENABLE_I2CA, LOW );
    ReadyTime += 2500;                      //wait for the next cycle
    return StatusCode::NEXT;                //continue on next iteration
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

        printf("Scan_I2C");
        if ( StatusCode::OK !=
            pPipelines[I2CAIN]->AddProcessor( nullptr, Scan_I2C, nullptr ) ) {
            printf(" <- Error");
        }
        printf(",\r\n");

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
