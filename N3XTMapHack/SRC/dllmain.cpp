#include <windows.h>
#include <iostream>
#include <stdlib.h>

#define ZOOM_OFFSET	0x00876784
#define NORMAL_ZOOM	0xC4757000
#define ZOOM_LIFT	0xC4D57000
#define MAPOFFSET	0x004D5FA0
#define OFFSET0		0x00000028
#define OFFSET1		0x00000034
#define OFFSET2		0x00000040
#define OFFSET3		0x0000004C

void ZoomRestore(DWORD base);
void ZoomLift(DWORD base);
void MapEdit(DWORD base);
void RedirectIOToConsole();
void MapRestore(DWORD base);
VOID WINAPI ThreadProc(HMODULE hModule);

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
	
	MessageBox(NULL, L"PRESS OK WHEN YOU HAVE LOGGED IN", L"N3XT Map Hack", MB_OK);
	printf("OPEN-N3XT Hacks Find us on GitHub : https://github.com/OPEN-N3XT \n");
	DWORD BASE = (DWORD)(GetModuleHandleA("PathOfExile.exe"));
	printf("Press F5 to restore and unload the hack\n");
	MapEdit(BASE);
	ZoomLift(BASE);

	bool loop = true;
	
	while (loop)
	{
		if (GetAsyncKeyState(VK_F5))
		{
			loop = false;
			MapRestore(BASE);
			ZoomRestore(BASE);
			printf("Values restored, unloading hack\n");
			FreeLibraryAndExitThread(hModule, 0);
		}
	}
}

void MapEdit(DWORD base)
{
	DWORD loc1, loc2, loc3, loc0;

	base += MAPOFFSET;
	loc0 = (base + OFFSET0);
	loc1 = (base + OFFSET1);
	loc2 = (base + OFFSET2);
	loc3 = (base + OFFSET3);

	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)base, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	*(DWORD*)loc0 = 0x240C8BE8;
	*(DWORD*)loc1 = 0xE8D9240C;
	*(DWORD*)loc2 = 0xE8D9240C;
	*(DWORD*)loc3 = 0xE8D9240C;

	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
}

void MapRestore(DWORD base)
{
	DWORD loc1, loc2, loc3, loc0;

	base += MAPOFFSET;
	loc0 = (base + OFFSET0);
	loc1 = (base + OFFSET1);
	loc2 = (base + OFFSET2);
	loc3 = (base + OFFSET3);

	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)base, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	*(DWORD*)loc0 = 0x240C8B00;
	*(DWORD*)loc1 = 0x00D9240C;
	*(DWORD*)loc2 = 0x00D9240C;
	*(DWORD*)loc3 = 0x00D9240C;

	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
}

void ZoomLift(DWORD base)
{
	DWORD address = base + ZOOM_OFFSET;
	*(DWORD*)address = ZOOM_LIFT;
}

void ZoomRestore(DWORD base)
{
	DWORD address = base + ZOOM_OFFSET;
	*(DWORD*)address = NORMAL_ZOOM;
}

void RedirectIOToConsole()
{
	FILE *conin, *conout;
	AllocConsole();
	freopen_s(&conin, "conin$", "r", stdin);
	freopen_s(&conout, "conout$", "w", stdout);
	freopen_s(&conout, "conout$", "w", stderr);
}