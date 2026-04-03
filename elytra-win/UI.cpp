#include "UI.h"
#include "Globals.h"
#include "LuaEngine.h"
#include "NetHooks.h"
#include "SDK.h"
#include "include.h"
#include <algorithm>
#include <shellapi.h> // Add this for ShellExecuteA
#include <string>
#include <vector>

// do the style first
void InitImGuiStyle() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  style.WindowRounding = 6.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;
  style.ChildRounding = 4.0f;

  style.WindowPadding = ImVec2(8, 8);
  style.FramePadding = ImVec2(5, 2);
  style.ItemSpacing = ImVec2(6, 4);
  style.ItemInnerSpacing = ImVec2(6, 4);
  style.IndentSpacing = 20.0f;
  style.ScrollbarSize = 14.0f;

  // ==================== DARK GRAY THEME ====================

  // main colors
  colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.16f, 0.95f);
  colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.30f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.34f, 1.00f);

  colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.12f, 0.60f);

  colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);

  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.44f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);

  colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.60f, 0.64f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.80f, 0.84f, 1.00f);

  colors[ImGuiCol_Button] = ImVec4(0.24f, 0.24f, 0.28f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.36f, 0.36f, 0.40f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.46f, 0.50f, 1.00f);

  colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.26f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.32f, 0.32f, 0.36f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.38f, 0.42f, 1.00f);

  colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.60f, 0.60f, 0.64f, 1.00f);

  colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.39f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.26f, 0.30f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.20f, 0.97f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.22f, 0.22f, 0.26f, 1.00f);

  colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.45f, 0.45f, 0.49f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.60f, 0.60f, 0.64f, 1.00f);

  colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.80f, 0.80f, 0.84f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.80f, 0.80f, 0.84f, 1.00f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.54f, 0.90f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.74f, 0.70f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.40f, 0.44f, 0.35f);
}

// ==================== EXTERNALS ====================
extern std::vector<std::string> g_ConsoleLogs;
extern ExplorerNode g_RootNode;
extern void *g_SelectedInstance;
extern std::vector<std::string> g_PropertiesList;
extern char g_ScriptsDir[MAX_PATH];

extern ScriptTab g_ScriptTabs[16];
extern int g_ScriptTabCount;
extern int g_CurrentScriptTab;

extern void *g_LiveEditorTarget;
extern char g_LiveEditorBuffer[512000];
extern bool g_ShowLiveEditor;

// ==================== LOCAL STATE ====================
static bool g_AutoScroll = true;
static int g_SelectedFileIdx = -1;
static std::vector<std::string> g_FileList;

// ==================== FUNCS ====================
void RefreshFileList() {
  g_FileList.clear();
  char searchPath[MAX_PATH];
  snprintf(searchPath, MAX_PATH, "%s\\*.lua", g_ScriptsDir);

  WIN32_FIND_DATAA fd;
  HANDLE hFind = FindFirstFileA(searchPath, &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        g_FileList.push_back(fd.cFileName);
      }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
  }
}

void RenderExplorerNode(ExplorerNode &node) {
  ImGuiTreeNodeFlags flags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
  if (node.children.empty())
    flags |= ImGuiTreeNodeFlags_Leaf;
  if (g_SelectedInstance == node.instance)
    flags |= ImGuiTreeNodeFlags_Selected;

  char label[256];
  snprintf(label, sizeof(label), "%s [%s]", node.name.c_str(),
           node.className.c_str());

  if (ImGui::TreeNodeEx((void *)node.instance, flags, "%s", label)) {

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
      g_SelectedInstance = node.instance;
      g_PropertiesList.clear();
      char buf[256];

      sprintf_s(buf, "Address: 0x%p", node.instance);
      g_PropertiesList.push_back(buf);
      sprintf_s(buf, "Name: %s", node.name.c_str());
      g_PropertiesList.push_back(buf);
      sprintf_s(buf, "Class: %s", node.className.c_str());
      g_PropertiesList.push_back(buf);

      void *ch[512];
      int cnt = SafeGetChildren(node.instance, ch, 512);
      sprintf_s(buf, "Children: %d", cnt);
      g_PropertiesList.push_back(buf);

      if (node.className == "ScriptInstance" ||
          node.className == "LocalScript" || node.className == "ModuleScript") {
        g_PropertiesList.push_back("--- SCRIPT ---");
        g_PropertiesList.push_back("<DUMP SOURCE>");
        g_PropertiesList.push_back("<EDIT RUNTIME>");
      }
    }

    for (auto &child : node.children)
      RenderExplorerNode(child);
    ImGui::TreePop();
  }
}

