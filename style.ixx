module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
export module style;



export enum styles {
	Neon = 0,
};

auto Neon_style() -> void;


export class c_style {

private:
	c_style() = default;
	c_style(const c_style&) = delete;
	c_style(const c_style&&) = delete;
	c_style& operator=(c_style&) = delete;
	c_style& operator=(c_style&&) = delete;

public:
	static auto get() -> c_style* {
		static c_style init;
		return &init;
	}

public:
	auto choose_style(unsigned char number_style) -> int {
		switch (number_style) {
		case Neon: {
			Neon_style();
			return Neon;
		}
		}
	}
};





auto Neon_style() -> void {
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.0f, nullptr, 0);
	ImGui::GetStyle().WindowPadding = ImVec2(45.0f, 15.0f);

}