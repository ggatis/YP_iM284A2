#include <stdint.h>
#include <stdio.h>  //c printf
//#include <iostream>
//#include <cstdio>

#include "printf.inc"   //enable printf
bool noPrintf = true;

#include <Wire.h>

#include "YP_iM284A2.h"
//#include "Pipeline.h"
#include "CurrentTime.h"

#include "RX1_Radio.h"
#include "RX2_Monitor.h"
//...
#include "CO_2_click.h"
//...
#include "TX1_Radio.h"

//ServiceAccessPoint
#include "ServiceAccessPoint.h"
#include "DeviceManagement.h"

/* Some important data */

//time
volatile uint32_t mySysTick     = 0;
volatile uint32_t mySysTickOld  = 0;

/* Buffers and Pipelines */
/*
 0 - SerialUSB: PC, debug
 1 - Serial1:   from Lora module
 2 - Serial2:   UART2
 3 - not used,  just for comfort
 4 - Serial4:   UART4
 5 - Serial5:   UART5
 6 - I2CA:      CO 2 Click
 7 - I2CB
 8 - I2CC
 9 - not used,  just for comfort
10 - SerialUSB: special output
11 - Serial1:   to Lora module
12 - Serial2:   to UART2
13 - not used,  just for comfort
14 - Serial4:   to UART4
15 - Serial5:   to UART5
*/

//uint8_t         Present[9]  = { 0, 0, 1, 0, 1, 1, 1, 1, 1 }; //max devices present
uint8_t         Present[9]  = { 0, 0, 0, 0, 0, 0, 1, 0, 0 }; //devices present
uint8_t         Ready[9]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //reading ready
ByteArray*      pMeasurements;

//last input from buffers: only SERIALUSB and SERIAL1IN can operate independently
volatile uint32_t lastIOticks[3] =
    { 0, 0, 0 };

//planned sizes of the buffers
uint16_t CBuffSizes[CPIPELINES] =
    { 300, 300, 100, 000,
      100, 100,  16,  16,
       16, 000, 300, 300,
      100, 000, 100, 100 };

//input buffers
CircularBuffer* pCBuffs[CPIPELINES] =
    { nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr };

//dynamically allocated objects
Pipeline* pPipelines[CPIPELINES] =
    { nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr };

//ServiceAccessPoints
ServiceAccessPoint* pServiceAccessPoints[3] =
    { nullptr, nullptr, nullptr };

#include "LoRa_Mesh_DemoApp.h"
LoRa_Mesh_DemoApp* pDemoApp;

//#include "CRC16.h"
//#include "SlipDecoder.h"
//#include "SlipEncoder.h"


/*********************************************************************/
/*                               Pins                                */
/*********************************************************************/
#include "LED.h"


/*********************************************************************/
/*                              Timeout                              */
/*********************************************************************/
void setTimeOut( uint32_t* pTimeOutTime, uint32_t timeout ) {
    *pTimeOutTime = HAL_GetTick() + timeout;
}

bool TimedOut( uint32_t* pTimeOutTime ) {
    return *pTimeOutTime < HAL_GetTick();
}


/*********************************************************************/
/*                              printf                               */
/*********************************************************************/
void setupPrintf( void ) {
    registerNoPrintf( &noPrintf );
}


/*********************************************************************/
/*                              Serials                              */
/*********************************************************************/
//data from PC, called by default when
//defined(USBCON) && defined(USBD_USE_CDC)
//and ndef DISABLE_GENERIC_SERIALUSB
void serialEventUSB( void ) {
    lastIOticks[0] = HAL_GetTick();
    while ( 0 < SerialUSB.available() ) {
        LEDtoggle();
        if ( pCBuffs[0] ) {
            pCBuffs[0]->put( SerialUSB.read() );
        } else {
            SerialUSB.write( SerialUSB.read() );
        }
    }
}

//data from modem
void serialEvent1() {
    lastIOticks[1] = HAL_GetTick();
    while ( 0 < Serial1.available() ) {
        LEDtoggle();
        if ( pCBuffs[1] ) {
            pCBuffs[1]->put( Serial1.read() );
        } else {
            Serial1.read();
        }
    }
}

//data 2 or to modem
void serialEvent2() {
    lastIOticks[2] = HAL_GetTick();
    while ( 0 < Serial2.available() ) {
        LEDtoggle();
        if ( pCBuffs[2] ) {
            pCBuffs[2]->put( (uint8_t)Serial2.read() );
        } else {
            Serial2.read();
        }
    }
}

//data 4
void serialEvent4() {
    while ( 0 < Serial4.available() ) {
        LEDtoggle();
        if ( pCBuffs[4] ) {
            pCBuffs[4]->put( (uint8_t)Serial4.read() );
        } else {
            Serial4.read();
        }
    }
}

//data 5
void serialEvent5() {
    while ( 0 < Serial5.available() ) {
        LEDtoggle();
        if ( pCBuffs[5] ) {
            pCBuffs[5]->put( (uint8_t)Serial5.read() );
        } else {
            Serial5.read();
        }
    }
}


