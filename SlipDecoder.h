/**
 * @file    SlipDecoder.h
 *
 * @brief   Declaration of class SlipDecoder
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#ifndef _Slip_Decoder_H_
#define _Slip_Decoder_H_

#include "Slip.h"
#include "Pipeline.h"


/**
 * decoder states
 */
enum DecoderState {
    SLIP_STInitial = 0,
    SLIP_STInFrame,
    SLIP_STEscState
};


void        SLIP_Reset( void );

//actually input is CircularBuffer*
StatusCode  SLIP_Decode( ByteArray* pInput, ByteArray* pOutput );
//void        Decode( uint8_t input, ByteArray* pOutput );


#endif // _Slip_Decoder_H_
