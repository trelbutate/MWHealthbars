
#include <Windows.h>

#include "healthbars.h"

HealthbarRenderer renderer;

typedef void (*DrawGUI)(bool someValue);

DrawGUI realDrawGUIFunc = (DrawGUI)0x6e6e40;

void DrawGUIHook(bool someValue) {
    if (!someValue)
        renderer.Draw();
    realDrawGUIFunc(someValue);
}

void HookFunction() {    
    DWORD previous;
    void* instrAddress = (void*)0x6e75a7;
    VirtualProtect(instrAddress, 4, PAGE_READWRITE, &previous);

    void* targetFunctionAddress = (void*)DrawGUIHook;
    int diff = ((int)targetFunctionAddress - (int)instrAddress - 5);
    *(unsigned int*)((int)instrAddress + 1) = diff;

    VirtualProtect(instrAddress, 4, previous, &previous);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        HookFunction();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

