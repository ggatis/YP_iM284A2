/**
 * @file    iM284A_L0.c
 *
 * @brief   iM284A LVL0 API test commands
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#include <stdint.h>
//#include <Arduino.h>

#include "iM284A.h"
#include "iM284A_L0.h"

#include "LoRa_Mesh_DemoApp.h"


const char cDescription00[] = "print usage";
const char cDescription0Q[] = "quit";
const char cDescription0A[] = "Device Management Services";
const char cDescription01[] = "ping device";
const char cDescription02[] = "get Device Information";
const char cDescription03[] = "get Firmware Version";
const char cDescription04[] = "get Date and Time";
const char cDescription05[] = "set Date and Time";
const char cDescription06[] = "restart Target Device";
const char cDescription07[] = "get System Options";
const char cDescription08[] = "set System Options";
const char cDescription0B[] = "LoRa Mesh Router";
const char cDescription0a[] = "get Network Address";
const char cDescription0b[] = "set Network Address(A)";
const char cDescription0c[] = "set Network Address(B)";
const char cDescription0d[] = "get Mode";
const char cDescription0e[] = "disable Router";
const char cDescription0f[] = "enable Router";
const char cDescription0g[] = "enable Coordinator";
const char cDescription0h[] = "get Link Status";
const char cDescription0i[] = "get Routing Info";
const char cDescription0j[] = "send Packet to Node(A)";
const char cDescription0k[] = "send Packet to Node(B)";
const char cDescription0C[] = "Misc";
const char cDescription0p[] = "print demo setup";
const char cDescription0t[] = "test radio serial monitor";

const Command_t Commands_L0[] = {
  { ' ', cDescription00, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnShowMenu] },
  {  27, cDescription0Q, &quitDemo },
  { '-', cDescription0A, nullptr },
  { '1', cDescription01, &PingDevice },
  { '2', cDescription02, &GetDeviceInformation },
  { '3', cDescription03, &GetFirmwareVersion },
  { '4', cDescription04, &GetDateTime },
  { '5', cDescription05, &SetDateTime },
  { '6', cDescription06, &RestartDevice },
  { '7', cDescription07, &GetSystemOptions },
  { '8', cDescription08, &SetSystemOptions },
  { '-', cDescription0B, nullptr },
  { 'a', cDescription0a, &GetNetworkAddress },
  { 'b', cDescription0b, &SetNetworkAddress_A },
  { 'c', cDescription0c, &SetNetworkAddress_B },
  { 'd', cDescription0d, &GetMode },
  { 'e', cDescription0e, &DisableRouter },
  { 'f', cDescription0f, &EnableRouter },
  { 'g', cDescription0g, &EnableCoordinator },
  { 'h', cDescription0h, &GetLinkStatus },
  { 'i', cDescription0i, &GetRoutingInfo },
  { 'j', cDescription0j, &SendPacketToNode_A },
  { 'k', cDescription0k, &SendPacketToNode_B },
  { '-', cDescription0C, nullptr },
  { 'p', cDescription0p, &printDemo },
  { 't', cDescription0t, &testRadioSerialMonitor }
};

const uint8_t cntCommands_L0 = sizeof( Commands_L0 ) / sizeof( Commands_L0[0] );


/*************************************************/

/*** Commands related to this application ***/

void printUsage( void ) {
    for ( 
        uint8_t i = 0;
        i < CommandTables[ActiveCommandTable].CommandCount;
        showItem( CommandTables[ActiveCommandTable].pCommandTable, i++ )
    );
    pDemoApp->JumpTable[cJumpTo_OnShowMenu]();
}

void quitDemo( void ) {
    printf("Quit?\r\n");
    pDemoApp->JumpTable[cJumpTo_OnExit]();
}

/*** Device Management Services ***/

void PingDevice( void ) {
    printf("PingDevice\r\n");
    pDemoApp->JumpTable[cJumpTo_OnPingDevice]();
}

void GetDeviceInformation( void ) {
    printf("GetDeviceInformation\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetDeviceInformation]();
}

void GetFirmwareVersion( void ) {
    printf("GetFirmwareVersion\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetFirmwareVersion]();
}

void GetDateTime( void ) {
    printf("GetDateTime\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetDateTime]();
}

void SetDateTime( void ) {
    printf("SetDateTime\r\n");
    pDemoApp->JumpTable[cJumpTo_OnSetDateTime]();
}

void RestartDevice( void ) {
    printf("RestartDevice\r\n");
    pDemoApp->JumpTable[cJumpTo_OnRestartDevice]();
}

void GetSystemOptions( void ) {
    printf("GetSystemOptions\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetSystemOptions]();
}

void SetSystemOptions( void ) {
    printf("SetSystemOptions\r\n");
    pDemoApp->JumpTable[cJumpTo_OnSetSystemOptions]();
}

/*** LoRa Mesh Router ***/

void GetNetworkAddress( void ) {
    printf("GetNetworkAddress\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetNetworkAddress]();
}

void SetNetworkAddress_A( void ) {
    printf("SetNetworkAddress_A\r\n");
    pDemoApp->JumpTable[cJumpTo_OnSetNetworkAddress_A]();
}

void SetNetworkAddress_B( void ) {
    printf("SetNetworkAddress_B\r\n");
    pDemoApp->JumpTable[cJumpTo_OnSetNetworkAddress_B]();
}

void GetMode( void ) {
    printf("GetMode\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetMode]();
}

void DisableRouter( void ) {
    printf("DisableRouter\r\n");
    pDemoApp->JumpTable[cJumpTo_OnDisableRouter]();
}

void EnableRouter( void ) {
    printf("EnableRouter\r\n");
    pDemoApp->JumpTable[cJumpTo_OnEnableRouter]();
}

void EnableCoordinator( void ) {
    printf("EnableCoordinator\r\n");
    pDemoApp->JumpTable[cJumpTo_OnEnableCoordinator]();
}

void GetLinkStatus( void ) {
    printf("GetLinkStatus\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetLinkStatus]();
}

void GetRoutingInfo( void ) {
    printf("GetRoutingInfo\r\n");
    pDemoApp->JumpTable[cJumpTo_OnGetRoutingInfo]();
}

void SendPacketToNode_A( void ) {
    printf("SendPacketToNode_A\r\n");
    pDemoApp->JumpTable[cJumpTo_OnSendPacketToNode_A]();
}

void SendPacketToNode_B( void ) {
    printf("SendPacketToNode_B\r\n");
    pDemoApp->JumpTable[cJumpTo_OnSendPacketToNode_B]();
}

/*** Rest ***/

void printDemo( void ) {
    printf("printDemo\r\n");
    pDemoApp->JumpTable[cJumpTo_OnPrintDemoApp]();
}

void testRadioSerialMonitor( void ) {
    printf("testRadioSerialMonitor\r\n");
    pDemoApp->JumpTable[cJumpTo_OnTestRadioSerialMonitor]();
}
