#include "pch.h"
#include "Logic.h"

Logic::Logic() 
{
    this->moduleBase = (uintptr_t)GetModuleHandle(L"Spel2.exe");
	this->GetDynamicAddresses();
}

uintptr_t Logic::GetModuleBase()
{
    return this->moduleBase;
}

void Logic::GetDynamicAddresses()
{
	this->ply = (Player*)mem::ResolvePointerChain(moduleBase + 0x22DB7F08, { 0x1788, 0x178, 0x0 });
	this->nextLevel = (float*)mem::ResolvePointerChain(moduleBase + 0x22DB7F08, { 0x1798, 0x120, 0x0 });
}

BOOL Logic::IsPointerInvalid()
{
	if (this->ply == (Player*)0x40)
    {
		return true;
	} 
    return false;
}

void Logic::HandleInput()
{ 
    // Failsafe for level transitions where pointer might not be valid for a split second
    if (this->ply == NULL) return;

    if (GetAsyncKeyState(VK_DELETE) & 1)
    {
        this->keep_running = 0;
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD0) & 1)
    {
        BOOL temp = this->lockMoney;
        if (!temp)  // Off -> on
        {
            this->prevMoney = this->ply->utility->money;
        }
        else        // On -> off   
        {
            this->ply->utility->money = this->prevMoney;
        }
        this->lockMoney = ~(this->lockMoney);
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
        BOOL temp = this->godMode;
        if (!temp)
        {
            this->prevHealth = this->ply->health;
        }
        else
        {
            this->ply->health = this->prevHealth;
        }
        this->godMode = ~(this->godMode);
        this->ToggleGodMode(this->godMode);
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
    {
        BOOL temp = this->lockBombs;
        if (!temp)
        {
            this->prevBombs = this->ply->utility->bombs;
        }
        else
        {
            this->ply->utility->bombs = this->prevBombs;
        }
        this->lockBombs = ~(this->lockBombs);
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 1)
    {
        BOOL temp = this->lockRopes;
        if (!temp)
        {
            this->prevRopes = this->ply->utility->ropes;
        }
        else
        {
            this->ply->utility->ropes = this->prevRopes;
        }
        this->lockRopes = ~(this->lockRopes);
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD4) & 1)
    {
        this->speedHack = ~(this->speedHack);
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD5) & 1)
    {
        this->highJump = ~(this->highJump);
        this->ToggleHighJump(this->highJump);
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD6) & 1)
    {
        return;
    }


    if (GetAsyncKeyState(VK_NUMPAD7) & 1)
    {
        if (this->ply->state != 240)
        {
            this->ply->Teleport(this->nextLevel[0], this->nextLevel[1]);
        }
        return;
    }
}

void Logic::ContinousWrite() 
{
    // Failsafe for level transitions where pointer might not be valid for a split second
    if (this->ply == NULL) return;

    if (this->lockBombs)
    {
        this->ply->utility->bombs = 44;
    }

    if (this->lockRopes)
    {
        this->ply->utility->ropes = 55;
    }

    if (this->lockMoney)
    {
        this->ply->utility->money = 66778899;
    }

    if (this->speedHack)
    {
        if (this->ply->state != 88 && this->ply->state != 176 && this->ply->state != 240) {
            if (GetAsyncKeyState(VK_LEFT))
            {
                this->ply->x -= this->runModifier;
            }
            else if (GetAsyncKeyState(VK_RIGHT))
            {
                this->ply->x += this->runModifier;
            }
        }
    }
}

void Logic::ToggleGodMode(BOOL toggle)
{
    uintptr_t DETOUR_SOURCE = this->moduleBase + 0x228d5a92;

    if (toggle) 
    {
        this->ply->health = 33;

        // detour to code cave
        mem::Patch(DETOUR_SOURCE, 7, "\xE9\x6E\x13\x3E\x00\x90\x90");
    }
    else
    {
        this->ply->health = prevHealth;

        // restore stolen bytes
        mem::Patch(DETOUR_SOURCE, 7, "\x41\x88\x86\x17\x01\x00\x00");
    }
}

void Logic::ToggleHighJump(BOOL toggle)
{
    uintptr_t DETOUR_SOURCE = this->moduleBase + 0x22885565;

    if (toggle)
    {
        // detour to code cave
        mem::Patch(DETOUR_SOURCE, 9, "\xE9\xBa\x18\x43\x00\x90\x90\x90\x90");
    }
    else
    {
        // restore stolen bytes
        mem::Patch(DETOUR_SOURCE, 9, "\xF3\x41\x0F\x11\xB6\x0C\x01\x00\x00");
    }
}