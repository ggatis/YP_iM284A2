/**
 * @file    SlipEncoder.cpp
 *
 * @brief   Implementation of class SlipEncoder.
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#include "YP_iM284A2.h"                             //global data
#include "SlipEncoder.h"


/**
 * @brief   encode message
 *
 * @param   input       SerialMessage buffer with original messsage
 *
 * @param   output      CircularBuffer for encoded message
 *
 * @return  status
 */
//StatusCode status = SLIP_Encode( pSMin, pCBout );
StatusCode SLIP_Encode( ByteArray* pSMin, ByteArray* pCBout ) {

    SerialMessage*  pSM = (SerialMessage*)pSMin;
    CircularBuffer* pCB = (CircularBuffer*)pCBout;

    pCB->append( SLIP_End );

    for ( int index = 0; index < pSM->count(); index++ ) {

        uint8_t byte = pSM->at( index );

        switch ( byte ) {
        case SLIP_End:
            pCB->append( SLIP_Esc );
            pCB->append( SLIP_EscEnd );
            break;

        case SLIP_Esc:
            pCB->append( SLIP_Esc );
            pCB->append( SLIP_EscEsc );
            break;

        default:
            pCB->append( byte );
            break;
        }

    }

    pCB->append( SLIP_End );

    return StatusCode::OK;

}
