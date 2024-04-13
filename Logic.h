#pragma once
#include <Windows.h>
#include <vector>
#include "mem.h"
#include "ReClass.h"

/*
    Main 'engine' of the cheat
*/


class Logic
{
    // toggles for different features like infinite supplies or health points
    BOOL godMode = 0;
    BOOL lockBombs = 0;
    BOOL lockRopes = 0;
    BOOL lockMoney = 0;
    BOOL speedHack = 0;
    BOOL highJump = 0;

    // original state of consumables/health to restore after disabling hacks
    DWORD prevMoney;
    char prevHealth;
    char prevRopes;
    char prevBombs;

    // modifiers for high jump and speedy gonzales hack 
    float jumpModifier = 0.06f; // 0.06 
    float runModifier = 0.015f; // 0.015 roughly double original

    // pointers to structures/variables of interest
    uintptr_t moduleBase;
    Player* ply;
    float* nextLevel;

public:
    BOOL keep_running = 1;
    Logic();
    uintptr_t GetModuleBase();
    void GetDynamicAddresses();
    BOOL IsPointerInvalid();
    void HandleInput();
    void ContinousWrite();

    void ToggleGodMode(BOOL toggle);
    void ToggleHighJump(BOOL toggle);
};

