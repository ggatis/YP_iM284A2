/**
 * @file    LoRa_Mesh_DemoApp.cpp
 *
 * @brief   Implementation of class LoRa_Mesh_DemoApp
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "LoRa_Mesh_DemoApp.h"
#include "Dictionary.h"
#include "CircularBuffer.h"
#include "iM284A.h"
#include "YP_iM284A2.h"


void ( *LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnTestRadioSerialMonitor+1] )( void ) = {

    //<! commands related to this application
    &OnShowMenu,
    &OnExit,

    //<! commands for DeviceManagement messages
    &OnPingDevice,

    &OnGetDeviceInformation,
    &OnGetFirmwareVersion,

    &OnGetDateTime,
    &OnSetDateTime,

    &OnRestartDevice,

    &OnGetSystemOptions,
    &OnSetSystemOptions,

    //<! commands for LoRaMesh Router messages
    &OnGetNetworkAddress,
    &OnSetNetworkAddress_A,
    &OnSetNetworkAddress_B,

    &OnGetMode,
    &OnDisableRouter,
    &OnEnableRouter,
    &OnEnableCoordinator,

    &OnGetLinkStatus,
    &OnGetRoutingInfo,

    &OnSendPacketToNode_A,
    &OnSendPacketToNode_B,

    &OnPrintDemoApp,
    &OnTestRadioSerialMonitor
};

/**
 * @brief   class constructor of main application
 * @param   console     reference to a console
 */

//LoRa_Mesh_DemoApp::LoRa_Mesh_DemoApp( HardwareSerial& RadioSerial, USBSerial& HMISerial ) :
void setup_DemoApp( void ) {

    pDemoApp->_Network_ID           = "00-2A";
    pDemoApp->_DeviceEUI_Node_A     = "01-aa-aa-aa-02-aa-aa-aa";
    pDemoApp->_DeviceEUI_Node_B     = "03-bb-bb-bb-04-bb-bb-bb";
    pDemoApp->_User_Port            =  21;
    pDemoApp->_Payload_for_Node_A   = "AA-01-02-02-04-05-06-07-08-AA";
    pDemoApp->_Payload_for_Node_B   = "BB-01-02-02-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F-BB";

}


/**
 *  @brief  some command handler
 */

void OnShowMenu( void ) {
    printf("OnShowMenu\r\n");
    for ( 
        uint8_t i = 0;
        i < CommandTables[ActiveCommandTable].CommandCount;
        showItem( CommandTables[ActiveCommandTable].pCommandTable, i++ )
    );
}

void OnExit( void ) {
    printf("Exit where?\r\n");
}


/* commands for DeviceManagement messages */

void OnPingDevice( void ) {
    printf("OnPingDevice\r\n");
    //GetDeviceManagement().OnPingDevice();
}

void OnGetDeviceInformation( void ) {
    printf("OnGetDeviceInformation\r\n");
    //GetDeviceManagement().OnGetDeviceInfo();
}

void OnGetFirmwareVersion( void ) {
    printf("OnGetFirmwareVersion\r\n");
    //GetDeviceManagement().OnGetFirmwareVersion();
}

void OnGetDateTime( void ) {
    printf("OnGetDateTime\r\n");
    //GetDeviceManagement().OnGetDateTime();
}

void OnSetDateTime( void ) {
    printf("OnSetDateTime\r\n");
    //GetDeviceManagement().OnSetDateTime();
}

void OnRestartDevice( void ) {
    printf("OnRestartDevice\r\n");
    //GetDeviceManagement().OnRestartDevice();
}

void OnGetSystemOptions( void ) {
    printf("OnGetSystemOptions\r\n");
    //GetDeviceManagement().OnGetSystemOptions();
}

void OnSetSystemOptions( void ) {
    printf("OnSetSystemOptions\r\n");
    //QJsonObject params;
    Dictionary params;

    //params[ "Options" ] = "Trace = off, Startup Event = on";
    params.append("Options", "Trace = off, Startup Event = on");
    params.print();
    //_RadioHub.GetDeviceManagement().OnSetSystemOptions( params );
    //GetDeviceManagement().OnSetSystemOptions( params );
}

/* Commands for LoRaMesh Router messages */

void OnGetNetworkAddress( void ) {
    printf("OnGetNetworkAddress\r\n");
    //_RadioHub.GetLoRaMeshRouter().OnGetNetworkAddress();
}

void OnSetNetworkAddress_A( void ) {
    printf("OnSetNetworkAddress_A\r\n");
    //QJsonObject params;
    Dictionary params;

    //params[ "Network-ID" ]  =   _Network_ID;
    //params[ "Device-EUI" ]  =   _DeviceEUI_Node_A;
    params.append("Network-ID", pDemoApp->_Network_ID );
    params.append("Device-EUI", pDemoApp->_DeviceEUI_Node_A );
    params.print();

    //_RadioHub.GetLoRaMeshRouter().OnSetNetworkAddress( params );
    //GetDeviceManagement().OnSetNetworkAddress( params );
}

void OnSetNetworkAddress_B( void ) {
    printf("OnSetNetworkAddress_B\r\n");
    //QJsonObject params;

    //params[ "Network-ID" ]  =   _Network_ID;
    //params[ "Device-EUI" ]  =   _DeviceEUI_Node_B;

    //_RadioHub.GetLoRaMeshRouter().OnSetNetworkAddress( params );
}

