#include "NetHooks.h"
#include "Globals.h"
#include "include.h"
#include <unordered_map>
#include <vector>

// ==================== STATE ====================
struct RpcLogEntry {
  DWORD timestamp;
  uint16_t hash;
  uint32_t netId;
  bool blocked;
  bool outgoing; // false=Incoming RPC, true=Outgoing Cmd
  char name[64];
};
std::vector<RpcLogEntry> g_RpcLog;
bool g_Paused = false;

struct BlockedInfo {
  uint16_t hash;
  bool enabled;
};
std::vector<BlockedInfo> g_BlockedList;

void *g_HandleRemoteCallAddr = nullptr;
BYTE g_HandleRemoteCallOrig[14];
void *g_SendCmdInternalAddr = nullptr;
BYTE g_SendCmdInternalOrig[14];

bool g_ShowNetMon = false;

BYTE g_HookStub[14] = {0xFF, 0x25, 0, 0, 0, 0};

// ==================== HELPERS ====================
const char *GetName(uint16_t hash) {
  if (!GetFunctionMethodName)
    return "?";
  static char buf[128];
  Il2CppString *str = nullptr;
  try {
    if (GetFunctionMethodName(hash, &str) && str) {
      SafeReadString(str, buf, 128);
      return buf;
    }
  } catch (...) {
  }
  return "?";
}

bool IsBlocked(uint16_t hash) {
  for (auto &b : g_BlockedList)
    if (b.hash == hash && b.enabled)
      return true;
  return false;
}

void AddLog(bool outgoing, uint16_t hash, uint32_t netId, bool blocked) {
  if (g_Paused)
    return;
  RpcLogEntry e;
  e.timestamp = GetTickCount();
  e.hash = hash;
  e.netId = netId;
  e.blocked = blocked;
  e.outgoing = outgoing;
  strncpy_s(e.name, GetName(hash), 63);

  g_RpcLog.push_back(e);
  if (g_RpcLog.size() > 1000)
    g_RpcLog.erase(g_RpcLog.begin());
}

// ==================== HOOKS ====================

void __fastcall Hook_HandleRemoteCall(void *self, uint8_t compIdx,
                                      uint16_t hash, int callType, void *reader,
                                      void *conn) {
  bool blocked = IsBlocked(hash);

  // callType 0=Cmd, 1=ClientRpc, 2=TargetRpc
  if (callType > 0) {
    uint32_t netId = 0;
    try {
      netId = NB_get_netId(self);
    } catch (...) {
    }
    AddLog(false, hash, netId, blocked);
  }

  if (blocked)
    return;

  DWORD oldProt;
  VirtualProtect(g_HandleRemoteCallAddr, 14, PAGE_EXECUTE_READWRITE, &oldProt);
  memcpy(g_HandleRemoteCallAddr, g_HandleRemoteCallOrig, 14);
  try {
    ((HandleRemoteCall_t)g_HandleRemoteCallAddr)(self, compIdx, hash, callType,
                                                 reader, conn);
  } catch (...) {
  }
  memcpy(g_HandleRemoteCallAddr, g_HookStub, 14);
  VirtualProtect(g_HandleRemoteCallAddr, 14, oldProt, &oldProt);
}

void __fastcall Hook_SendCommandInternal(void *self, Il2CppString *funcName,
                                         int hash, void *writer, int channel,
                                         bool reqAuth) {
  bool blocked = IsBlocked((uint16_t)hash);
  uint32_t netId = 0;
  try {
    netId = NB_get_netId(self);
  } catch (...) {
  }

  AddLog(true, (uint16_t)hash, netId, blocked);

  if (blocked)
    return;

  DWORD oldProt;
  VirtualProtect(g_SendCmdInternalAddr, 14, PAGE_EXECUTE_READWRITE, &oldProt);
  memcpy(g_SendCmdInternalAddr, g_SendCmdInternalOrig, 14);
  try {
    ((SendCommandInternal_t)g_SendCmdInternalAddr)(self, funcName, hash, writer,
                                                   channel, reqAuth);
  } catch (...) {
  }
  memcpy(g_SendCmdInternalAddr, g_HookStub, 14);
  VirtualProtect(g_SendCmdInternalAddr, 14, oldProt, &oldProt);
}

