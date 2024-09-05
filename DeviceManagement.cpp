/**
 * @file    DeviceManagement.cpp
 *
 * @brief   Implementation of class DeviceManagement
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include <string>
#include <cstring>  //strstr

#include "YP_iM284A2.h"
#include "DeviceManagement.h"

//#include <QDateTime>
#include "CurrentTime.h"


//<! map with status code strings
//const aMap < uint8_t, std::string > DeviceManagement::_StatusCodes = {
const aMap < uint8_t, const char* > DeviceManagement::_StatusCodes = {
    { Ok,                       "ok" },
    { Error,                    "error" },
    { CommandNotSupported,      "command not supported" },
    { WrongParameter,           "wrong parameter" },
    { WrongApplicationMode,     "wrong application mode" },
    { Reserved,                 "reserved" },
    { ApplicationBusy,          "application busy" },
    { WrongMessageLength,       "wrong message length" },
    { NVM_WriteError,           "NVM write error" },
    { NVM_ReadError,            "NVM read error" },
    { CommandRejected,          "command rejected" }
};

//<! map with module type strings
//const aMap < uint8_t, std::string > DeviceManagement::_ModuleTypes = {
const aMap < uint8_t, const char* > DeviceManagement::_ModuleTypes = {
    { 104,  "iM284A-XL" },
    { 109,  "iM891A-XL" },
    { 110,  "iU891A-XL" },
    { 163,  "iM881A-XL" }
};

//<! map with response & event names for HCI messages
//const aMap < uint8_t, std::string > DeviceManagement::_EventNames = {
const aMap < uint8_t, const char* > DeviceManagement::_EventNames = {
    { Startup_Ind,              "startup indication" },
    { Ping_Rsp,                 "ping device response" },
    { GetDeviceInfo_Rsp,        "get device info response" },
    { GetFirmwareVersion_Rsp,   "get firmware version response" },
    { GetDateTime_Rsp,          "get date time response" },
    { SetDateTime_Rsp,          "set date time response" },
    { RestartDevice_Rsp,        "restart device response" },
    { SetSystemOptions_Rsp,     "set system options response" },
    { GetSystemOptions_Rsp,     "get system options response" }
};

//<! map with message handlers for HCI messages
const aMap < uint8_t , DeviceManagement::Handler > DeviceManagement::_Handlers = {
    { Startup_Ind,              &DeviceManagement::OnStartupIndication },
    { Ping_Rsp,                 &DeviceManagement::OnDefaultResponse },
    { GetDeviceInfo_Rsp,        &DeviceManagement::OnDeviceInfoResponse },
    { GetFirmwareVersion_Rsp,   &DeviceManagement::OnFirmwareVersionResponse },
    { GetDateTime_Rsp,          &DeviceManagement::OnDateTimeResponse },
    { SetDateTime_Rsp,          &DeviceManagement::OnDefaultResponse },
    { RestartDevice_Rsp,        &DeviceManagement::OnDefaultResponse },
    { SetSystemOptions_Rsp,     &DeviceManagement::OnDefaultResponse },
    { GetSystemOptions_Rsp,     &DeviceManagement::OnSystemOptionsResponse }
};

/**
 * @brief   class constructor
 *
 * @param
 */
DeviceManagement::DeviceManagement( void )
                : ServiceAccessPoint( DeviceManagement::Sap_ID ) {
}


/**
 * @brief   send "ping request"
 */
bool
DeviceManagement::OnPingDevice() {
    //return SendMessage( Ping_Req );
    return CreateMessage( Ping_Req );
}


/**
 * @brief   send "get device information request"
 */
bool
DeviceManagement::OnGetDeviceInfo() {
    //return SendMessage( GetDeviceInfo_Req );
    return CreateMessage( GetDeviceInfo_Req );
}


/**
 * @brief   send "get firmware version request"
 */
bool
DeviceManagement::OnGetFirmwareVersion() {
    //return SendMessage( GetFirmwareVersion_Req );
    return CreateMessage( GetFirmwareVersion_Req );
}


/**
 * @brief   send "get date time request"
 */
bool
DeviceManagement::OnGetDateTime() {
    //return SendMessage( GetDateTime_Req );
    return CreateMessage( GetDateTime_Req );
}


