#include "YP_iM284A2.h" //global data
#include "TX1_Radio.h"
#include "SlipEncoder.h"
#include "ServiceAccessPoint.h"


StatusCode PipeSwitch( ByteArray* pIn, ByteArray* pOut ) {
    //CB4, SM
    CircularBuffer* pCBin = (CircularBuffer*)pIn;
    if ( 4 > pCBin->count() )
        return StatusCode::OK;

    //declare pointer
    StatusCode (*pFunction)(ByteArray*, ByteArray*);

    //cpp magic
    pFunction = reinterpret_cast<StatusCode (*)(ByteArray*, ByteArray*)>( pCBin->getU32() );

    //a call
    StatusCode status = pFunction( pIn, pOut );

    if ( StatusCode::OK == status )
        return StatusCode::NEXT;

    return status;

}


/**
 * @brief   send a messsage
 *
 * @param   serialMsg   outgoing message
 *
 * @return  true/false
 */
StatusCode SendMessage( ByteArray* pSMin, ByteArray* pCBout ) {

    //ByteArray  outputData;
    SerialMessage*  pSM = (SerialMessage*)pSMin;
    CircularBuffer* pCB = (CircularBuffer*)pCBout;

    uint8_t sapID = pSM->GetSapID();

    //find SAP
    for ( uint8_t SAPi = 0; SAPi < 3; SAPi++ ) {
        ServiceAccessPoint* pServiceAccessPoint = pServiceAccessPoints[SAPi];
        //SAP found ?
        if ( pServiceAccessPoint && ( pServiceAccessPoint->GetSAPid() == sapID ) ) {
            //yes, add _NumWakeupChars to output

            //simple solution for systems with enough RAM?
            //prepend wakeup chars
            for ( uint8_t i = 0; i < pServiceAccessPoint->GetWUpChars(); i++ ) {
                pCB->append( SLIP_Begin );
            }

        }
    }

    //ietverts Pipe, izsauc ar ByteArray atvasinaatiem objektiem,
    // izlabot!!! SLIP_Encode
    return SLIP_Encode( pSMin, pCBout );

}


StatusCode TX1_Write( ByteArray* pIn, ByteArray* pOut ) {
    //nullptr == pOut; endpoint

    //debug-vvv
    //printf("TX1_Write\r\n");
    //debug-^^^
    
    //we know what we have on inputs
    CircularBuffer* pCBin = (CircularBuffer*)pIn;

    if ( 0 == pCBin->count() ) return StatusCode::OK;

    while ( pCBin->count() && Serial1.availableForWrite() ) {
        uint8_t chOut = pCBin->get();
        //printf("%c", chOut );
        Serial1.write( chOut );
    }

    if ( pCBin->count() )
        return StatusCode::REPEAT;

    return StatusCode::OK;

}


static const InitRecord InitArray[] = {
    { nullptr,  PipeSwitch,     nullptr },  //CB4, SM
    { nullptr,  SendMessage,    nullptr },  //SM, CB
    { nullptr,  TX1_Write,      nullptr }   //CB
};

void setup_TX1_Radio( void ) {
    if ( pPipelines[SERIAL1OUT] ) {

        printf("Making pPipelines[SERIAL1OUT]:\r\n");

        StatusCode status = pPipelines[SERIAL1OUT]->Setup(
            InitArray,
            sizeof( InitArray ) / SIZE_OF_INITRECORD );

        //input CB
        if ( StatusCode::OK == status ) {

            printf("TX1 CBuffer4");
            CircularBuffer* pCB = new CircularBuffer( 4 );
            pPipelines[SERIAL1OUT]->setInputBuffer( 1, pCB );
            if ( pPipelines[SERIAL1OUT]->getInputBuffer( 1 ) != pCB ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        //SM
        if ( StatusCode::OK == status ) {

            printf("TX1 SMessage");
            SerialMessage* pSM = new SerialMessage( CBuffSizes[SERIAL1OUT] );
            pPipelines[SERIAL1OUT]->setOutputBuffer( 1, pSM );
            if ( pPipelines[SERIAL1OUT]->getOutputBuffer( 1 ) != pSM ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            pPipelines[SERIAL1OUT]->setInputBuffer( 2, pSM );
            if ( pPipelines[SERIAL1OUT]->getInputBuffer( 2 ) != pSM ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        //output CB
        if ( StatusCode::OK == status ) {

            printf("TX1 CBuffer");
            pPipelines[SERIAL1OUT]->setOutputBuffer( 2, pCBuffs[SERIAL1OUT] );
            if ( pPipelines[SERIAL1OUT]->getOutputBuffer( 2 ) != pCBuffs[SERIAL1OUT] ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            pPipelines[SERIAL1OUT]->setInputBuffer( 3, pCBuffs[SERIAL1OUT] );
            if ( pPipelines[SERIAL1OUT]->getInputBuffer( 3 ) != pCBuffs[SERIAL1OUT] ) {
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
