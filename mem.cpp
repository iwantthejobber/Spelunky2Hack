#include "pch.h"
#include "mem.h"

uintptr_t mem::ResolvePointerChain(const uintptr_t baseAddress, const std::vector<unsigned int> offsets)
{
	uintptr_t resolvedAddress = baseAddress;
	for (int i = 0; i < offsets.size(); i++) 
	{
		resolvedAddress = *(uintptr_t*)resolvedAddress;
		resolvedAddress += offsets[i];
	}
	return resolvedAddress;
}

void mem::Patch(const uintptr_t addr, const SIZE_T size, const void* data)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((LPVOID)addr, data, size);
	DWORD temp;
	VirtualProtect((LPVOID)addr, size, oldProtect, &temp);
}

void mem::Hook(const uintptr_t addr, const uintptr_t function, const size_t size)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	DWORD relAddress = ((DWORD)function - (DWORD)addr) - 5;
	*(BYTE*)addr = 0xE9;
	*(DWORD*)((DWORD)addr + 1) = relAddress;

	DWORD temp;
	VirtualProtect((LPVOID)addr, size, oldProtect, &temp);
}

void mem::GodModeCave(const uintptr_t moduleBase)
{
	const char* code = "\x48\x81\xF9\xC8\x00\x00\x00\x0F\x84\x05\x00\x00\x00\xE9\x07\x00\x00\x00\x41\x88\x86\x17\x01\x00\x00\xE9";
	// hack code + E9 (jmp) needs return address written after
	const BYTE codeLength = 26;

	const uintptr_t DETOUR_SOURCE = moduleBase + 0x228d5a92;
	const uintptr_t CAVE = DETOUR_SOURCE + 0x3E136E + 4;
	const DWORD RETURN_FROM_CAVE = (DWORD)DETOUR_SOURCE + 5 - ((DWORD)CAVE + codeLength +4);

	DWORD temp;
	VirtualProtect((LPVOID)CAVE, 40, PAGE_EXECUTE_READWRITE, &temp);
	
	Patch(CAVE, codeLength, (void*)code);
	*((DWORD*)(CAVE + codeLength)) = (DWORD)RETURN_FROM_CAVE;
}

void mem::HighJumpCave(const uintptr_t moduleBase)
{
	const char* code = "\x49\x81\xFA\xFF\x3F\x00\x00\x0F\x84\x08\x00\x00\x00\xF3\x0F\x58\x35\x10\x00\x00\x00\xF3\x41\x0F\x11\xB6\x0C\x01\x00\x00\xE9";
	// hack code + E9 (jmp) needs return address written after
	const BYTE codeLength = 31;

	const uintptr_t DETOUR_SOURCE = moduleBase + 0x22885565;
	const uintptr_t CAVE = DETOUR_SOURCE + 0x4318BA + 4 + 1; // +1 for proper alignment
	const DWORD RETURN_FROM_CAVE = (DWORD)DETOUR_SOURCE + 5 - ((DWORD)CAVE + codeLength + 4);


	float* jumpModifier = (float*)(CAVE + codeLength + 4 + 2);
	*jumpModifier = 0.06f;

	DWORD temp;
	VirtualProtect((LPVOID)CAVE, 100, PAGE_EXECUTE_READWRITE, &temp);

	Patch(CAVE, codeLength, (void*)code);
	*((DWORD*)(CAVE + codeLength)) = (DWORD)RETURN_FROM_CAVE;
}

void mem::InitialiseCodeCaves(const uintptr_t moduleBase)
{
	GodModeCave(moduleBase);
	HighJumpCave(moduleBase);
}