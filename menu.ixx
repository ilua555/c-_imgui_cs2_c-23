module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
export module menu;
import legitBotButton;
import visualButton;
import miscButton;
import settingsButton;
import BGstyle;

enum class tabs : char {
	no_selected = -1,
	settings = 0,
	misc = 1,
	visual = 2,
	legitbot = 3,
};



export auto menu() -> void {
	using enum tabs;
	ImDrawList* background_draw = ImGui::GetBackgroundDrawList();
	ImDrawList* foreground_draw = ImGui::GetForegroundDrawList();

	ImGui::Begin("NULL", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	//backgeround style(название странно но также рисуется и foreground тут)
	BGstyle(background_draw, foreground_draw);

	//main buttons
	static tabs tab = no_selected;
	
	if (CustomGui::customImageButton(foreground_draw, ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 20), ImVec2(100, 100), CustomGui::Photo::photoResourse_head)) {
		tab = legitbot;
	}
	if (CustomGui::customImageButton(foreground_draw, ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 150), ImVec2(100, 100), CustomGui::Photo::photoResourse_person)) {
		tab = visual;
	}
	if (CustomGui::customImageButton(foreground_draw, ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 300), ImVec2(100, 100), CustomGui::Photo::photoResourse_rect)) {
		tab = misc;
	}
	if (CustomGui::customImageButton(foreground_draw, ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 500), ImVec2(100, 100), CustomGui::Photo::photoResourse_gear)) {
		tab = settings;
	}


	switch (tab) {
	case legitbot: {
		legitBotButton(foreground_draw);
		foreground_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 13), ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 125), IM_COL32(255, 255, 255, 20)); //подсветка нажатой кнопки
		break;
	}

	case visual: {
		visualButton(foreground_draw);
		foreground_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 138), ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 258), IM_COL32(255, 255, 255, 20)); //подсветка нажатой кнопки
		break;
	}

	case misc: {
		miscButton(foreground_draw);
		foreground_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 280), ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 415), IM_COL32(255, 255, 255, 20)); //подсветка нажатой кнопки
		break;
	}

	case settings: {
		settingsButton(foreground_draw);
		foreground_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y + 490), ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 610), IM_COL32(255, 255, 255, 20)); //подсветка нажатой кнопки
		break;
	}

	}
	
	ImGui::End();
}