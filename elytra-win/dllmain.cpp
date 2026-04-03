#include <Windows.h>
#include "SDK.h"
#include "UI.h"
#include "LuaEngine.h"
#include "Globals.h"
#include "menu/dx11hook.h"

void MainThread(HMODULE hModule)
{
    while (!GetModuleHandleA("GameAssembly.dll")) Sleep(100);

    if (!InitIL2CPP() || !AttachIL2CPP()) {
        MessageBoxA(0, "Failed to init SDK", "Elytra Error", MB_ICONERROR);
        FreeLibraryAndExitThread(hModule, 1);
        return;
    }

    InitLuaEngine();
    InitMoonSharp();
    LoadAllTabs();

    if (DX11::Hook(OnRenderCallback)) {
        CmdLog("[+] Elytra v1.5 Loaded!");
        CmdLog("[*] Press END to unload");

        while (true) {
            if (GetAsyncKeyState(VK_END) & 1) break;
            Sleep(100);
        }
    }
    else {
        MessageBoxA(0, "Failed to hook DX11", "Elytra Error", MB_ICONERROR);
    }

    DX11::Unhook();
    ShutdownMoonSharp();
    CleanupLuaEngine();

    if (g_Thread && il2cpp_thread_detach) {
        il2cpp_thread_detach(g_Thread);
    }

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
    }
    return TRUE;
}