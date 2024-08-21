/**
 * @file    SlipDecoder.cpp
 *
 * @brief   Implementation of class SlipDecoder.
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
#include "SlipDecoder.h"
#include "CircularBuffer.h"


static DecoderState _State;


/**
 * @brief   reset decoder
 */

void SLIP_Reset() {
    _State = SLIP_STInitial;
}


/**
 * @brief   decode SLIP stream
 *
 * @param   output      decoded data
 *
 * @param   input       incoming SLIP encoded data
 *
 * @note    _Client->OnSlipDecoder_MessageReady is called with decoded SLIP message
 *
 */
StatusCode SLIP_Decode( ByteArray* pInput, ByteArray* pOutput ) {

    //we know what we have on inputs
    CircularBuffer* pCBin = (CircularBuffer*)pInput;

    if ( 0 == pCBin->count() ) return StatusCode::PENDING;

    for ( int index = 0; index < pCBin->count(); index++ ) {

        uint8_t byte = pCBin->get();

        switch ( _State ) {
        case SLIP_STInitial:

            // begin of SLIP frame ?
            if ( SLIP_Begin == byte ) {
                // reset output buffer
                pOutput->clear();
                _State = SLIP_STInFrame;
            }
            break;

        case SLIP_STInFrame:

            // end of SLIP frame ?
            if ( SLIP_End == byte ) {
                // notify client that SLIP frame is ready in output buffer
                if ( 0 < pOutput->count() ) {
                    return StatusCode::OK;
                }
                // reset output buffer
                //output.clear();   //somewere after the output is taken
            }
            // SLIP esc ?
            else if ( SLIP_Esc == byte ) {
                _State = SLIP_STEscState;
            }
            // default case
            else {
                pOutput->append( byte );
            }
            break;

        case  SLIP_STEscState:

            // end of escape state ?
            if ( SLIP_EscEnd == byte ) {
                pOutput->append( SLIP_End );
                _State = SLIP_STInFrame;
            }
            // end of escape state ?
            else if ( SLIP_EscEsc == byte ) {
                pOutput->append( SLIP_Esc );
                _State = SLIP_STInFrame;
            }
            // error
            else {
                // abort frame reception -> return to initial state
                _State = SLIP_STInitial;
            }
            break;

        } // switch ( _State )

    } // for...

    return StatusCode::PENDING;

}

/**
 * @brief   decode a byte from the encoded SLIP stream
 *
 * @param   output      decoded frame
 * @param   input       a byte from the encoded SLIP byte stream
 *
 * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
 */
/*
void
SlipDecoder::SLIP_Decode( ByteArray& output, int input ) {

    if ( -1 < input ) {

        uint8_t byte = (uint8_t)input;

        switch ( _State ) {
        case SlipDecoder::Initial:

            //begin of SLIP frame ?
            if ( SlipDecoder::Begin == byte ) {
                // reset output buffer
                output.clear();
                _State = SlipDecoder::InFrame;
            }
            break;

        case SlipDecoder::InFrame:

            //end of SLIP frame ?
            if ( SlipDecoder::End == byte ) {
                //notify client that SLIP frame is ready in output buffer
                if ( _DecoderClient && ( output.count() > 0 ) ) {
                    _DecoderClient->OnSlipDecoder_MessageReady( output );
                }
                //reset output buffer
                output.clear();
            }
            //SLIP esc ?
            else if ( SlipDecoder::Esc == byte ) {
                _State = SlipDecoder::EscState;
            }
            //default case
            else {
                output.append( byte );
            }
            break;

        case  SlipDecoder::EscState:

            //end of escape state ?
            if ( SlipDecoder::EscEnd == byte ) {
                output.append( SlipDecoder::End );
                _State = InFrame;
            }
            //end of escape state ?
            else if ( SlipDecoder::EscEsc == byte ) {
                output.append( SlipDecoder::Esc );
                _State = SlipDecoder::InFrame;
            }
            //error
            else {
                //abort frame reception -> return to initial state
                _State = SlipDecoder::Initial;
            }
            break;

        } //switch ( _State )

    }
}
*/

/**
 * @brief   decode an encoded SLIP stream using flagged callback
 *
 * @param   output          decoded frame
 * @param   flByteStream    function returning a byte/-1 from the encoded SLIP byte stream
 *
 * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
 */
/*
void
SlipDecoder::SLIP_Decode( ByteArray& output, int (*const flByteStream)( void ) ) {

    int input;

    while ( -1 < ( input = flByteStream() ) ) {

        uint8_t byte = (uint8_t)input;

        switch ( _State ) {
        case SlipDecoder::Initial:

            //begin of SLIP frame ?
            if ( SlipDecoder::Begin == byte ) {
                //reset output buffer
                output.clear();
                _State = SlipDecoder::InFrame;
            }
            break;

        case SlipDecoder::InFrame:

            //end of SLIP frame ?
            if ( SlipDecoder::End == byte ) {
                //notify client that SLIP frame is ready in output buffer
                if ( _DecoderClient && ( output.count() > 0 ) ) {
                    _DecoderClient->OnSlipDecoder_MessageReady( output );
                }
                //reset output buffer
                output.clear();
            }
            //SLIP esc ?
            else if ( SlipDecoder::Esc == byte ) {
                _State = SlipDecoder::EscState;
            }
            // default case
            else {
                output.append( byte );
            }
            break;

        case  SlipDecoder::EscState:

            //end of escape state ?
            if ( SlipDecoder::EscEnd == byte ) {
                output.append( SlipDecoder::End );
                _State = InFrame;
            }
            //end of escape state ?
            else if ( SlipDecoder::EscEsc == byte ) {
                output.append( SlipDecoder::Esc );
                _State = SlipDecoder::InFrame;
            }
            //error
            else {
                //abort frame reception -> return to initial state
                _State = SlipDecoder::Initial;
            }
            break;

        } //switch( _State )

    } //while...
}
*/