/**
 * @brief   send "set date time request"
 */
bool
DeviceManagement::OnSetDateTime() {

    if ( pPipelines[SERIAL1OUT] ) {

        //no Radio
        //SerialMessage msg( DeviceManagement::Sap_ID, SetDateTime_Req );
        SerialMessage* pSMout = (SerialMessage*)pPipelines[SERIAL1OUT]->getInputBuffer( 1 );

        if ( pSMout ) {
            //make message
            pSMout->InitRequest( _SapID, SetDateTime_Req );
            //uint32_t secondsSincePeriod = QDateTime::currentSecsSinceEpoch();
            uint32_t secondsSincePeriod = getCurrentTimeInSeconds();
            pSMout->Append( secondsSincePeriod );
            //calculate and append CRCC16
            pSMout->Append_CRC16();
            return true;
        }
    }

    return false;

}


/**
 * @brief   send "restart device request"
 */
bool
DeviceManagement::OnRestartDevice() {
    //return SendMessage( RestartDevice_Req );
    return CreateMessage( RestartDevice_Req );
}


/**
 * @brief   send "get system options request"
 */
bool
DeviceManagement::OnGetSystemOptions() {
    //return SendMessage( GetSystemOptions_Req );
    return CreateMessage( GetSystemOptions_Req );
}


/**
 * @brief   send "set system options request"
 */
bool
DeviceManagement::OnSetSystemOptions( const Dictionary& params ) {

    uint32_t    mask    = 0;
    uint32_t    options = 0;

    char*       data    = nullptr;

    static const char* Trace_on     = "Trace = on";
    static const char* Trace_off    = "Trace = off";
    static const char* Start_on     = "Startup Event = on";
    static const char* Start_off    = "Startup Event = off";

    if ( ( data = (char*)params.contains("Options") ) ) {
        if ( strstr( data, Trace_on ) ) {

            mask    |= SO_Trace;
            options |= SO_Trace;

        } else if ( strstr( data, Trace_off ) ) {

            mask    |= SO_Trace;

        }
        if ( strstr( data, Start_on ) ) {

            mask    |= SO_StartupEvent;
            options |= SO_StartupEvent;

        } else if ( strstr( data, Start_off ) ) {

            mask    |= SO_StartupEvent;

        }
    }

    //SerialMessage msg( DeviceManagement::Sap_ID, SetSystemOptions_Req );
    //msg.Append( mask );
    //msg.Append( options );
    //return SendMessage( msg );

    if ( pPipelines[SERIAL1OUT] ) {

        //no Radio
        //SerialMessage msg( DeviceManagement::Sap_ID, SetDateTime_Req );
        SerialMessage* pSMout = (SerialMessage*)pPipelines[SERIAL1OUT]->getInputBuffer( 1 );

        if ( pSMout ) {
            //make message
            pSMout->InitRequest( _SapID, SetSystemOptions_Req );
            //append data
            pSMout->Append( mask );
            pSMout->Append( options );
            //calculate and append CRCC16
            pSMout->Append_CRC16();
            return true;
        }
    }

    return false;

}


/**
 * @brief   find message handler and decode convert message into Dictionary format
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   pDictOut        decoded data in Dictionary format
 *
 * @return  StatusCode
 */
StatusCode
DeviceManagement::OnDecodeMessage( const SerialMessage* pSMin, Dictionary* pDictOut ) {

    uint8_t msgID = pSMin->GetMsgID();

    //find handler by message ID
    Handler handler = _Handlers.value( msgID, nullptr );
    if ( handler != nullptr ) {
        //get handler event name
        pDictOut->append(
            "Event",
            _EventNames.value( msgID, "unknown handler name" ) );

        //call message handler
        return ( this->*handler )( pSMin, pDictOut ) ? StatusCode::OK : StatusCode::ERROR;
    }
    //no handler found
    
    pDictOut->append  ("Error", "unsupported MsgID: ");
    pDictOut->appendU8( msgID );
    pDictOut->append  (" received");
    return StatusCode::OK;

}


/**
 * @brief   decode default response, HCI status only
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false      true = OK
 */
