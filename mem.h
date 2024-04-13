#pragma once
#include <vector>

// basic utility namespace for memory manipulation

namespace mem 
{
	uintptr_t ResolvePointerChain(const uintptr_t baseAddress, const std::vector<unsigned int> offsets);

	// replace specific area of memory with desired input
	void Patch(const uintptr_t addr, const SIZE_T size, const void* data);

	// insert asm jump statement to create a "detour"
	void Hook(const uintptr_t addr, const uintptr_t function, const size_t size);


	// Game specific functions
	void InitialiseCodeCaves(const uintptr_t moduleBase);
	void GodModeCave(const uintptr_t moduleBase);
	void HighJumpCave(const uintptr_t moduleBase);
}
