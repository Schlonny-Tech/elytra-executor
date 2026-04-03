#include "Globals.h"
#include "LuaEngine.h"
#include "NetHooks.h"
#include "SDK.h"
#include <commdlg.h>
#include <cstdarg>
#include <fstream>
#include <sstream>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

// ==================== VARS ====================
char g_ElytraDir[MAX_PATH] = {};
char g_ScriptsDir[MAX_PATH] = {};
char g_TabsDir[MAX_PATH] = {};
std::vector<std::string> g_ConsoleLogs;

ExplorerNode g_RootNode;
void *g_SelectedInstance = nullptr;
std::vector<std::string> g_PropertiesList;

void *g_LiveEditorTarget = nullptr;
char g_LiveEditorBuffer[512000] = {};
bool g_ShowLiveEditor = false;

// ==================== LOG & IO ====================
void CmdLog(const char *fmt, ...) {
  char buf[2048];
  va_list a;
  va_start(a, fmt);
  vsnprintf(buf, sizeof(buf), fmt, a);
  va_end(a);
  g_ConsoleLogs.push_back(std::string(buf));
  if (g_ConsoleLogs.size() > 500)
    g_ConsoleLogs.erase(g_ConsoleLogs.begin());
  OutputDebugStringA(buf);
}

std::string ReadFileContents(const std::string &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open())
    return "";
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  if (size <= 0)
    return "";
  std::string contents((size_t)size, '\0');
  file.read(&contents[0], size);
  return contents;
}

