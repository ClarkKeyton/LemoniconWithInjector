#pragma once
#include "includes.h"
#include <MinHook/MinHook.h>
#define MinHOOK_FASTCALL __fastcall*
#define MinHOOK_STDCALL __stdcall
uintptr_t baseaddr = (uintptr_t)GetModuleHandleA("GameAssembly.dll"); //GameAssembly REAL ADDRESS :D
uintptr_t unity_player = (uintptr_t)GetModuleHandleA("UnityPlayer.dll"); //UnityPlayer REAL ADDRESS :D
LPVOID* MinHookMagic(uintptr_t RVA) //RVA it's Fixed Address... Example: GameAssembly.dll+0x4532AB 
{
	return reinterpret_cast<LPVOID*>(baseaddr + RVA); //Please Write RVA Address :D
}

void(MinHOOK_FASTCALL CheckCheatTools_OnStateBegin_o)(DWORD*, DWORD*);

void MinHOOK_STDCALL CheckCheatTools_OnStateBegin_h(DWORD* __this, DWORD* method) 
{
	cout << "If you see Japanese Message Box Error, please try cleaning regedit and delete all folders when you installing Cheat Engine or something else" << endl;
	return CheckCheatTools_OnStateBegin_o(__this, method); //Returning of Original Code :D
}

void MinHook_InitCheckCheatTools() 
{
	MH_CreateHook(MinHookMagic(0x840740), &CheckCheatTools_OnStateBegin_h, (LPVOID*)&CheckCheatTools_OnStateBegin_o);
	MH_EnableHook(MinHookMagic(0x840740));
}

void(MinHOOK_FASTCALL NameVisible_orig)(DWORD*, bool, DWORD*);

void(MinHOOK_STDCALL NameVisible_hack)(DWORD* __this, bool isVisible, DWORD* method) {
	isVisible = false;

	return NameVisible_orig(__this, isVisible, method);
}

void MinHook_InitNameInvisibleRemotePlayer() 
{
	MH_CreateHook(MinHookMagic(0x7CEC70), &NameVisible_hack, (LPVOID*)&NameVisible_orig);
	MH_EnableHook(MinHookMagic(0x7CEC70));
}

bool(MinHOOK_FASTCALL TerminateSpecificDllDetected_originalcode)(DWORD*);

bool(MinHOOK_STDCALL TerminateSpecificDllDetected_hacked)(DWORD* method) 
{
	cout << "DLL Detection has been Disabled!!!" << endl;
	return false; //TRUE it's searching all DLL files and Blocked him, FALSE it's ignoring this :D
}

void MinHook_MagicTerminateSpecificDLLDetected() {
	MH_CreateHook(MinHookMagic(0x7A9BB0), &TerminateSpecificDllDetected_hacked, (LPVOID*)&TerminateSpecificDllDetected_originalcode);
	MH_EnableHook(MinHookMagic(0x7A9BB0));
}