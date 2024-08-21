#include "YP_iM284A2.h"                             //global data
#include "RX1_Radio.h"
#include "SlipDecoder.h"
#include "ServiceAccessPoint.h"


/**
 * @brief   check crc and forward message to corresponding service access point
 *
 * @param   serialMsg   incoming message
 *
 * @param   result      decoded data in Json fromat
 *
 * @return  true/false
 */
StatusCode SAP_OnDispatchMessage( ByteArray* pSMin, ByteArray* pDictOut ) {
    //*SerialMessage, *Dictionary
    SerialMessage*  pSM = (SerialMessage*)pSMin;
    Dictionary*     pDi = (Dictionary*)pDictOut;

    // check CRC first
    if ( false == pSM->CheckCRC16() )
        return StatusCode::ERROR;

    pSM->RemoveCRC16();

    uint8_t sapID = pSM->GetSapID();

    // find SAP
    for ( uint8_t SAPi = 0; SAPi < 3; SAPi++ ) {
        ServiceAccessPoint* pServiceAccessPoint = pServiceAccessPoints[SAPi];
        // SAP found ?
        if ( pServiceAccessPoint && ( pServiceAccessPoint->GetSAPid() == sapID ) ) {
            // yes, call message decoder
            return pServiceAccessPoint->OnDecodeMessage( pSM, pDi );
        }

    }
    //
    printf("No SAP for SAPid = %d.", sapID );
    //

    return StatusCode::ERROR;

}


/**
 * @brief   print results for incoming radio events and response
 *
 * @param   result      decoded data from radio module in Json format
 *          out         if something has to be processed further
 */
//void
//LoRa_Mesh_DemoApp::OnRadioHub_DataEvent( const QJsonObject& result ) {
//void
//LoRa_Mesh_DemoApp::OnRadioHub_DataEvent( const ByteArray& result ) {
//void
//LoRa_Mesh_DemoApp::OnRadioHub_DataEvent( const Dictionary& result ) {
StatusCode OnRadioHub_DataEvent( ByteArray* result, ByteArray* out ) {
    //pirms nodot OnRadioHub_DataEvent, jaabuut paarkonverteetam Dictionary
    //gan ByteArray, gan Dictionary print jaastraadaa

    //debug-vvv
    printf("OnRadioHub_DataEvent\r\n");
    //debug-^^^
    
    if ( 0 == result->count() ) return StatusCode::PENDING;

    const char* key0 = "Event";
    const char* key1 = "Status";
    const char* keys[] = { key0, key1 };

    Dictionary* pDataDict = (Dictionary*)result;

#define RadioHubDataEventPipeKind 0

#if 2 == RadioHubDataEventPipeKind
    //A

    for ( uint8_t i = 0; i < ( sizeof( keys ) / sizeof( keys[0] ) ); i++ ) {

        const char*     key = keys[i];
        const uint8_t*  keydata = pDataDict->contains( (const uint8_t*)key );

        if ( keydata ) {
            printf( key );
            printf(" : ");
            printf( (const char*)keydata );
            pDataDict->remove( (const uint8_t*)key );
        }
    }
    pDataDict->print();
    printf("\r\n");


#elif 1 == RadioHubDataEventPipeKind
    //B
    pDataDict->print( keys, ( sizeof( keys ) / sizeof( keys[0] ) ) );
    pDataDict->print( keys, ( sizeof( keys ) / sizeof( keys[0] ) ), true );

#else

    result->print();    //check if if should be casted to CB* first!!

#endif

    return StatusCode::OK;

}


static const InitRecord InitArray[] = {
    { nullptr,  SLIP_Decode,            nullptr },  //CB, SM
    { nullptr,  SAP_OnDispatchMessage,  nullptr },  //SM, Dict
    { nullptr,  OnRadioHub_DataEvent,   nullptr }   //Dict
};


void setup_RX1_Radio( void ) {
    if ( pPipelines[SERIAL1IN] ) {

        printf("Making pPipelines[SERIAL1IN]:\r\n");

        StatusCode status = pPipelines[SERIAL1IN]->Setup(
            InitArray,
            sizeof( InitArray ) / SIZE_OF_INITRECORD );

        //input CB
        if ( StatusCode::OK == status ) {

            printf("RX1 CBuffer");
            pPipelines[SERIAL1IN]->setInputBuffer( 1, pCBuffs[SERIAL1IN] );
            if ( pPipelines[SERIAL1IN]->getInputBuffer( 1 ) != pCBuffs[SERIAL1IN] ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        //output SM
        if ( StatusCode::OK == status ) {

            printf("RX1 SerialMessage");
            SerialMessage* pSM = new SerialMessage( CBuffSizes[SERIAL1IN] );
            if ( nullptr == pSM ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            if ( StatusCode::OK == status ) {
                pPipelines[SERIAL1IN]->setOutputBuffer( 1, pSM );
                if ( pPipelines[SERIAL1IN]->getOutputBuffer( 1 ) != pSM ) {
                    printf(" <- Error");
                    status = StatusCode::ERROR;
                }
            }
            if ( StatusCode::OK == status ) {
                pPipelines[SERIAL1IN]->setInputBuffer( 2, pSM );
                if ( pPipelines[SERIAL1IN]->getInputBuffer( 2 ) != pSM ) {
                    printf(" <- Error");
                    status = StatusCode::ERROR;
                }
            }
            printf("\r\n");

        }

        //output Dict
        if ( StatusCode::OK == status ) {

            printf("RX1 Dictionary");
            Dictionary* pDi = new Dictionary( CBuffSizes[SERIAL1IN] + 150 );
            if ( nullptr == pDi ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            if ( StatusCode::OK == status ) {
                pPipelines[SERIAL1IN]->setOutputBuffer( 2, (ByteArray*)pDi );
                if ( pPipelines[SERIAL1IN]->getOutputBuffer( 2 ) != (ByteArray*)pDi ) {
                    printf(" <- Error");
                    status = StatusCode::ERROR;
                }
            }
            printf("\r\n");

        }

        printf("pPipelines[SERIAL1IN] is ");
        if ( StatusCode::OK != status ) {
            printf("not");
        }
        printf(" ready!\r\n");

    } else {

        printf("No pPipelines[SERIAL1IN] :(\r\n");

    }
}
