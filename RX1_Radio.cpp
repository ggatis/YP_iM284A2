#include "YP_iM284A2.h" //global data
#include "RX1_Radio.h"
#include "Pipeline.h"

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
//    { nullptr,  powerOn_CO_2,   nullptr },
//    { nullptr,  waitOn_CO_2,    nullptr },
    { nullptr,  OnRadioHub_DataEvent,  nullptr }
//    { nullptr,  powerOff_CO_2,  nullptr },
//    { nullptr,  waitOn_CO_2,    nullptr }
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
            pPipelines[SERIAL1IN]->setInputBuffer( 1, pCBuffs[SERIAL1] );
            if ( pPipelines[SERIAL1IN]->getInputBuffer( 1 ) != pCBuffs[SERIAL1IN] ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        //output BA
        if ( StatusCode::OK == status ) {

            printf("RX1 BArray");
            ByteArray* pBA = new ByteArray( CBuffSizes[SERIAL1IN] );
            if ( nullptr == pBA ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            if ( StatusCode::OK == status ) {
                pPipelines[SERIAL1IN]->setOutputBuffer( 1, pBA );
                if ( pPipelines[SERIAL1IN]->getOutputBuffer( 1 ) != pBA ) {
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
