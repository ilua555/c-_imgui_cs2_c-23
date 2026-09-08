module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "skyCrypt/skCrypter.h"
#include <print>
#define CONSOLE_DEBUG 1 //0 - если надо отключить консоль
export module main;
import console;
import hooks;
import patternScan;

DWORD WINAPI MainThread(LPVOID LpReserv) {
	#if CONSOLE_DEBUG == 1
	c_console::get()->console_start();
	#endif
	c_hooks::get()->EnableDxgi(c_patternScan::get()->StartScan(GetModuleHandleA(skCrypt("dxgi.dll")), "\x48\x89\x74\x24\x20\x55\x57\x41\x56", "xxxxxxxxx"));

	while (true) {
		Sleep(600);
		if (GetAsyncKeyState(VK_END) & 0x8000) {
			#if CONSOLE_DEBUG == 1
			c_console::get()->console_end();
			#endif
			c_hooks::get()->UnEnableDxgi();
			FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(LpReserv), 1);
		}
	}
	return 1;
}


export class c_main{
	c_main() = default;
	c_main(const c_main&) = delete;
	c_main(const c_main&&) = delete;
	c_main& operator=(c_main&) = delete;
	c_main& operator=(c_main&&) = delete;

public:
	static auto get() -> c_main* {
		static c_main init;
		return &init;
	}

public:
	auto StartThread(HMODULE hModule) {
		using th_fn = HANDLE(WINAPI*) (LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
		th_fn th = reinterpret_cast<th_fn>(GetProcAddress(GetModuleHandleA(skCrypt("kernel32.dll")), skCrypt("CreateThread")));
		HANDLE hTh = th(nullptr, 0, &MainThread, hModule, 0, nullptr);
		CloseHandle(hTh);
	}
};