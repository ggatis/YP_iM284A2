#include "YP_iM284A2.h" //global data
#include "TX1_Radio.h"
#include "Pipeline.h"


StatusCode TX1_Write( ByteArray* pIn, ByteArray* pOut ) {
    //nullptr == pOut; endpoint

    //debug-vvv
    //printf("TX1_Write\r\n");
    //debug-^^^
    
    //we know what we have on inputs
    CircularBuffer* pCBin = (CircularBuffer*)pIn;

    if ( 0 == pCBin->count() ) return StatusCode::OK;

    //debug-vvv
    printf("TX1_Write: \"");
    pCBin->print();
    printf("\":\"");
    //debug-^^^

    while ( pCBin->count() ) {
        uint8_t chOut = pCBin->get();
        printf("%c", chOut );
        Serial1.write( chOut );
    }
    printf("\"\r\n");
    //debug-^^^

    return StatusCode::OK;

}


static const InitRecord InitArray[] = {
    { nullptr,  TX1_Write,  nullptr }
};

void setup_TX1_Radio( void ) {
    if ( pPipelines[SERIAL1OUT] ) {

        printf("Making pPipelines[SERIAL1OUT]:\r\n");

        StatusCode status = pPipelines[SERIAL1OUT]->Setup(
            InitArray,
            sizeof( InitArray ) / SIZE_OF_INITRECORD );

        //input CB
        if ( StatusCode::OK == status ) {

            printf("TX1 CBuffer");
            pPipelines[SERIAL1OUT]->setInputBuffer( 1, pCBuffs[SERIAL1OUT] );
            if ( pPipelines[SERIAL1OUT]->getInputBuffer( 1 ) != pCBuffs[SERIAL1OUT] ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        printf("pPipelines[SERIAL1OUT] is ");
        if ( StatusCode::OK != status ) {
            printf("not");
        }
        printf(" ready!\r\n");

    } else {

        printf("No pPipelines[SERIAL1OUT] :(\r\n");

    }
}
