module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
export module visualButton;
import BGstyle;

bool checkbox_test = false;
export auto visualButton(ImDrawList* foreground_draw) -> void {
	ImGui::Text("Enable Visual");
	CustomGui::customCheckbox(foreground_draw, ImVec2(280.0f, 18.0f), ImVec2(292.0f, 30.0f), IM_COL32(128, 128, 128, 200), IM_COL32(32, 32, 32, 255), &checkbox_test);
}