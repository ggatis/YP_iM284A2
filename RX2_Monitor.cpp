#include "YP_iM284A2.h"                             //global data
#include "RX2_Monitor.h"
#include "Pipeline.h"
#include "CircularBuffer.h"


StatusCode parser( ByteArray* pin, ByteArray* pout ) {
    
    CircularBuffer* pinCB = (CircularBuffer*)pin;
    //printf("parser pinCB->count(): %d\r\n", pinCB->count() );

    if ( pinCB->count() ) {

        uint8_t maxi = pinCB->count();
        if ( 20 < ( pout->count() + maxi ) ) {
            maxi = 20 - pout->count();
        }
        //printf("parser maxi:%d", maxi );
        for ( uint8_t i = 0; i < maxi; i++ ) {
            //printf(", %d", pinCB->count() );
            pout->append( pinCB->get() );
        }
        //printf("\r\n");

    }
    
    //printf("parser pout->count(): %d\r\n", pout->count() );

    if ( 20 > pout->count() ) {
        if ( ( lastIOticks[2] + 2000 ) > mySysTick ) {
            return StatusCode::PENDING;
        }
    }

    return StatusCode::OK;
}


StatusCode process( ByteArray* pin, ByteArray* pout ) {

    uint8_t maxi = 20;
    if ( 20 > pin->count() ) {
        maxi = pin->count();
    }
    if ( maxi ) {
        if ( 20 > maxi ) {
            printf("Monitor TO: ");
        } else {
            printf("Monitor:    ");
        }
        pin->printHEX();
        maxi += maxi;
        while ( maxi++ < 41 ) {
            printf(" ");
        }
        pin->print();
        pin->clear();
        printf("\r\n");
    }

    return StatusCode::OK;
}

void myErrorHandler( Pipeline* pPipeline, StatusCode ErrorCode ) {
    switch ( ErrorCode ) {
    case StatusCode::PENDING:
        if ( 1 != pPipeline->getFaultyPipe() ) {
            printf("Pending %d bytes in pipe: %d.\r\n",
                pPipeline->getFrontEnd()->count(),
                pPipeline->getFaultyPipe() );
        }
        break;
    case StatusCode::ERROR:
        printf("Processing failed in pipe: %d.\r\n", pPipeline->getFaultyPipe() );
        break;
    default:
        printf("Processing done OK.\r\n");
    }
}


static const InitRecord InitArray[] = {
    { nullptr,  parser,     nullptr },
    { nullptr,  process,    nullptr }
};

void setup_RX2_Monitor( void ) {
    
    if ( pPipelines[SERIAL2IN] ) {

        printf("Making pPipelines[SERIAL2IN]:\r\n");

        StatusCode status = pPipelines[SERIAL2IN]->Setup(
            InitArray,
            sizeof( InitArray ) / SIZE_OF_INITRECORD );

        //input CB
        if ( StatusCode::OK == status ) {

            printf("RX2 CBuffer");
            pPipelines[SERIAL2IN]->setInputBuffer( 1, pCBuffs[SERIAL2IN] );
            if ( pPipelines[SERIAL2IN]->getInputBuffer( 1 ) != pCBuffs[SERIAL2IN] ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            printf("\r\n");

        }

        //intermediate BA
        if ( StatusCode::OK == status ) {

            printf("RX2 BArray");
            ByteArray* pBA = new ByteArray( 20 );
            if ( nullptr == pBA ) {
                printf(" <- Error");
                status = StatusCode::ERROR;
            }
            if ( StatusCode::OK == status ) {
                pPipelines[SERIAL2IN]->setOutputBuffer( 1, pBA );
                if ( pPipelines[SERIAL2IN]->getOutputBuffer( 1 ) != pBA ) {
                    printf(" <- Error");
                    status = StatusCode::ERROR;
                }
            }
            if ( StatusCode::OK == status ) {
                pPipelines[SERIAL2IN]->setInputBuffer( 2, pBA );
                if ( pPipelines[SERIAL2IN]->getInputBuffer( 2 ) != pBA ) {
                    printf(" <- Error");
                    status = StatusCode::ERROR;
                }
            }
            printf("\r\n");

        }

        //ErrorHandler
        if ( StatusCode::OK == status ) {
            pPipelines[SERIAL2IN]->setErrorHandler( myErrorHandler );
        }

        printf("pPipelines[SERIAL2IN] is ");
        if ( StatusCode::OK != status ) {
            printf("not");
        }
        printf(" ready!\r\n");

    } else {

        printf("No pPipelines[SERIAL2IN] :(\r\n");

    }

}
