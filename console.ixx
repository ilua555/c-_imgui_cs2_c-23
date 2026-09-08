module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>
export module console;



export class c_console {
private:
	FILE* f;
private:
	c_console() : f(nullptr) {};
	c_console(const c_console&) = delete;
	c_console(const c_console&&) = delete;
	c_console& operator=(c_console&) = delete;
	c_console& operator=(c_console&&) = delete;

public:
	static auto get() -> c_console* {
		static c_console init;
		return &init;
	}

public:
	auto console_start() -> void {
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
	}

	auto console_end() -> void {
		if (f != nullptr) {
			fclose(f);
			f = nullptr;
		}
		FreeConsole();
	}
};