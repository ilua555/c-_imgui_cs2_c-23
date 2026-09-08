module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <expected>
#include <print>
export module hooks;
import dxgi;
import patternScan;


export class c_hooks {
	c_hooks() = default;
	c_hooks(const c_hooks&) = delete;
	c_hooks(const c_hooks&&) = delete;
	c_hooks& operator=(c_hooks&) = delete;
	c_hooks& operator=(c_hooks&&) = delete;

public:
	static auto get() -> c_hooks* {
		static c_hooks init;
		return &init;
	}

public:
	auto EnableDxgi(std::expected<uintptr_t, c_patternScan::errors> TargetAddress) -> void {
		if (TargetAddress.has_value()) {
			std::print("dxgi address: {:#x}", TargetAddress.value());
			InitHookDxgi(TargetAddress.value() - static_cast<BYTE>(5));
			return;
		}
		else {
			std::print("\ndxgi error: {}", static_cast<int>(TargetAddress.error()));
			return;
		}
	}

	auto UnEnableDxgi() -> void {
		UnInitHookDxgi();
	}

};