bool HttpGet(const char *url, char *outBuf, int maxLen) {
  HINTERNET hInternet = InternetOpenA(
      "Elytra/1.5", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
  if (!hInternet)
    return false;
  HINTERNET hUrl =
      InternetOpenUrlA(hInternet, url, nullptr, 0,
                       INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
  if (!hUrl) {
    InternetCloseHandle(hInternet);
    return false;
  }
  int totalRead = 0;
  DWORD bytesRead = 0;
  while (totalRead < maxLen - 1) {
    if (!InternetReadFile(hUrl, outBuf + totalRead, maxLen - totalRead - 1,
                          &bytesRead))
      break;
    if (bytesRead == 0)
      break;
    totalRead += bytesRead;
  }
  outBuf[totalRead] = 0;
  InternetCloseHandle(hUrl);
  InternetCloseHandle(hInternet);
  return totalRead > 0;
}

void InitDirectories() {
  char appdata[MAX_PATH] = {};
  if (!GetEnvironmentVariableA("APPDATA", appdata, MAX_PATH))
    strcpy_s(appdata, "C:\\");
  snprintf(g_ElytraDir, MAX_PATH, "%s\\Elytra", appdata);
  snprintf(g_ScriptsDir, MAX_PATH, "%s\\scripts", g_ElytraDir);
  snprintf(g_TabsDir, MAX_PATH, "%s\\tabs", g_ElytraDir);
  CreateDirectoryA(g_ElytraDir, nullptr);
  CreateDirectoryA(g_ScriptsDir, nullptr);
  CreateDirectoryA(g_TabsDir, nullptr);
}

// ==================== FLAT FOR SEH ====================

bool Internal_ReadScriptSource(void *inst, char *outBuf, int maxLen) {
  if (!inst || !outBuf)
    return false;
  try {
    Il2CppString *srcStr =
        *(Il2CppString **)((uintptr_t)inst + BASESCRIPT_SOURCE_FIELD_OFFSET);
    if (!srcStr)
      return false;
    int len = *(int *)((uintptr_t)srcStr + STRING_LENGTH_OFFSET);
    if (len <= 0 || len >= maxLen)
      return false;
    wchar_t *chars = (wchar_t *)((uintptr_t)srcStr + STRING_CHARS_OFFSET);
    for (int i = 0; i < len; i++)
      outBuf[i] = (char)chars[i];
    outBuf[len] = 0;
    return true;
  } catch (...) {
    return false;
  }
}

void Internal_SetHealth(void *lp, float hp) {
  if (!lp || !Plr_CmdSetHealth)
    return;
  try {
    Plr_CmdSetHealth(lp, hp);
  } catch (...) {
  }
}

// ==================== EXPLORER ====================

void BuildExplorerTree(ExplorerNode &node, void *inst, int depth) {
  if (!inst || depth > 20)
    return;

  node.instance = inst;
  char nameBuf[128] = {};
  char classBuf[64] = {};
  SafeGetName(inst, nameBuf, 128);
  SafeGetClassName(inst, classBuf, 64);
  node.name = nameBuf;
  node.className = classBuf;
  if (node.name.empty())
    node.name = "?";

  void *children[512];
  int cnt = SafeGetChildren(inst, children, 512);
  if (node.className == "Players" && !g_PlayersInstance)
    g_PlayersInstance = inst;

  for (int i = 0; i < cnt; i++) {
    ExplorerNode child;
    BuildExplorerTree(child, children[i], depth + 1);
    node.children.push_back(child);
  }
}

void RefreshExplorer() {
  g_RootNode.children.clear();
  g_SelectedInstance = nullptr;
  g_PropertiesList.clear();
  if (!g_GameSingleton)
    g_GameSingleton = SafeFindGame(g_Domain);
  if (!g_GameSingleton) {
    CmdLog("[-] Game not found");
    return;
  }
  BuildExplorerTree(g_RootNode, g_GameSingleton, 0);
  CmdLog("[+] Explorer Refreshed");
}

void DeleteSelectedInstance() {
  if (!g_SelectedInstance || !Instance_Destroy)
    return;
  try {
    Instance_Destroy(g_SelectedInstance, 0);
    CmdLog("[+] Deleted");
    g_SelectedInstance = nullptr;
    RefreshExplorer();
  } catch (...) {
    CmdLog("[-] Delete failed!");
  }
}

void CloneSelectedInstance() {
  if (!g_SelectedInstance || !Instance_Clone)
    return;
  try {
    void *c = Instance_Clone(g_SelectedInstance);
    if (c) {
      CmdLog("[+] Cloned");
      RefreshExplorer();
    }
  } catch (...) {
    CmdLog("[-] Clone failed!");
  }
}

// ==================== ACTIONS ====================

void DumpSelectedScriptSource() {
  if (!g_SelectedInstance) {
    CmdLog("[-] Nothing selected");
    return;
  }

  char cls[64] = {};
  SafeGetClassName(g_SelectedInstance, cls, 64);
  if (strcmp(cls, "ScriptInstance") != 0 && strcmp(cls, "LocalScript") != 0 &&
      strcmp(cls, "ModuleScript") != 0) {
    CmdLog("[-] Not a script");
    return;
  }

  char name[128] = {};
  SafeGetName(g_SelectedInstance, name, 128);

  static char scriptBuf[512000];
  if (!Internal_ReadScriptSource(g_SelectedInstance, scriptBuf,
                                 sizeof(scriptBuf))) {
    CmdLog("[-] Failed to read source");
    return;
  }

  char filepath[MAX_PATH] = "script.lua";
  OPENFILENAMEA ofn = {0};
  ofn.lStructSize = sizeof(ofn);
  ofn.lpstrFilter = "Lua Files (*.lua)\0*.lua\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = filepath;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
  ofn.lpstrTitle = "Save Script Source";

  if (GetSaveFileNameA(&ofn)) {
    FILE *f = nullptr;
    fopen_s(&f, filepath, "w");
    if (f) {
      fwrite(scriptBuf, 1, strlen(scriptBuf), f);
      fclose(f);
      CmdLog("[+] Saved '%s' source to %s", name, filepath);
    } else
      CmdLog("[-] Failed to write file");
  }
}

void OpenLiveScriptInEditor(void *instance) {
  if (!instance)
    return;
  char cls[64] = {};
  SafeGetClassName(instance, cls, 64);
  if (strcmp(cls, "ScriptInstance") != 0 && strcmp(cls, "LocalScript") != 0) {
    CmdLog("[-] Not a script");
    return;
  }

  if (!Internal_ReadScriptSource(instance, g_LiveEditorBuffer,
                                 sizeof(g_LiveEditorBuffer))) {
    CmdLog("[-] Failed to read source");
    return;
  }

  g_LiveEditorTarget = instance;
  g_ShowLiveEditor = true;

  char name[128] = {};
  SafeGetName(instance, name, 128);
  CmdLog("[*] Editing '%s'...", name);
}

// ==================== COMMANDS ====================

void ProcessCommand(const char *cmd) {
  CmdLog(">> %s", cmd);
  char command[64] = {}, arg1[256] = {};
  sscanf_s(cmd, "%63s %255[^\n]", command, (unsigned)sizeof(command), arg1,
           (unsigned)sizeof(arg1));

  if (strcmp(command, "sh") == 0) {
    if (!arg1[0]) {
      CmdLog("[!] sh <health>");
      return;
    }
    float hp = (float)atof(arg1);
    void *lp = SafeGetLocalPlayer();
    if (!lp) {
      CmdLog("[-] No LocalPlayer. Refresh Explorer.");
      return;
    }

    Internal_SetHealth(lp, hp);
    CmdLog("[+] Set health to %.0f", hp);
  } else if (strcmp(command, "exec") == 0) {
    if (!arg1[0]) {
      CmdLog("[!] exec <url>");
      return;
    }
    CmdLog("[*] Fetching %s...", arg1);
    static char httpBuf[512000];
    if (HttpGet(arg1, httpBuf, sizeof(httpBuf))) {
      CmdLog("[+] Downloaded %d bytes", (int)strlen(httpBuf));
      ExecuteScriptFromString(httpBuf);
    } else
      CmdLog("[-] HTTP Failed");
  }
  // ===== PC COMMANDS =====
  else if (strcmp(command, "pc") == 0) {
    if (strcmp(arg1, "on") == 0) {
      InitNetHooks();
      CmdLog("[+] Packet Canceller ON");
    } else if (strcmp(arg1, "off") == 0) {
      ShutdownNetHooks();
      CmdLog("[+] Packet Canceller OFF (Restart game to fully unload)");
    }
  } else if (strcmp(command, "block") == 0) {
    int hash = 0;
    if (sscanf_s(arg1, "%i", &hash) == 1)
      NetCmd_Block((uint16_t)hash);
  } else if (strcmp(command, "unblock") == 0) {
    int hash = 0;
    if (sscanf_s(arg1, "%i", &hash) == 1)
      NetCmd_Unblock((uint16_t)hash);
  } else if (strcmp(command, "l2s") == 0) {
    if (strcmp(arg1, "on") == 0) {
      g_Lua2LuauEnabled = true;
      CmdLog("[+] L2S ON");
    } else if (strcmp(arg1, "off") == 0) {
      g_Lua2LuauEnabled = false;
      CmdLog("[+] L2S OFF");
    } else
      CmdLog("[!] l2s on/off");
  } else if (strcmp(command, "cleanup") == 0) {
    CmdLog("[+] Cleanup command received");
  } else if (strcmp(command, "clear") == 0) {
    g_ConsoleLogs.clear();
  } else if (strcmp(command, "help") == 0) {
    CmdLog("Commands: sh, exec, l2s, cleanup, clear, block [0xID], unblock "
           "[0xID]");
  } else {
    CmdLog("[-] Unknown command: %s", command);
  }
}

// safe setsrc
bool TrySetScriptSource(void *instance, const char *code) {
  if (!instance || !code)
    return false;
  try {
    BaseScript_set_Source(instance, il2cpp_string_new(code));
    return true;
  } catch (...) {
    return false;
  }
}

// safe runscript
bool TryRunScript(void *instance) {
  if (!instance || !BaseScript_Run)
    return false;
  try {
    BaseScript_Run(instance);
    return true;
  } catch (...) {
    return false;
  }
}