module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "cstdio"
#include <d3d11.h>
export module BGstyle;



export namespace CustomGui {
	namespace Photo {
		ID3D11ShaderResourceView* photoResourse_head = nullptr;
		int x_texture_head, y_texture_head, chanels_in_file_head = 0;

		ID3D11ShaderResourceView* photoResourse_person = nullptr;
		int x_texture_person, y_texture_person, chanels_in_file_person = 0;

		ID3D11ShaderResourceView* photoResourse_rect = nullptr;
		int x_texture_rect, y_texture_rect, chanels_in_file_rect = 0;

		ID3D11ShaderResourceView* photoResourse_gear = nullptr;
		int x_texture_gear, y_texture_gear, chanels_in_file_gear = 0;
	}

	auto customButton(ImDrawList* foreground_draw, ImVec2 pos_start, ImVec2 pos_end, ImU32 color, const char* text) -> bool {
		ImVec2 mouse_pos = ImGui::GetMousePos();
		ImVec2 text_size = ImGui::CalcTextSize(text);
		foreground_draw->AddRectFilled(pos_start, pos_end, color, ImDrawFlags_None);

		ImVec2 text_pos_center = ImVec2((pos_start.x + (pos_end.x - pos_start.x) / 2 - text_size.x / 2), (pos_start.y + (pos_end.y - pos_start.y) / 2 - text_size.y / 2));
		foreground_draw->AddText(text_pos_center, IM_COL32(255, 255, 255, 255), text);

		if (mouse_pos.x >= pos_start.x && mouse_pos.x <= pos_end.x && mouse_pos.y >= pos_start.y && mouse_pos.y <= pos_end.y) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				return true;
			}
		}
		return false;
	}

	auto customCheckbox(ImDrawList* foreground_draw, ImVec2 pos_start, ImVec2 pos_end, ImU32 color_disabled, ImU32 color_enabled, bool* bool_checkbox_is_power) -> void {
		ImVec2 mos_pos = ImGui::GetMousePos();
		ImVec2 rect_pos_start = ImVec2(ImGui::GetWindowPos().x + pos_start.x, ImGui::GetWindowPos().y + pos_start.y);
		ImVec2 rect_pos_end = ImVec2(ImGui::GetWindowPos().x + pos_end.x, ImGui::GetWindowPos().y + pos_end.y);
		foreground_draw->AddRectFilled(rect_pos_start, rect_pos_end, color_disabled, 0.0f, ImDrawFlags_None); //основной квдарат
		if (mos_pos.x >= rect_pos_start.x && mos_pos.x <= rect_pos_end.x && mos_pos.y >= rect_pos_start.y && mos_pos.y <= rect_pos_end.y) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				*bool_checkbox_is_power = !*bool_checkbox_is_power;
			}
		}

		if (*bool_checkbox_is_power) {
			ImVec2 check_box_start_enabled = ImVec2((rect_pos_start.x + ((rect_pos_end.x - rect_pos_start.x) * 0.25f)), (rect_pos_start.y + ((rect_pos_end.y - rect_pos_start.y) * 0.25f)));
			ImVec2 check_box_end_enabled = ImVec2((rect_pos_end.x - ((rect_pos_end.x - rect_pos_start.x) * 0.25f)), (rect_pos_end.y - ((rect_pos_end.y - rect_pos_start.y) * 0.25f)));
			foreground_draw->AddRectFilled(check_box_start_enabled, check_box_end_enabled, color_enabled, 0.0f, ImDrawFlags_None);
		}
	}

	auto customImageButton(ImDrawList* foreground_draw, ImVec2 screen_pos, ImVec2 size_image, ID3D11ShaderResourceView* photoShader) -> bool {
		foreground_draw->AddImage(photoShader, screen_pos, ImVec2(screen_pos.x + size_image.x, screen_pos.y + size_image.y));
		
		ImVec2 mouse_pos = ImGui::GetMousePos();
		if (mouse_pos.x >= screen_pos.x && mouse_pos.x <= screen_pos.x + size_image.x && mouse_pos.y >= screen_pos.y && mouse_pos.y <= screen_pos.y + size_image.y) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				return true;
			}
		}
		return false;
	}
}


export auto BGstyle(ImDrawList* background_draw, ImDrawList* foreground_draw) -> void {

	//квадрат слева от меню
	background_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x - 100, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 700), IM_COL32(96, 96, 96, 255), 8.0f, ImDrawFlags_RoundCornersLeft);
	//квадрат в главной зоне слева
	foreground_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x + 40, ImGui::GetWindowPos().y + 10), ImVec2(ImGui::GetWindowPos().x + 300, ImGui::GetWindowPos().y + 700 - 10), IM_COL32(64, 64, 64, 100), 6.0f, ImDrawFlags_None);
	//квадрат в главной зоне справа
	foreground_draw->AddRectFilled(ImVec2(ImGui::GetWindowPos().x + 340, ImGui::GetWindowPos().y + 10), ImVec2(ImGui::GetWindowPos().x + 640, ImGui::GetWindowPos().y + 700 - 10), IM_COL32(64, 64, 64, 100), 6.0f, ImDrawFlags_None);

}