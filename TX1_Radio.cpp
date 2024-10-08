#include "YP_iM284A2.h" //global data
#include "TX1_Radio.h"
#include "SlipEncoder.h"
#include "ServiceAccessPoint.h"


/* neizmanto */
/*
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
*/


/**
 * @brief   send a messsage
 *
 * @param   serialMsg   outgoing message
 *
 * @return  true/false
 */
StatusCode SendMessage( ByteArray* pSMin, ByteArray* pCBout ) {

    //input
    SerialMessage*  pSM = (SerialMessage*)pSMin;

    if ( !pSM->count() ) {
        return StatusCode::PENDING;
    }

    //output
    CircularBuffer* pCB = (CircularBuffer*)pCBout;

    uint8_t sapID = pSM->GetSapID();

    //find SAP and append WUpChars before SLIP
    bool NotFound = true;
    for ( uint8_t SAPi = 0; SAPi < 3; SAPi++ ) {
        ServiceAccessPoint* pServiceAccessPoint = pServiceAccessPoints[SAPi];
        //SAP found ?
        if ( pServiceAccessPoint && ( pServiceAccessPoint->GetSAPid() == sapID ) ) {
            //yes, add _NumWakeupChars to output
            NotFound = false;

            //simple solution for systems with enough RAM?
            //prepend wakeup chars
            for ( uint8_t i = 0; i < pServiceAccessPoint->GetWUpChars(); i++ ) {
                pCB->append( SLIP_Begin );
            }
            break;

        }
    }

    if ( NotFound ) {
        printf("No SAP for SAPid = %d.", sapID );
        pSMin->clear();
        return StatusCode::ERROR;
    }

    //ietverts Pipe, izsauc ar ByteArray atvasinaatiem objektiem,
    StatusCode status = SLIP_Encode( pSMin, pCBout );
    pSMin->clear();

    return status;

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


/*
static const InitRecord InitArray[] = {
    { nullptr,  PipeSwitch,     nullptr },  //CB4, SM
    { nullptr,  SendMessage,    nullptr },  //SM, CB
    { nullptr,  TX1_Write,      nullptr }   //CB
};
*/

static const InitRecord InitArray[] = {
    { nullptr,  SendMessage,    nullptr },  //SM, CB
    { nullptr,  TX1_Write,      nullptr }   //CB
};

void setup_TX1_Radio( void ) {
    if ( pPipelines[SERIAL1OUT] ) {

        printf("Making pPipelines[SERIAL1OUT]:\r\n");

        StatusCode status = pPipelines[SERIAL1OUT]->Setup(
            InitArray,
            sizeof( InitArray ) / SIZE_OF_INITRECORD );

        //SM
        if ( StatusCode::OK == status ) {

            printf("TX1 SMessage");
            SerialMessage* pSM = new SerialMessage( CBuffSizes[SERIAL1OUT] );
            pPipelines[SERIAL1OUT]->setInputBuffer( 1, pSM );
            if ( pPipelines[SERIAL1OUT]->getInputBuffer( 1 ) != pSM ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        //output CB
        if ( StatusCode::OK == status ) {

            printf("TX1 CBuffer");
            pPipelines[SERIAL1OUT]->setOutputBuffer( 1, pCBuffs[SERIAL1OUT] );
            if ( pPipelines[SERIAL1OUT]->getOutputBuffer( 1 ) != pCBuffs[SERIAL1OUT] ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            pPipelines[SERIAL1OUT]->setInputBuffer( 2, pCBuffs[SERIAL1OUT] );
            if ( pPipelines[SERIAL1OUT]->getInputBuffer( 2 ) != pCBuffs[SERIAL1OUT] ) {
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