/*********************************************************************/
/*                               Setup                               */
/*********************************************************************/
void setupPins( void ) {
    LEDinit();
    //SERIAL2IN
    //SERIALUSB
    //SERIAL1IN
    //SERIAL2IN
    //SERIAL4IN
    //SERIAL5IN
    //I2CAIN
    setup_CO_2_HW();
    //I2CBIN
    //I2CCIN
}


void setupWire( void ) {
    Wire.end();
    Wire.setSDA( WIRE_SDA );
    Wire.setSCL( WIRE_SCL );
    Wire.begin();
    Wire.setClock( 100000 );
}


void setupSerials( void ) {
  //Serial.end();
  SerialUSB.begin( 115200 );
  //Initialize serial and wait for port to open:
  uint32_t SerialUSBtimeout;
  setTimeOut( &SerialUSBtimeout, 8000 );
  noPrintf = false;
  while ( !SerialUSB ) {
     if ( TimedOut( &SerialUSBtimeout ) ) {
        noPrintf = true;
        SerialUSB.end();
        break;
    }   //wait for serial port to connect. Needed for native USB port only
  }
  //flush SerialUSB
  while ( 0 < SerialUSB.available() ) SerialUSB.read();
  LEDoff();
  //printf( "1RX PA10 DigitalPin/PinName: %d/%d\r\n", PA10, digitalPinToPinName( PA10 ) );
  //printf( "1TX PA9  DigitalPin/PinName: %d/%d\r\n", PA9, digitalPinToPinName( PA9 ) );
  //printf( "2RX PA_3_ALT1 DigitalPin/PinName: %d/%d\r\n", PA_3_ALT1, digitalPinToPinName( PA_3_ALT1 ) );
  //printf( "2TX PA_2_ALT1 DigitalPin/PinName: %d/%d\r\n", PA_2_ALT1, digitalPinToPinName( PA_2_ALT1 ) );

  Serial1.end();
  Serial2.end();
  Serial4.end();
  Serial5.end();

  Serial1.setRx( UART1_RX );
  Serial1.setTx( UART1_TX );
  Serial1.begin( 115200 );
  //while ( !Serial1 ) {
  //  printf("1");
  //}
  //printf("Serial1 ready!\r\n");

  Serial2.setRx( UART2_RX );
  Serial2.setTx( UART2_TX );
  Serial2.begin( 115200 );
  //while ( !Serial2 ) {
  //  printf("2");
  //}
  //printf("Serial2 ready!\r\n");

  Serial4.setRx( UART4_RX );
  Serial4.setTx( UART4_TX );
  Serial4.begin( 115200 );
  //while ( !Serial4 ) {
  //  printf("4");
  //}
  //printf("Serial4 ready!\r\n");

  Serial5.setRx( UART5_RX );
  Serial5.setTx( UART5_TX );
  Serial5.begin( 115200 );
  //while ( !Serial5 ) {
  //  printf("5");
  //}
  //printf("Serial5 ready!\r\n");

  //printf( "1RX PA10 PinName: %d\r\n", Serial1.getRx() );
  //printf( "1TX PA9  PinName: %d\r\n", PA9, Serial1.getTx() );
  //printf( "2RX PA_3_ALT1 PinName: %d\r\n", PA_3_ALT1, digitalPinToPinName( PA_3_ALT1 ) );
  //printf( "2TX PA_2_ALT1 PinName: %d\r\n", PA_2_ALT1, digitalPinToPinName( PA_2_ALT1 ) );

}

void setupTimers( void ) {

#if 0 == _SYSTIME_
  //initCurrentTime(); temp!!!
#endif
#if defined( TIM1 )
  TIM_TypeDef *Instance = TIM1;
#else
  TIM_TypeDef *Instance = TIM2;
#endif

  //Instantiate HardwareTimer object.
  //Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
/* temp!!!
  pMyTim = new HardwareTimer( Instance );
  pMyTim->setMode( 2, TIMER_OUTPUT_COMPARE );
  pMyTim->setOverflow( 10, HERTZ_FORMAT );  //10 Hz
  pMyTim->attachInterrupt( _100msCallback );
  pMyTim->resume();
*/
}


