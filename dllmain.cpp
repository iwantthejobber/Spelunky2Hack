// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include "logic.h"


DWORD WINAPI HackThread(HMODULE hModule) 
{
    Logic logic;
    mem::InitialiseCodeCaves(logic.GetModuleBase());

    do 
    {
        logic.GetDynamicAddresses();
        if (logic.IsPointerInvalid()) continue;
        logic.HandleInput();
        logic.ContinousWrite();

        Sleep(2);
    } while (logic.keep_running);

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