//bool
//DeviceManagement::OnDefaultResponse( const SerialMessage& serialMsg, Dictionary& result ) {
bool
DeviceManagement::OnDefaultResponse( const SerialMessage* pSMin, Dictionary* pDictOut ) {
    //check minimum payload length
    if ( pSMin->GetPayloadLength() > 0 ) {
        pDictOut->append(
            "Status",
            _StatusCodes.value( pSMin->GetResponseStatus(), "error" ) );
        return true;
    }
    return false;
}


/**
 * @brief   decode startup indication
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
//bool
//DeviceManagement::OnStartupIndication( const SerialMessage& serialMsg, Dictionary& result );
bool
DeviceManagement::OnStartupIndication( const SerialMessage* pSMin, Dictionary* pDictOut ) {
    //check minimum payload length
    if ( pSMin->GetPayloadLength() <
        ( ReservedInfo_Size + DeviceInfo_Size + FirmwareInfo_MinSize ) )
        return false;

#if ReservedInfo_Size > 0
    pDictOut->append("Reserved Info",
        pSMin->GetHexString( SerialMessage::EventData_Index, ReservedInfo_Size ) );
    //buutu jaapaartaisa!!
    //pDictOut->append("Reserved Info", "");
    //pDictOut->AppendHexString(
    //    pSMin->data() + SerialMessage::EventData_Index,
    //    ReservedInfo_Size );
#endif
    //nocekot, vai Dict pievieno ""!!
    pDictOut->append("Device Info", "");
    DecodeDeviceInfo(
        pSMin->data() + SerialMessage::EventData_Index + ReservedInfo_Size,
        pDictOut );
    pDictOut->append("Firmware Info", "");
    DecodeFirmwareInfo(
        pSMin->data() + SerialMessage::EventData_Index + ReservedInfo_Size + DeviceInfo_Size,
        pDictOut );

    return true;

}


/**
 * @brief   decode get device information response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
//DeviceManagement::OnDeviceInfoResponse( const SerialMessage* serialMsg, Dictionary* result ) {
bool
DeviceManagement::OnDeviceInfoResponse( const SerialMessage* pSMin, Dictionary* pDictOut ) {

    //check minimum response payload length
    if ( pSMin->GetResponsePayloadLength() < DeviceInfo_Size )
        return false;

    uint8_t status = pSMin->GetResponseStatus();
    pDictOut->append("Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {
        pDictOut->append("Device Info", "");
        //DecodeDeviceInfo( serialMsg, SerialMessage::ResponseData_Index );
        DecodeDeviceInfo(
            pSMin->data() + SerialMessage::ResponseData_Index,
            pDictOut );
    }
    return true;
}


/**
 * @brief   decode get firmware version response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
//DeviceManagement::OnFirmwareVersionResponse( const SerialMessage& serialMsg, Dictionary& result ) {
bool
DeviceManagement::OnFirmwareVersionResponse( const SerialMessage* pSMin, Dictionary* pDictOut ) {

    //check minimum payload length
    if ( pSMin->GetResponsePayloadLength() < FirmwareInfo_MinSize )
        return false;

    uint8_t status = pSMin->GetResponseStatus();
    pDictOut->append("Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {
        pDictOut->append("Firmware Info", "");
        DecodeDeviceInfo(
            pSMin->data() + SerialMessage::ResponseData_Index,
            pDictOut );
    }
    return true;
}


/**
 * @brief   decode get date time response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
//DeviceManagement::OnDateTimeResponse( const SerialMessage& serialMsg, Dictionary& result ) {
bool
DeviceManagement::OnDateTimeResponse( const SerialMessage* pSMin, Dictionary* pDictOut ) {
    // check minimum payload length
    if ( pSMin->GetResponsePayloadLength() < ( 4 ) )
        return false;

    uint8_t status = pSMin->GetResponseStatus();
    pDictOut->append("Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {
        //Dictionary info;
        //...
        //result[ "Date Time Info" ]      =   info;
        pDictOut->append(
            "Date Time Info: Seconds since epoch",
            std::to_string( pSMin->GetU32( SerialMessage::ResponseData_Index ) ) );
        pDictOut->append(
            "Date Time Info: Date Time",
            pSMin->GetDateTime( SerialMessage::ResponseData_Index, "dd-MM-yyyy hh:mm:ss" ) );
    }
    return true;
}

/**
 * @brief   decode get system options response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
//DeviceManagement::OnSystemOptionsResponse( const SerialMessage& serialMsg, Dictionary& result ) {
bool
DeviceManagement::OnSystemOptionsResponse( const SerialMessage* pSMin, Dictionary* pDictOut ) {
    //check minimum payload length sID+mID+CRC16
    if ( pSMin->GetResponsePayloadLength() < 4 )
        return false;

    uint8_t status = pSMin->GetResponseStatus();
    pDictOut->append( "Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {

        uint32_t options                =   pSMin->GetU32( SerialMessage::ResponseData_Index );

        pDictOut->append("System Options.Options.APS",
            ( options & SO_APS )            ? "on" : "off");
        pDictOut->append("System Options.Options.Trace",
            ( options & SO_Trace )          ? "on" : "off");
        pDictOut->append("System Options.Options.RTC",
            ( options & SO_RTC )            ? "on" : "off");
        pDictOut->append("System Options.Options.WatchDog",
            ( options & SO_WatchDog )       ? "on" : "off");
        pDictOut->append("System Options.Options.Startup Event",
            ( options & SO_StartupEvent )   ? "on" : "off");

    }
    return true;
}

/**
 * @brief   decode Device Info Field
 *
 * @param   serialMsg       incoming HCI message
 * @param   index           index to device info field
 *
 * @return  Dictionary including decoded data
 */
