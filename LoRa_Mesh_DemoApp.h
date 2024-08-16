/**
 * @file    LoRa_Mesh_DemoApp.h
 *
 * @brief   Declaration of LoRa_Mesh_DemoApp structure
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef _LoRa_Mesh_DemoApp_H_
#define _LoRa_Mesh_DemoApp_H_

#include "YP_iM284A2.h"

#define HMI_TIMEOUT 500

void setup_DemoApp( void );

enum JumpEnum {
    //<! commands related to this application
    cJumpTo_OnShowMenu,
    cJumpTo_OnExit,

    //<! commands for DeviceManagement messages
    cJumpTo_OnPingDevice,

    cJumpTo_OnGetDeviceInformation,
    cJumpTo_OnGetFirmwareVersion,

    cJumpTo_OnGetDateTime,
    cJumpTo_OnSetDateTime,

    cJumpTo_OnRestartDevice,

    cJumpTo_OnGetSystemOptions,
    cJumpTo_OnSetSystemOptions,

    //<! commands for LoRaMesh Router messages
    cJumpTo_OnGetNetworkAddress,
    cJumpTo_OnSetNetworkAddress_A,
    cJumpTo_OnSetNetworkAddress_B,

    cJumpTo_OnGetMode,
    cJumpTo_OnDisableRouter,
    cJumpTo_OnEnableRouter,
    cJumpTo_OnEnableCoordinator,

    cJumpTo_OnGetLinkStatus,
    cJumpTo_OnGetRoutingInfo,

    cJumpTo_OnSendPacketToNode_A,
    cJumpTo_OnSendPacketToNode_B,

    cJumpTo_OnPrintDemoApp,
    cJumpTo_OnTestRadioSerialMonitor
};


void OnShowMenu( void );
void OnExit( void );

//commands for DeviceManagement messages
void OnPingDevice( void );

void OnGetDeviceInformation( void );
void OnGetFirmwareVersion( void );

void OnGetDateTime( void );
void OnSetDateTime( void );

void OnRestartDevice( void );

void OnGetSystemOptions( void );
void OnSetSystemOptions( void );

//commands for LoRaMesh Router messages
void OnGetNetworkAddress( void );
void OnSetNetworkAddress_A( void );
void OnSetNetworkAddress_B( void );

void OnGetMode( void );
void OnDisableRouter( void );
void OnEnableRouter( void );
void OnEnableCoordinator( void );

void OnGetLinkStatus( void );
void OnGetRoutingInfo( void );

void OnSendPacketToNode_A( void );
void OnSendPacketToNode_B( void );

void OnPrintDemoApp( void );
void OnTestRadioSerialMonitor( void );


struct LoRa_Mesh_DemoApp {
    //application parameters
    const char*     _Network_ID;
    const char*     _DeviceEUI_Node_A;
    const char*     _DeviceEUI_Node_B;
    int             _User_Port;
    const char*     _Payload_for_Node_A;
    const char*     _Payload_for_Node_B;

    static void   ( *JumpTable[cJumpTo_OnTestRadioSerialMonitor+1] )( void );

};


extern LoRa_Mesh_DemoApp* pDemoApp;


//processor of incomig data from HMI, SerialUSB
StatusCode OnHMI_DataEvent( ByteArray* in, ByteArray* out );

#endif // _LoRa_Mesh_DemoApp_H_
