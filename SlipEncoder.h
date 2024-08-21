/**
 * @file    SlipEncoder.h
 *
 * @brief   Declaration of class SlipEncoder
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#ifndef _Slip_Encoder_H_
#define _Slip_Encoder_H_

#include "Slip.h"
#include "Pipeline.h"


//actually output is CircularBuffer*
StatusCode SLIP_Encode( ByteArray* pInput, ByteArray* pOutput );


#endif // _Slip_Encoder_H_
