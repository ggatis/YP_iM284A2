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
  {  27, cDescription0Q, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnExit] },
  { '-', cDescription0A, nullptr },
  { '1', cDescription01, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnPingDevice] },
  { '2', cDescription02, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetDeviceInformation] },
  { '3', cDescription03, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetFirmwareVersion] },
  { '4', cDescription04, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetDateTime] },
  { '5', cDescription05, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnSetDateTime] },
  { '6', cDescription06, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnRestartDevice] },
  { '7', cDescription07, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetSystemOptions] },
  { '8', cDescription08, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnSetSystemOptions] },
  { '-', cDescription0B, nullptr },
  { 'a', cDescription0a, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetNetworkAddress] },
  { 'b', cDescription0b, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnSetNetworkAddress_A] },
  { 'c', cDescription0c, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnSetNetworkAddress_B] },
  { 'd', cDescription0d, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetMode] },
  { 'e', cDescription0e, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnDisableRouter] },
  { 'f', cDescription0f, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnEnableRouter] },
  { 'g', cDescription0g, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnEnableCoordinator] },
  { 'h', cDescription0h, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetLinkStatus] },
  { 'i', cDescription0i, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnGetRoutingInfo] },
  { 'j', cDescription0j, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnSendPacketToNode_A] },
  { 'k', cDescription0k, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnSendPacketToNode_B] },
  { '-', cDescription0C, nullptr },
  { 'p', cDescription0p, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnPrintDemoApp] },
  { 't', cDescription0t, LoRa_Mesh_DemoApp::JumpTable[cJumpTo_OnTestRadioSerialMonitor] }
};

const uint8_t cntCommands_L0 = sizeof( Commands_L0 ) / sizeof( Commands_L0[0] );
