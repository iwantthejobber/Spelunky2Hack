#pragma once
#include <Windows.h>
#include <vector>
// Created with ReClass.NET 1.2 by KN4CK3R

class Utility
{
public:
	DWORD money;
	char bombs;
	char ropes;
};

class Player
{
public:
	void* N0000004F; //0x0000
	void* N00000050; //0x0008
	char pad_0010[48]; //0x0010
	float x; //0x0040
	float y; //0x0044
	float readX; //0x0048
	float readY; //0x004C
	float sprite_thickness; //0x0050
	float sprite_height; //0x0054
	char pad_0058[112]; //0x0058
	int8_t state; //0x00C8
	int8_t attacking; //0x00C9
	char pad_00CA[62]; //0x00CA
	float dx; //0x0108
	float dy; //0x010C
	int16_t heldItemId; //0x0110
	int16_t isHolding; //0x0112
	int16_t state2; //0x0114
	int8_t N00000073; //0x0116
	int8_t health; //0x0117
	char pad_0118[40]; //0x0118
	Utility* utility; //0x0140
	char pad_0148[48]; //0x0148
	float readYnd; //0x0178
	char pad_017C[20]; //0x017C
	
	void Teleport(float x, float y);
}; 

