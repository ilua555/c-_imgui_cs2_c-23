module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <dxgi.h>
#include <d3d11.h>
#include "safetyhook/safetyhook.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
export module dxgi;
import menu;
import style;
import custom_cursor;
import conveer_photo;
import headshot_png;
import person_png;
import rect_line_png;
import gear_png;
import BGstyle;


extern "C++" IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


safetyhook::InlineHook HOOK{};
inline ID3D11Device* pDevice = nullptr;
inline ID3D11DeviceContext* pContext = nullptr;
inline ID3D11RenderTargetView* mainRenderTargetView = nullptr;
inline HWND window = nullptr;
inline WNDPROC oWndProc = nullptr;
inline bool is_initialized = false;
inline bool show_menu = true;
inline bool cursor_draw = true;
LRESULT CALLBACK WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
        show_menu = !show_menu;
        return 0;
    }
    
    if (show_menu) {
        SetCursor(hCustomCursor);
        cursor_draw = false;
    }
    if (!cursor_draw && !show_menu) {
        SetCursor(LoadCursorA(nullptr, IDC_ARROW));
        cursor_draw = true;
    }



    

    if (show_menu) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;
    }

    if (show_menu) {
        switch (uMsg){
        case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDOWN: case WM_XBUTTONUP: case WM_XBUTTONDBLCLK:
        case WM_MOUSEMOVE:   case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
        case WM_KEYDOWN:    case WM_KEYUP:
        case WM_SYSKEYDOWN: case WM_SYSKEYUP:
        case WM_CHAR:       case WM_UNICHAR:
            return 0; 
        }
    }


    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


auto __fastcall HookDxgi(IDXGISwapChain* SwapChain, unsigned int a2, int a3) -> __int64 {
    if (!is_initialized) {
        if (SUCCEEDED(SwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
            pDevice->GetImmediateContext(&pContext);

            DXGI_SWAP_CHAIN_DESC sd;
            SwapChain->GetDesc(&sd);
            window = sd.OutputWindow;

            ID3D11Texture2D* pBackBuffer = nullptr;
            SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            if (pBackBuffer) {
                pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
                pBackBuffer->Release();
            }
            
            oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

            ImGui::CreateContext();
            c_style::get()->choose_style(0);
            CustomGui::Photo::photoResourse_head = LoadPhoto(pDevice, headshot_png, size_headshot_png, &CustomGui::Photo::x_texture_head, &CustomGui::Photo::y_texture_head, &CustomGui::Photo::chanels_in_file_head);// загрузили фото прицела над головой
            CustomGui::Photo::photoResourse_person = LoadPhoto(pDevice, person_png, size_person_png, &CustomGui::Photo::x_texture_person, &CustomGui::Photo::y_texture_person, &CustomGui::Photo::chanels_in_file_person);// загрузили пнг человечка
            CustomGui::Photo::photoResourse_rect = LoadPhoto(pDevice, rect_line_png, size_rect_line_png, &CustomGui::Photo::x_texture_rect, &CustomGui::Photo::y_texture_rect, &CustomGui::Photo::chanels_in_file_rect); //иконка с квадратом и полоской под ним
            CustomGui::Photo::photoResourse_gear = LoadPhoto(pDevice, gear_png, size_gear_png, &CustomGui::Photo::x_texture_gear, &CustomGui::Photo::y_texture_gear, &CustomGui::Photo::chanels_in_file_gear); //иконка шестеренки

            ImGui_ImplWin32_Init(window);
            ImGui_ImplDX11_Init(pDevice, pContext);

            is_initialized = true;
        }
    }

    
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //- работа со спавном окна и его размером
    if (show_menu) {
        static bool size_imgui = true;
        if (size_imgui) {
            ImGui::SetNextWindowPos(ImVec2(500, 500));
            ImGui::SetNextWindowSize(ImVec2(680, 700));
            size_imgui = false;
        }
    }

    //-- гуи самого окна
    if (show_menu) {
        menu();
    }

    ImGui::Render();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());



	return HOOK.call<__int64>(SwapChain, a2, a3);
}


export auto InitHookDxgi(uintptr_t TargetAddress) {
	auto hook = safetyhook::create_inline(TargetAddress, HookDxgi);
	HOOK = std::move(hook);
}

export auto UnInitHookDxgi() {
	HOOK.reset();
    if (is_initialized) {
        if (window && oWndProc) {
            SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (mainRenderTargetView) { mainRenderTargetView->Release(); mainRenderTargetView = nullptr; }
        if (pContext) { pContext->Release(); pContext = nullptr; }
        if (pDevice) { pDevice->Release(); pDevice = nullptr; }

        is_initialized = false;
    }
}