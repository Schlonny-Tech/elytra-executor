#include "LuaEngine.h"
#include "Globals.h"
#include "SDK.h"
#include <algorithm>

// ==================== FORWARD DECLARATIONS ====================
void ReplaceAll(std::string &str, const std::string &from,
                const std::string &to) {
  if (from.empty())
    return;
  size_t pos = 0;
  while ((pos = str.find(from, pos)) != std::string::npos) {
    str.replace(pos, from.size(), to);
    pos += to.size();
  }
}

// ==================== Globals ====================
ScriptTab g_ScriptTabs[16];
int g_ScriptTabCount = 0;
int g_CurrentScriptTab = 0;

bool g_Lua2LuauEnabled = false;
bool g_AutoCleanup = true;
void *g_CurrentScript = nullptr;
void *g_CreatedScripts[128];
int g_CreatedScriptCount = 0;

// ==================== TAB MANAGEMENT ====================

int AddScriptTab(const char *title, const char *content) {
  if (g_ScriptTabCount >= 16)
    return -1;

  auto &tab = g_ScriptTabs[g_ScriptTabCount];
  memset(&tab, 0, sizeof(ScriptTab));
  strncpy_s(tab.title, title, 63);

  tab.editor = new TextEditor();

  tab.editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

  tab.editor->SetText(content ? content : "");

  return g_ScriptTabCount++;
}

void RemoveScriptTab(int idx) {
  if (idx < 0 || idx >= g_ScriptTabCount || g_ScriptTabCount <= 1)
    return;

  if (g_ScriptTabs[idx].editor) {
    delete g_ScriptTabs[idx].editor;
    g_ScriptTabs[idx].editor = nullptr;
  }

  for (int i = idx; i < g_ScriptTabCount - 1; i++) {
    g_ScriptTabs[i] = g_ScriptTabs[i + 1];
  }

  memset(&g_ScriptTabs[g_ScriptTabCount - 1], 0, sizeof(ScriptTab));

  g_ScriptTabCount--;
  if (g_CurrentScriptTab >= g_ScriptTabCount)
    g_CurrentScriptTab = g_ScriptTabCount - 1;
  else if (g_CurrentScriptTab > idx)
    g_CurrentScriptTab--;
}

std::string GetTabText(int idx) {
  if (idx < 0 || idx >= g_ScriptTabCount || !g_ScriptTabs[idx].editor)
    return "";
  return g_ScriptTabs[idx].editor->GetText();
}

void SaveTabToFile(int idx) {
  auto &tab = g_ScriptTabs[idx];
  if (!tab.editor)
    return;

  char path[MAX_PATH];
  snprintf(path, MAX_PATH, "%s\\tab_%d.lua", g_TabsDir, idx);

  FILE *f = nullptr;
  fopen_s(&f, path, "w");
  if (!f)
    return;

  fprintf(f, "-- TAB_TITLE: %s\n", tab.title);

  std::string text = tab.editor->GetText();
  fwrite(text.c_str(), 1, text.size(), f);

  fclose(f);
}

void SaveAllTabs() {
  // Clean old
  char searchPath[MAX_PATH];
  snprintf(searchPath, MAX_PATH, "%s\\tab_*.lua", g_TabsDir);
  WIN32_FIND_DATAA fd;
  HANDLE hFind = FindFirstFileA(searchPath, &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      char fullpath[MAX_PATH];
      snprintf(fullpath, MAX_PATH, "%s\\%s", g_TabsDir, fd.cFileName);
      DeleteFileA(fullpath);
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
  }

  for (int i = 0; i < g_ScriptTabCount; i++)
    SaveTabToFile(i);

  // Save meta
  char metaPath[MAX_PATH];
  snprintf(metaPath, MAX_PATH, "%s\\meta.txt", g_TabsDir);
  FILE *f = nullptr;
  fopen_s(&f, metaPath, "w");
  if (f) {
    fprintf(f, "%d\n%d\n", g_ScriptTabCount, g_CurrentScriptTab);
    fclose(f);
  }
}

