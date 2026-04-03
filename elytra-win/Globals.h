#pragma once
#include <string>
#include <vector>
#include <Windows.h>

// ==================== Converted stuff ====================
void CmdLog(const char* fmt, ...);

std::string ReadFileContents(const std::string& path);

bool HttpGet(const char* url, char* outBuf, int maxLen);

void InitDirectories();

extern char g_ElytraDir[MAX_PATH];
extern char g_ScriptsDir[MAX_PATH];
extern char g_TabsDir[MAX_PATH];

extern std::vector<std::string> g_ConsoleLogs;

// ==================== Explorer Data ====================
struct ExplorerNode {
    void* instance = nullptr;
    std::string name;
    std::string className;
    std::vector<ExplorerNode> children;
};

extern ExplorerNode g_RootNode; 
extern void* g_SelectedInstance;
extern std::vector<std::string> g_PropertiesList;

// ==================== Explorer funcs ====================
void RefreshExplorer();
void DeleteSelectedInstance();
void CloneSelectedInstance();

// ==================== CMD ====================
void ProcessCommand(const char* cmd);
// ==================== Property list ====================
void DumpSelectedScriptSource();
void OpenLiveScriptInEditor(void* instance);

extern void* g_LiveEditorTarget;
extern char g_LiveEditorBuffer[512000];
extern bool g_ShowLiveEditor;

// ==================== Execution ====================
bool TrySetScriptSource(void* instance, const char* code);
bool TryRunScript(void* instance);
