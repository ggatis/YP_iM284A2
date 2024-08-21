/**
 * @file    ServiceAccessPoint.cpp
 *
 * @brief   Implementation of class ServiceAccessPoint
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "ServiceAccessPoint.h"
#include "SlipEncoder.h"


/**
 * @brief   class constructor
 *
 * @param   sapID   ID of this service access point
 *
 * @param   port    a serial port
 */
ServiceAccessPoint::ServiceAccessPoint( uint8_t sapID, int numWakeupChars )
                  : _SapID          ( sapID )
                  , _NumWakeupChars ( numWakeupChars ) {
}


uint8_t
ServiceAccessPoint::GetSAPid( void ) {
    return _SapID;
}


uint16_t
ServiceAccessPoint::GetWUpChars( void ) {
    return _NumWakeupChars;
}


/**
 * @brief   send a simple command without payload
 *
 * @param   reqID       ID of outgoing command
 *
 * @return  true/false
 */
StatusCode
ServiceAccessPoint::CreateMessage( uint8_t reqID, SerialMessage* pSMout ) {
    pSMout->InitRequest( _SapID, reqID );
    //calculate and append CRCC16
    pSMout->Append_CRC16();
    return StatusCode::NEXT;
//    return SendMessage( msg );
}