// ==================== INSTALL ====================
void InstallHook(void *target, void *hook, BYTE *orig) {
  DWORD old;
  VirtualProtect(target, 14, PAGE_EXECUTE_READWRITE, &old);
  memcpy(orig, target, 14);
  *(void **)(g_HookStub + 6) = hook;
  memcpy(target, g_HookStub, 14);
  VirtualProtect(target, 14, old, &old);
}

void InitNetHooks() {
  if (!g_GameAssembly)
    return;
  uintptr_t b = (uintptr_t)g_GameAssembly;

  g_HandleRemoteCallAddr = (void *)(b + HANDLE_REMOTE_CALL_OFFSET);
  InstallHook(g_HandleRemoteCallAddr, Hook_HandleRemoteCall,
              g_HandleRemoteCallOrig);

  g_SendCmdInternalAddr = (void *)(b + SEND_CMD_INTERNAL_OFFSET);
  InstallHook(g_SendCmdInternalAddr, Hook_SendCommandInternal,
              g_SendCmdInternalOrig);

  CmdLog("[+] NetHooks Installed (Mirror)");
}

void ShutdownNetHooks() {}

// ==================== COMMANDS ====================
void NetCmd_Block(uint16_t hash) {
  for (auto &b : g_BlockedList)
    if (b.hash == hash) {
      b.enabled = true;
      return;
    }
  g_BlockedList.push_back({hash, true});
  CmdLog("[+] Blocked 0x%04X", hash);
}

void NetCmd_Unblock(uint16_t hash) {
  for (auto &b : g_BlockedList)
    if (b.hash == hash) {
      b.enabled = false;
    }
  CmdLog("[+] Unblocked 0x%04X", hash);
}

// ==================== UI WINDOW ====================
void RenderNetMonWindow() {
  if (!g_ShowNetMon)
    return;

  ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Network Monitor", &g_ShowNetMon)) {

    // Toolbar
    if (ImGui::Button("Clear"))
      g_RpcLog.clear();
    ImGui::SameLine();
    if (ImGui::Button(g_Paused ? "Resume" : "Pause"))
      g_Paused = !g_Paused;
    ImGui::SameLine();
    ImGui::Text("Logs: %d", (int)g_RpcLog.size());

    // Table
    ImGui::BeginChild("LogTable");
    if (ImGui::BeginTable("LogTbl", 6,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
      ImGui::TableSetupColumn("Time");
      ImGui::TableSetupColumn("Dir");
      ImGui::TableSetupColumn("Hash");
      ImGui::TableSetupColumn("Name");
      ImGui::TableSetupColumn("NetID");
      ImGui::TableSetupColumn("Status");
      ImGui::TableHeadersRow();

      for (auto &e : g_RpcLog) {
        ImGui::TableNextRow();

        // Time
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%u", (unsigned int)(e.timestamp % 100000));

        // Dir
        ImGui::TableSetColumnIndex(1);
        ImGui::Text(e.outgoing ? "OUT" : "IN");

        // Hash
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("0x%04X", e.hash);

        // Name
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("%s", e.name);

        // NetID
        ImGui::TableSetColumnIndex(4);
        ImGui::Text("%u", (unsigned int)e.netId);

        // Status
        ImGui::TableSetColumnIndex(5);
        if (e.blocked)
          ImGui::TextColored(ImVec4(1, 0, 0, 1), "BLOCKED");
        else
          ImGui::TextColored(ImVec4(0, 1, 0, 1), "PASS");

        // Context Menu
        if (ImGui::BeginPopupContextItem("Ctx")) {
          if (ImGui::Button("Block")) {
            NetCmd_Block(e.hash);
            ImGui::CloseCurrentPopup();
          }
          if (ImGui::Button("Unblock")) {
            NetCmd_Unblock(e.hash);
            ImGui::CloseCurrentPopup();
          }
          ImGui::EndPopup();
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
          NetCmd_Block(e.hash);
        }
      }
      ImGui::EndTable();
    }
    ImGui::EndChild();
  }
  ImGui::End();
}