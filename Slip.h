/**
 * @file    Slip.h
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

#ifndef _Slip_H_
#define _Slip_H_


/**
 * standard SLIP frame characters
 */
enum SLIP_FrameCharacters {
    SLIP_Begin   =   0xC0,
    SLIP_End     =   0xC0,
    SLIP_Esc     =   0xDB,
    SLIP_EscEnd  =   0xDC,
    SLIP_EscEsc  =   0xDD
};


#endif // _Slip_H_
