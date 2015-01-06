#include <windows.h>
#include <iostream>
#include <stdlib.h>

#define ZOOM_OFFSET		0x00876784 
#define NORMAL_ZOOM		0xC4757000 
#define MAX_ZOOM		0xC4D57000 
#define MAPOFFSET		0x004D5FA0

#define BRIGHT1			0x000005FC 
#define BRIGHT2			0x00000652 
#define BRIGHT_OFFSET		0x00232FC0

void Light(DWORD base);
void MapEdit(DWORD base, bool restore);
void RedirectIOToConsole();
void IncreaseZoomCeiling();
VOID WINAPI ThreadProc(HMODULE hModule);

DWORD BASE;
bool BREAK = false;
//------------Main----------------//
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		RedirectIOToConsole();
		DisableThreadLibraryCalls(hModule);
		HANDLE thread_Handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, hModule, 0, NULL);
		break;
	}
	return TRUE;
}

VOID WINAPI ThreadProc(HMODULE hModule)
{

	printf("MapHack -> F2\n FullBright -> F3\n ZoomHack -> F4\n Unload Hack -> Home\n No toggles, once its on, its on");

	BASE = (DWORD)(GetModuleHandleA("PathOfExile.exe"));
	
	while (!BREAK)
	{
		if (GetAsyncKeyState(VK_F2)) // MAP HACK
		{
			printf("MapHack Active\n");
			MapEdit(BASE, false);
		}

		if (GetAsyncKeyState(VK_F3))//fullBright hack
		{
			printf("FullBright Active\n");
			Light(BASE);
		}

		if (GetAsyncKeyState(VK_F4))//ZoomCeiling increase
		{
			printf("Zoomhack Active\n");
			DWORD address = BASE + ZOOM_OFFSET;
			*(DWORD*)address = MAX_ZOOM;
		}

		if (GetAsyncKeyState(VK_HOME))
		{
			printf("Unloading injected module\n");
			FreeLibraryAndExitThread(hModule, 0);
		}
		Sleep(150);
	}

}

void MapEdit(DWORD base, bool restore)
{
	DWORD loc1, loc2, loc3, loc0;
	
	//Offset to change 0	: 0x028
	//Offset to change 1	: 0x034 
	//offset to change 2	: 0x040
	//offset to change 3	: 0x04C

	base += MAPOFFSET;

	loc0	= (base + 0x028);
	loc1	= (base + 0x034);
	loc2	= (base + 0x040);
	loc3	= (base + 0x04C);

	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)base, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);

	if (restore)
	{
		*(DWORD*)loc0 = 0x240C8B00;
		*(DWORD*)loc1 = 0x00D9240C;
		*(DWORD*)loc2 = 0x00D9240C;
		*(DWORD*)loc3 = 0x00D9240C;
	}
	else
	{
		*(DWORD*)loc0 = 0x240C8BE8;
		*(DWORD*)loc1 = 0xE8D9240C;
		*(DWORD*)loc2 = 0xE8D9240C;
		*(DWORD*)loc3 = 0xE8D9240C;
	}

	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
}

void Light(DWORD base)
{
	DWORD locBase, b1, b2;

	locBase = base + BRIGHT_OFFSET;
	
	b1 = (locBase + BRIGHT1) + 4;
	b2 = (locBase + BRIGHT2) + 4;

	MEMORY_BASIC_INFORMATION mbi, mbi1;
	VirtualQuery((LPCVOID)*(DWORD*)b1, &mbi, sizeof(mbi));
	VirtualQuery((LPCVOID)*(DWORD*)b2, &mbi1, sizeof(mbi1));

	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
	VirtualProtect((LPVOID)mbi1.BaseAddress, mbi1.RegionSize, PAGE_READWRITE, &mbi1.Protect);

	*(DWORD*)*(DWORD*)b1 = 0x469C4000;
	*(DWORD*)*(DWORD*)b2 = 0x453B8000;

	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
	VirtualProtect((LPVOID)mbi1.BaseAddress, mbi1.RegionSize, mbi1.Protect, &mbi1.Protect);
}

void RedirectIOToConsole()
{
	FILE *conin, *conout;
	AllocConsole();
	freopen_s(&conin, "conin$", "r", stdin);
	freopen_s(&conout, "conout$", "w", stdout);
	freopen_s(&conout, "conout$", "w", stderr);
}