void OnGetMode( void ) {
    printf("OnGetMode\r\n");
    //_RadioHub.GetLoRaMeshRouter().OnGetMode();
}

void OnDisableRouter( void ) {
    printf("OnDisableRouter\r\n");
    //QJsonObject params;

    //params[ "Mode" ]  = "Off";

    //_RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void OnEnableRouter( void ) {
    printf("OnEnableRouter\r\n");
    //QJsonObject params;

    //params[ "Mode" ]  = "Router";

    //_RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void OnEnableCoordinator( void ) {
    printf("OnEnableCoordinator\r\n");
    //QJsonObject params;

    //params[ "Mode" ]  = "Coordinator";

    //_RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}


void OnGetLinkStatus( void ) {
    printf("OnGetLinkStatus\r\n");
    //_RadioHub.GetLoRaMeshRouter().OnGetLinkStatus();
}

void OnGetRoutingInfo( void ) {
    printf("OnGetRoutingInfo\r\n");
    //QJsonObject params;

    //// start index
    //params[ "Index" ]       = 0;

    //// read MaxItems per request
    //params[ "MaxItems" ]    = 4;

    //_RadioHub.GetLoRaMeshRouter().OnGetRoutingInfo( params );
}


void OnSendPacketToNode_A( void ) {
    printf("OnSendPacketToNode_A\r\n");
    //QJsonObject params;

    //params[ "Destination-EUI" ] = _DeviceEUI_Node_A;
    //params[ "Port" ]            = _User_Port;
    //params[ "Payload" ]         = _Payload_for_Node_A;

    //_RadioHub.GetLoRaMeshRouter().OnSendPacket( params );
}

void OnSendPacketToNode_B( void ) {
    printf("OnSendPacketToNode_B\r\n");
    //QJsonObject params;

    //params[ "Destination-EUI" ] = _DeviceEUI_Node_B;
    //params[ "Port" ]            = _User_Port;
    //params[ "Payload" ]         = _Payload_for_Node_B;

    //_RadioHub.GetLoRaMeshRouter().OnSendPacket( params );
}


void OnPrintDemoApp( void ) {
    printf("OnPrintDemoApp\r\n");
    printf(    "Network_ID:         ");
    printf( pDemoApp->_Network_ID );
    printf("\r\nDeviceEUI_Node_A:   ");
    printf( pDemoApp->_DeviceEUI_Node_A );
    printf("\r\nDeviceEUI_Node_B:   ");
    printf( pDemoApp->_DeviceEUI_Node_B );
    printf("\r\nUser_Port:          %d", pDemoApp->_User_Port );
    printf("\r\nPayload_for_Node_A: ");
    printf( pDemoApp->_Payload_for_Node_A );
    printf("\r\nPayload_for_Node_B: ");
    printf( pDemoApp->_Payload_for_Node_B );
}


void OnTestRadioSerialMonitor( void ) {
    printf("OnTestRadioSerialMonitor\r\n");
    if ( pPipelines[SERIAL1OUT] ) {
        printf("Sending Zs...");
        //sink in last/RAW pipes input bufer
        pPipelines[SERIAL1OUT]->Sink( pPipelines[SERIAL1OUT]->getPipeCount() - 1, "ZZZZZ");
        printf(" Done!\r\n");
    } else {
        printf("pPipelines[SERIAL1OUT] not defined\r\n");
    }
}


/**
 * @brief   respond to the HMI: modem commands and menu commands if HMI is allowed
 *
 * @param   in          Circular Buffer for analysis
 *          out         result of analysis
 */
StatusCode OnHMI_DataEvent( ByteArray* pIn, ByteArray* pOut ) {

    static uint8_t  ActiveCommand   = 0;
    static uint32_t HMIinputTimeout = 0;

    if ( ActiveCommand ) {
        if ( TimedOut( &HMIinputTimeout ) ) {
            ActiveCommand = 0;      //rollback visiem uzsaaktajiem procesiem!!
            printf(" <- timeout\r\n");
        }
    }

    CircularBuffer* pCBin   = (CircularBuffer*)pIn;
    
    if ( 0 == pCBin->count() ) return StatusCode::PENDING;

    setTimeOut( &HMIinputTimeout, HMI_TIMEOUT );

    ActiveCommand = pCBin->at( 0 );
    ActiveCommand = ( '<' == ActiveCommand ) ? 27 : ActiveCommand;
    printf("%c", ActiveCommand );

    if ( ActiveCommand ) {
        int16_t item = findItem(
            CommandTables[ActiveCommandTable].pCommandTable,
            CommandTables[ActiveCommandTable].CommandCount,
            ActiveCommand
        );
        if ( -1 == item ) {
            //
            printf(" <- command?\r\n");
        } else {
            if ( CommandTables[ActiveCommandTable].pCommandTable[item].aFunction ) {
                //SerialUSB.print( F(": ") );
                printf(": ");
                //SerialUSB.println( CommandTables[ActiveCommandTable].pCommandTable[item].cDescription );
                printf( CommandTables[ActiveCommandTable].pCommandTable[item].cDescription );
                printf("\r\n");
                CommandTables[ActiveCommandTable].pCommandTable[item].aFunction();
                printf(": %c\r\n", pCBin->get() );  //clean the command character
            } else {
                //SerialUSB.println( F(" <- a separator") );
                printf(" <- a separator\r\n");
            }
        }
        ActiveCommand = 0;
    }
}
