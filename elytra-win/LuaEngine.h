#pragma once
#include "SDK.h"
#include "TextEditor.h"
#include <string>

// ==================== Script Tabs ====================
struct ScriptTab {
    char title[64];
    TextEditor* editor;
    //char* buffer;
    int bufferSize;
    bool fromFile;
    char filePath[MAX_PATH];
};

extern ScriptTab g_ScriptTabs[16];
extern int g_ScriptTabCount;
extern int g_CurrentScriptTab;

// ==================== Configuration ====================
extern bool g_Lua2LuauEnabled;
extern bool g_AutoCleanup;

// ==================== Functions ====================
void InitLuaEngine();
void CleanupLuaEngine();

// Tab Management
int AddScriptTab(const char* title, const char* content);
void RemoveScriptTab(int idx);
void SwitchScriptTab(int newIdx);
void SaveCurrentTabContent(const char* newContent);
void SaveAllTabs();
void LoadAllTabs();
std::string GetTabText(int idx);
// Execution
void ExecuteScript(const std::string& source);
void ExecuteScriptFromString(const std::string& source);

std::string TranslateLuau(const std::string& source);
void CleanupScripts();

// ==================== MoonSharp Integration ====================
void InitMoonSharp();
void ShutdownMoonSharp();
MsDynValue DoStringDirect(const char* code, const char* chunk = "=elytra");