#define ModuleType_pos      0
#define ModuleID_pos        1
#define ProductType_pos     5
#define ProductID_pos       9
bool
DeviceManagement::DecodeDeviceInfo( const uint8_t* pDeviceInfo, Dictionary* pDictOut ) const {

    uint8_t moduleType      =   pDeviceInfo[ModuleType_pos];
    //info.append("Module Type",
    //    _ModuleTypes.value( moduleType, "unknown module type:" + std::to_string( moduleType ) ) );
    pDictOut->append("Device Info/Module Type", "\0");
    if ( 0 < _ModuleTypes.contains( moduleType ) ) {
        pDictOut->append( _ModuleTypes.value( moduleType, "\0" ) );
    } else {                                                
        pDictOut->append("unknown module type: ");
        pDictOut->appendU8( moduleType );
    }
    //
    pDictOut->print();
    //

    pDictOut->append("Device Info/Module ID", *(uint32_t*)( pDeviceInfo + ModuleID_pos ) );
    //
    pDictOut->print();
    //

    pDictOut->append("Device Info/Product Type", "\0");
    pDictOut->appendHEX( pDeviceInfo + ProductType_pos, 4 );
    //
    pDictOut->print();
    //

    pDictOut->append("Device Info/Product ID", "\0");
    pDictOut->appendHEX( pDeviceInfo + ProductID_pos, 4 );
    //
    pDictOut->print();
    //

    return true;

}


/**
 * @brief   decode Firmware Info Field
 *
 * @param   serialMsg       incoming HCI message
 * @param   index           index to firmware info field
 *
 * @return  Dictionary including decoded data
 */
#define FWVersionLSB_pos    0
#define FWVersionMSB_pos    1
#define BuildCount_pos      2
#define BuildDate_pos       4
#define FirmwareName_pos   14
bool
DeviceManagement::DecodeFirmwareInfo( const uint8_t* pFirmwareInfo, Dictionary* pDictOut ) const {
    //@pFirmwareInfo:
    //Fimrware VersionLSB(1) + VersionMSB(1) + BuildCount(2) +  BuildDate(10) + FirmwareName( > 1 )

    pDictOut->append("FirmwareInfo/Version", *( pFirmwareInfo + FWVersionMSB_pos ) );
    pDictOut->append(".");
    pDictOut->appendU8( *( pFirmwareInfo + FWVersionLSB_pos ) );
    //
    pDictOut->print();
    //
      
    pDictOut->append("FirmwareInfo/Build Count", (uint32_t)*(uint16_t*)( pFirmwareInfo + BuildCount_pos ) );
    //
    pDictOut->print();
    //

    pDictOut->append("FirmwareInfo/Build Date", pFirmwareInfo + BuildDate_pos, 10 );
    //
    pDictOut->print();
    //

    pDictOut->append("FirmwareInfo/Firmware Name", pFirmwareInfo + FirmwareName_pos );
    //
    pDictOut->print();
    //

    return true;

}