void LoadAllTabs() {
  char metaPath[MAX_PATH];
  snprintf(metaPath, MAX_PATH, "%s\\meta.txt", g_TabsDir);
  FILE *f = nullptr;
  fopen_s(&f, metaPath, "r");

  int tabCount = 0, currentTab = 0;
  if (f) {
    fscanf_s(f, "%d\n%d\n", &tabCount, &currentTab);
    fclose(f);
  }

  if (tabCount <= 0 || tabCount > 16) {
    AddScriptTab("Script 1", "print('Hello World!')");
    return;
  }

  for (int i = 0; i < tabCount; i++) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s\\tab_%d.lua", g_TabsDir, i);
    std::string content = ReadFileContents(path);

    char title[64];
    snprintf(title, sizeof(title), "Script %d", i + 1);

    // Parse title
    if (content.size() > 15) {
      const char *prefix = "-- TAB_TITLE: ";
      if (content.compare(0, strlen(prefix), prefix) == 0) {
        size_t end = content.find('\n');
        if (end != std::string::npos) {
          std::string t = content.substr(strlen(prefix), end - strlen(prefix));
          if (!t.empty() && t.back() == '\r')
            t.pop_back();
          strncpy_s(title, t.c_str(), 63);
          content = content.substr(end + 1);
        }
      }
    }
    AddScriptTab(title, content.c_str());
  }

  if (currentTab >= 0 && currentTab < g_ScriptTabCount)
    g_CurrentScriptTab = currentTab;
}

void SwitchScriptTab(int newIdx) {
  if (newIdx < 0 || newIdx >= g_ScriptTabCount)
    return;
  g_CurrentScriptTab = newIdx;
}

// ==================== EXECUTION ====================

void CleanupPreviousScript() {
  if (!g_AutoCleanup)
    return;
  if (g_CurrentScript) {
    try {
      Instance_Destroy(g_CurrentScript, 0);
    } catch (...) {
    }
    g_CurrentScript = nullptr;
  }
}

void TrackScript(void *script) {
  if (g_CreatedScriptCount < 128)
    g_CreatedScripts[g_CreatedScriptCount++] = script;
}

void ExecuteScriptFromString(const std::string &source) {
  if (source.empty()) {
    CmdLog("[-] Empty!");
    return;
  }

  if (!g_ScriptService)
    g_ScriptService = SafeGetScriptService();
  if (!g_ScriptService && !g_GameSingleton)
    g_GameSingleton = SafeFindGame(g_Domain);
  void *parent = g_ScriptService ? g_ScriptService : g_GameSingleton;
  if (!parent) {
    CmdLog("[-] No execution target!");
    return;
  }

  CleanupPreviousScript();

  void *script = nullptr;
  CmdLog("[*] Executing (%d chars)...", (int)source.size());
  try {
    script = Instance_New(il2cpp_string_new("ScriptInstance"), parent);
  } catch (...) {
    CmdLog("[-] Instance_New crashed!");
    return;
  }

  if (!script) {
    CmdLog("[-] Failed to create script!");
    return;
  }

  g_CurrentScript = script;
  TrackScript(script);
  CmdLog("[+] Script @ 0x%p", script);

  try {
    BaseScript_set_Source(script, il2cpp_string_new(source.c_str()));
    CmdLog("[+] Executed!");
  } catch (...) {
    CmdLog("[-] set_Source crashed!");
  }
}

void ExecuteScript(const std::string &source) {
  std::string code = source;
  if (g_Lua2LuauEnabled) {

    ReplaceAll(code, "Instance.new(", "Instance.New(");
    ReplaceAll(code, "game.Workspace", "game[\"Environment\"]");
  }
  ExecuteScriptFromString(code);
}

// ==================== INIT ====================
void InitLuaEngine() { InitDirectories(); }
void CleanupLuaEngine() { SaveAllTabs(); }

void InitMoonSharp() {}
void ShutdownMoonSharp() {}
void InjectNativeGlobals() {}
MsDynValue DoStringDirect(const char *, const char *) { return nullptr; }