void OnRenderCallback() {
  static bool init = false;
  if (!init) {
    InitImGuiStyle();
    RefreshFileList();
    init = true;
  }

  ImGui::SetNextWindowSize(ImVec2(1000, 600), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Elytra v1.5", nullptr, ImGuiWindowFlags_MenuBar)) {

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save All Tabs"))
          SaveAllTabs();
        if (ImGui::MenuItem("Refresh Files"))
          RefreshFileList();
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Tools")) {
        if (ImGui::MenuItem("Network Monitor"))
          g_ShowNetMon = true;
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (ImGui::BeginTabBar("MainTabs")) {

      // ==================== EXPLORER ====================
      if (ImGui::BeginTabItem("EXPLORER")) {
        if (ImGui::Button("Refresh"))
          RefreshExplorer();
        ImGui::SameLine();
        if (ImGui::Button("Delete"))
          DeleteSelectedInstance();
        ImGui::SameLine();
        if (ImGui::Button("Clone"))
          CloneSelectedInstance();

        ImGui::Separator();
        ImGui::Columns(2, "ExplorerColumns", true);

        ImGui::BeginChild("TreeRegion");
        if (g_RootNode.instance)
          RenderExplorerNode(g_RootNode);
        else
          ImGui::Text("Click [Refresh]");
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("PropertyRegion");
        ImGui::Text("Properties:");
        ImGui::Separator();

        if (g_SelectedInstance) {
          for (const auto &prop : g_PropertiesList) {
            if (prop[0] == '<') {
              if (ImGui::Button(prop.c_str())) {
                if (prop == "<DUMP SOURCE>")
                  DumpSelectedScriptSource();
                else if (prop == "<EDIT RUNTIME>")
                  OpenLiveScriptInEditor(g_SelectedInstance);
              }
            } else {
              ImGui::Text("%s", prop.c_str());
            }
          }
        } else
          ImGui::Text("Select an instance");
        ImGui::EndChild();

        ImGui::Columns(1);
        ImGui::EndTabItem();
      }

      // ==================== LUA ====================
      if (ImGui::BeginTabItem("LUA")) {
        ImGui::Columns(2, "LuaColumns", true);
        ImGui::SetColumnWidth(0, 200.0f);

        ImGui::BeginChild("FileListRegion");
        if (ImGui::Button("Refresh"))
          RefreshFileList();
        ImGui::SameLine();
        if (ImGui::Button("Folder"))
          ShellExecuteA(nullptr, "open", g_ScriptsDir, nullptr, nullptr,
                        SW_SHOW);
        ImGui::Separator();

        for (size_t i = 0; i < g_FileList.size();
             i++) { // Changed int to size_t
          if (ImGui::Selectable(g_FileList[i].c_str(),
                                g_SelectedFileIdx == (int)i))
            g_SelectedFileIdx = (int)i;
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            std::string path = std::string(g_ScriptsDir) + "\\" + g_FileList[i];
            std::string content = ReadFileContents(path);
            if (!content.empty()) {
              auto &tab = g_ScriptTabs[g_CurrentScriptTab];
              if (tab.editor)
                tab.editor->SetText(content);
              strncpy_s(tab.title, g_FileList[i].c_str(), 63);
              CmdLog("[+] Loaded: %s", g_FileList[i].c_str());
            }
          }
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("EditorRegion");

        // Tab Bar
        if (ImGui::BeginTabBar("ScriptTabs", ImGuiTabBarFlags_Reorderable)) {
          for (int i = 0; i < g_ScriptTabCount; i++) {
            bool open = true;

            char tabLabel[128];
            snprintf(tabLabel, sizeof(tabLabel), "%s##%d",
                     g_ScriptTabs[i].title, i);

            if (ImGui::BeginTabItem(tabLabel, &open)) {
              if (g_CurrentScriptTab != i)
                SwitchScriptTab(i);

              // Render Editor
              if (g_ScriptTabs[i].editor) {
                g_ScriptTabs[i].editor->Render(
                    "##Source",
                    ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - 35));
              }
              ImGui::EndTabItem();
            }
            if (!open) {
              RemoveScriptTab(i);
              i--;
            }
          }

          if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
            AddScriptTab("New Script", "");
          }

          ImGui::EndTabBar();
        }

        if (ImGui::Button("Execute", ImVec2(100, 30))) {
          std::string code = GetTabText(g_CurrentScriptTab);
          ExecuteScript(code);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(80, 30))) {
          if (g_ScriptTabs[g_CurrentScriptTab].editor)
            g_ScriptTabs[g_CurrentScriptTab].editor->SetText("");
        }
        ImGui::SameLine();
        ImGui::Text("Tabs: %d", g_ScriptTabCount);

        ImGui::EndChild();
        ImGui::Columns(1);
        ImGui::EndTabItem();
      }

      // ==================== CMD ====================
      if (ImGui::BeginTabItem("CMD")) {
        ImGui::BeginChild("ConsoleLog", ImVec2(0, -40), true,
                          ImGuiWindowFlags_HorizontalScrollbar);
        for (const auto &line : g_ConsoleLogs)
          ImGui::TextUnformatted(line.c_str());
        if (g_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10)
          ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();

        static char cmdBuf[512] = "";
        ImGui::PushItemWidth(-70);
        bool enter = ImGui::InputText("##Cmd", cmdBuf, IM_ARRAYSIZE(cmdBuf),
                                      ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("Send") || enter) {
          if (cmdBuf[0]) {
            ProcessCommand(cmdBuf);
            cmdBuf[0] = 0;
            ImGui::SetKeyboardFocusHere(-1);
          }
        }

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
  }
  ImGui::End();

  // ==================== POPUPS / OTHER WINDOWS ====================

  if (g_ShowLiveEditor) {
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    char title[128] = "Live Script Editor";
    if (g_LiveEditorTarget) {
      char name[64] = {};
      SafeGetName(g_LiveEditorTarget, name, 64);
      snprintf(title, sizeof(title), "Editing: %s", name);
    }

    if (ImGui::Begin(title, &g_ShowLiveEditor, 0)) {
      ImGui::InputTextMultiline(
          "##LiveSource", g_LiveEditorBuffer, IM_ARRAYSIZE(g_LiveEditorBuffer),
          ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - 40));

      if (ImGui::Button("Set Source", ImVec2(120, 30))) {
        if (g_LiveEditorTarget) {
          if (TrySetScriptSource(g_LiveEditorTarget, g_LiveEditorBuffer))
            CmdLog("[+] Source updated!");
          else
            CmdLog("[-] Failed to set source");
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Run Script", ImVec2(120, 30))) {
        if (g_LiveEditorTarget) {
          TrySetScriptSource(g_LiveEditorTarget, g_LiveEditorBuffer);
          if (TryRunScript(g_LiveEditorTarget))
            CmdLog("[+] Script Ran!");
          else
            CmdLog("[-] Failed to run");
        }
      }
    }
    ImGui::End();
  }

  // Net Monitor
  RenderNetMonWindow();
}