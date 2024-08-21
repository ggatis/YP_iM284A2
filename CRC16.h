/**
 * @file    CRC16.h
 *
 * @brief   Declaration of class CRC16
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#ifndef _CRC16_H_
#define _CRC16_H_

#include <stdint.h>
#include "ByteArray.h"


enum CRC16_Values {
    CRC16_Init_Value  =  0xFFFF,    //!< initial value for CRC algorithem
    CRC16_Good_Value  =  0x0F47,    //!< constant compare value for check
    CRC16_Polynom     =  0x8408     //!< 16 Bit CRC CCITT Generator Polynom, used for table generation
};


void                    CRC16_Init( uint16_t initValue = CRC16_Init_Value );

uint16_t                CRC16_Calc( const ByteArray* data );
uint16_t                CRC16_Calc_X25( const ByteArray* data );

bool                    CRC16_Check_X25( const ByteArray* data );


#endif // _CRC16_H_
