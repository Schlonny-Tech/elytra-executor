#pragma once
#include "SDK.h"
void InitNetHooks();
void ShutdownNetHooks();


void RenderNetMonWindow();


void NetCmd_Block(uint16_t hash);
void NetCmd_Unblock(uint16_t hash);

extern bool g_ShowNetMon;