void setupObjects( void ) {

    pMeasurements = new ByteArray( DATA_SIZE );     //all results
    if ( nullptr == pMeasurements )
        printf("No space for pMeasurements!\r\n");

    /* ServiceAccessPoints */
    pServiceAccessPoints[0] = new DeviceManagement();
    //pServiceAccessPoints[1] = new LoRaMeshRouter();
    //pServiceAccessPoints[2] = new Trace();
    if ( nullptr == pServiceAccessPoints[0] )
        printf("No space for DeviceManagement!\r\n");
    //if ( nullptr == pServiceAccessPoints[1] )
    //    printf("No space for LoRaMeshRouter!\r\n");
    //if ( nullptr == pServiceAccessPoints[2] )
    //    printf("No space for Trace!\r\n");


    /* Circular buffers */
/*
uint16_t CBuffSizes[CPIPELINES] =
    { 300, 300, 100, 000,
      100, 100, 100, 100,
      100, 000, 000, 300,
      100, 000, 100, 100 };
*/
    for ( uint8_t i = 0; i < CPIPELINES; i++ ) {
        if ( CBuffSizes[i] ) {
            pCBuffs[i] = new CircularBuffer( CBuffSizes[i] );
            if ( nullptr == pCBuffs[i] ) {
                printf("No space for pCBuffs[%d]!\r\n", i );
#if 0
            } else {
                printf("pCBuffs[%d] with size %d created!\r\n", i, pCBuffs[i]->size() );
#endif
            }
        }
    }
    
    /* Initialize Pipeline objects specifying the default buffer size */
#if 1
    for ( uint8_t i = 0; i < CPIPELINES; i++ ) {
        if ( CBuffSizes[i] ) {
            pPipelines[i] = new Pipeline( pCBuffs[i] );
            if ( nullptr == pPipelines[i] ) {
                printf("No space for Pipeline %d!\r\n", i );
#if 0
            } else {
                printf("Pipeline#%d created! Buffer count/size: %d/%d\r\n",
                    i, pPipelines[i]->getBufferCount(), pPipelines[i]->getBackEnd()->size() );
#endif
            }
        }
    }
#endif

    /* init Pipes */

    //SERIALUSB: HMI incoming data processing pipeline
    setup_RXU_HMI();

    //SERIAL1IN: radio modem incoming data processing pipeline
    setup_RX1_Radio();

    //SERIAL2IN: a Serial1 monitor (at the start)
    setup_RX2_Monitor();

    //SERIAL4IN
    //SERIAL5IN

    //I2CAIN: actually no CB needed, but still the pipeline
    setup_CO_2_SW();

    //I2CBIN
    //I2CCIN

    //SERIAL1OUT
    setup_TX1_Radio();

    //SERIAL2OUT
    //SERIAL4OUT
    //SERIAL5OUT

    /* init main app */
#if 1
    pDemoApp = new LoRa_Mesh_DemoApp;
    if ( nullptr == pDemoApp ) {
        printf("No space for DemoApp! Stopping!\r\n");
        while ( 1 ) {;}
    } else {
        //debug---vvv
        printf("DemoApp created!\r\n");
        //debug---^^^
        setup_DemoApp();
        //debug---vvv
        printf("DemoApp initialised!\r\n");
        //debug---^^^
    }
#endif

}


void setup( void ) {

  setupPins();
  setupWire();
  setupPrintf();
  setupSerials();

  printf("\r\nYP_iM284A2.ino\r\n");
  printf("This application demonstrates the host controller message protocol\r\n");
  printf("for WiMOD radio modules provided by IMST.\r\n");

  Serial1.print("Serial1\r\n");
  Serial2.print("Serial2\r\n");
  Serial4.print("Serial4\r\n");
  Serial5.print("Serial5\r\n");

#if 0
  //setupTimers();
#endif

  setupObjects();

  //setup Time
  mySysTick     = HAL_GetTick();
  initCurrentTime();

}

/*********************************************************************/
/*                               Main                                */
/*********************************************************************/
void loop() {

    mySysTickOld    = mySysTick;
    mySysTick       = HAL_GetTick();
    addCurrentTimeInUSeconds( 1000 * ( mySysTickOld - mySysTick ) );

#if 1
    /*
    if ( 0 == ( mySysTick % 1000 ) ) {
        printf("%d:", mySysTick / 1000 );
        printf("%d/%d/%d\r\n",
            pCBuffs[0]->count(), pCBuffs[1]->count(), pCBuffs[2]->count() );
    }
    */
#endif

#if 1
    pPipelines[SERIALUSB]->processAll();
#endif

#if 0
    //pPipelines[SERIAL1IN]->processAll();
#else
    if ( pCBuffs[SERIAL1IN]->count() ) {
        printf("1%c", pCBuffs[SERIAL1IN]->get() );
    }
#endif

    pPipelines[SERIAL2IN]->processAll();

#if 0
    //pPipelines[SERIAL4IN]->processAll();
#else
    if ( pCBuffs[SERIAL4IN]->count() ) {
        printf("4%c", pCBuffs[SERIAL4IN]->get() );
    }
#endif

#if 0
    //pPipelines[SERIAL5IN]->processAll();
#else
    if ( pCBuffs[SERIAL5IN]->count() ) {
        printf("5%c", pCBuffs[SERIAL5IN]->get() );
    }
#endif

    //I2CAIN
    pPipelines[I2CAIN]->processAll();

    //I2CBIN
    //I2CCIN

    //SERIAL1OUT
    pPipelines[SERIAL1OUT]->processAll();

    //kaa konvejieri
    if ( Ready[I2CAIN] && pMeasurements ) {
        Ready[I2CAIN] = 0;
        uint32_t ppb = (uint32_t)( 1000 * *(float*)( pMeasurements->data() + CO_2_OFFS ) );
        printf("CO: 0.%03d ppm\r\n", ppb );
    };

}
