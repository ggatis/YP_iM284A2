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
 * @param   input       buffer with original messsage
 *
 * @param   output      buffer for encoded message
 *
 * @return  status
 */
StatusCode SLIP_Encode( ByteArray* pInput, ByteArray* pOutput ) {

    pOutput->append( SLIP_End );

    for ( int index = 0; index < pInput->count(); index++ ) {

        uint8_t byte = pInput->at( index );

        switch ( byte ) {
        case SLIP_End:
                pOutput->append( SLIP_Esc );
                pOutput->append( SLIP_EscEnd );
                break;

        case SLIP_Esc:
                pOutput->append( SLIP_Esc );
                pOutput->append( SLIP_EscEsc );
                break;

        default:
                pOutput->append( byte );
                break;
        }

    }

    pOutput->append( SLIP_End );

    return StatusCode::OK;

}
