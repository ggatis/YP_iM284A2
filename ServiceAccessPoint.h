/**
 * @file    ServiceAccessPoint.h
 *
 * @brief   Declaration of class ServiceAccessPoint
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef _ServiceAccessPoint_H_
#define _ServiceAccessPoint_H_

#include "Pipeline.h"
#include "SerialMessage.h"
#include "Dictionary.h"
#include "CircularBuffer.h"


class ServiceAccessPoint {

public:
                                ServiceAccessPoint( uint8_t sapID, int numWakeupChars = 0 );

    //<! handle incoming messages
    //static bool                 OnDispatchMessage( SerialMessage* serialMsg, Dictionary* result );
    uint8_t                     GetSAPid( void );
    uint16_t                    GetWUpChars( void );

    virtual StatusCode          OnDecodeMessage(
                                    const SerialMessage* pSMin, Dictionary* pDictOut ) {
        return StatusCode::ERROR;
    }

protected:

    //<! helpers for outgoing messages
    //bool                      SendMessage( uint8_t reqID );
    StatusCode                  CreateMessage( uint8_t reqID, SerialMessage* pSMout );
    //bool                      SendMessage( SerialMessage& serialMsg );

protected:

    //<! SAP identifier
    uint8_t                     _SapID;

    //<! wakeup chars for sleeping, power saving end nodes
    uint16_t                    _NumWakeupChars;

};


#endif // _ServiceAccessPoint_H_
