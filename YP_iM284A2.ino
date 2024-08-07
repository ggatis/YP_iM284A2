#include <stdint.h>
#include <stdio.h>  //c printf
//#include <iostream>
//#include <cstdio>

#include "printf.inc"   //enable printf
bool noPrintf = true;

#include "YP_iM284A2.h"
#include "CircularBuffer.h"
//#include "Pipeline.h"

//some important data

//time
volatile uint32_t mySysTick = 0;

/* Buffers and Pipelines */
/*
0 - SerialUSB: PC, debug
1 - Serial1:   Lora module
2 - Serial2:   UART2
3 - not used, just for comfort
4 - Serial4:   UART4
5 - Serial5:   UART5
6 - I2CA
7 - I2CB
8 - I2CC
*/

//last input from buffers: only SERIALUSB and SERIAL1IN can operate independently
volatile uint32_t lastIOticks[3] =
    { 0, 0, 0 };

//planned sizes of the buffers
uint16_t CBuffSizes[CPIPELINES] =
    { 300, 300, 100, 000,
      100, 100, 100, 100,
      100, 000, 000, 300,
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
}

void setupSerials( void ) {
  //Serial.end();
  SerialUSB.begin( 115200 );
  //Initialize serial and wait for port to open:
  uint32_t SerialUSBtimeout;
  setTimeOut( &SerialUSBtimeout, 7000 );
  noPrintf = false;
  while ( !SerialUSB ) {
     if ( TimedOut( &SerialUSBtimeout ) ) {
        noPrintf = true;
        break;
    }   //wait for serial port to connect. Needed for native USB port only
  }
  LEDoff();
  //printf( "1RX PA10 DigitalPin/PinName: %d/%d\r\n", PA10, digitalPinToPinName( PA10 ) );
  //printf( "1TX PA9  DigitalPin/PinName: %d/%d\r\n", PA9, digitalPinToPinName( PA9 ) );
  //printf( "2RX PA_3_ALT1 DigitalPin/PinName: %d/%d\r\n", PA_3_ALT1, digitalPinToPinName( PA_3_ALT1 ) );
  //printf( "2TX PA_2_ALT1 DigitalPin/PinName: %d/%d\r\n", PA_2_ALT1, digitalPinToPinName( PA_2_ALT1 ) );

  Serial1.end();
  Serial2.end();
  Serial4.end();
  Serial5.end();

  Serial1.setRx( PA10 );
  Serial1.setTx( PA9 );
  Serial1.begin( 115200 );
  //while ( !Serial1 ) {
  //  printf("1");
  //}
  //printf("Serial1 ready!\r\n");

  Serial2.setRx( PA_3_ALT1 );
  Serial2.setTx( PA_2_ALT1 );
  Serial2.begin( 115200 );
  //while ( !Serial2 ) {
  //  printf("2");
  //}
  //printf("Serial2 ready!\r\n");

  Serial4.setRx( PA1 );
  Serial4.setTx( PA0 );
  Serial4.begin( 115200 );
  //while ( !Serial4 ) {
  //  printf("4");
  //}
  //printf("Serial4 ready!\r\n");

  Serial5.setRx( PB4 );
  Serial5.setTx( PB3 );
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

#include "Pipes.h"

void setupObjects() {
/*
uint16_t CBuffSizes[CPIPELINES] =
    { 300, 300, 100, 000,
      100, 100, 100, 100,
      100, 000, 000, 300,
      100, 000, 100, 100 };
*/
    //circular buffers
    for ( uint8_t i = 0; i < CPIPELINES; i++ ) {
        if ( CBuffSizes[i] ) {
            pCBuffs[i] = new CircularBuffer( CBuffSizes[i] );
            if ( nullptr == pCBuffs[i] ) {
                printf("No space for CircularBuffer %d!\r\n", i );
            }
        }
    }
    
    //Initialize Pipeline objects specifying the default buffer size
    for ( uint8_t i = 0; i < CPIPELINES; i++ ) {
        if ( CBuffSizes[i] ) {
            pPipelines[i] = new Pipeline( pCBuffs[i] );
            if ( nullptr == pPipelines[i] ) {
                printf("No space for Pipeline %d!\r\n", i );
            }
        }
    }

    //init GoL
    //GoLinit( pPipeline->getFrontEnd(), pPipeline->getBackEnd() );

    //pPipeline->getFrontEnd()->print2D( width, height );

    pDemoApp = new LoRa_Mesh_DemoApp;
    if ( nullptr == pDemoApp ) {
        printf("No space for DemoApp\r\n");
    }
    setup_DemoApp();
    
    //Add HMI incoming data processing pipeline
    if ( pPipelines[0] ) {
        if ( StatusCode::OK == pPipelines[0]->AddProcessor( OnHMI_DataEvent ) ) {
            ;
        } else {
            printf("No space for pPipelines[0] OnHMI_DataEvent\r\n");
        }
    }

    //Add radio modem incoming data processing pipeline
    if ( pPipelines[1] ) {
        if ( StatusCode::OK == pPipelines[1]->AddProcessor( OnRadioHub_DataEvent, 0 ) ) {
            ;
        } else {
            printf("No space for pPipelines[1] OnRadioHub_DataEvent\r\n");
        }
    }
    
    //Add a parser and a processor to the Pipeline list
    if ( pPipelines[2] ) {
        if ( StatusCode::OK == pPipelines[2]->AddProcessor( parser, 20 ) ) {
            if ( StatusCode::OK == pPipelines[2]->AddProcessor( process, 1 ) ) {
                pPipelines[2]->setErrorHandler( myErrorHandler );
            } else {
                printf("No space for pPipelines[2] process\r\n");
            }
        } else {
            printf("No space for pPipelines[2] parser\r\n");
        }
    }


}


void setup( void ) {

  setupPins();
  setupPrintf();
  setupSerials();

  printf("\r\nYP_iM284A2.ino\r\n");
  printf("This application demonstrates the host controller message protocol\r\n");
  printf("for WiMOD radio modules provided by IMST.\r\n");

  Serial1.print("Serial1\r\n");
  Serial2.print("Serial2\r\n");
  Serial4.print("Serial4\r\n");
  Serial5.print("Serial5\r\n");

  //setupTimers();
  //setupObjects();

  mySysTick = HAL_GetTick();

}

/*********************************************************************/
/*                               Main                                */
/*********************************************************************/
void loop() {

    mySysTick = HAL_GetTick();
    if ( 0 == ( mySysTick % 1000 ) ) {
        printf("%d\r\n", mySysTick / 1000 );
    }
    //while ( 0 < SerialUSB.available() ) {
    //    LEDtoggle();
    //    printf("%c", SerialUSB.read() );
    //    //buff = SerialUSB.read();
    //    //if ( pCBuffs[0] ) {
    //    //    pCBuffs[0]->put( buff );
    //    //}
    //}

    //pPipelines[0]->processAll();
    //pPipelines[1]->processAll();
    //pPipelines[2]->processAll();

}
