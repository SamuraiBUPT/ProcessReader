//
// Created by D on 2023-08-05.
//

#include <dwmapi.h>
#include <stdexcept>
#include "windowbinder.h"

#pragma comment(lib,"dwmapi.lib")


WindowBinder::WindowBinder(std::string window_name, std::string class_name) {
    if (window_name.empty() || class_name.empty()){
        throw std::runtime_error("window not found");
    }
    this->window_name = window_name;
    this->class_name = class_name;
    create_overlay();
}

WindowBinder::~WindowBinder() {

}

void WindowBinder::create_overlay() {
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = m_win_proc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = nullptr;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = HBRUSH(RGB(0, 0, 0));
    wc.lpszMenuName = "";
    wc.lpszClassName = "wnd";
    wc.hIconSm = nullptr;
    if (!RegisterClassExA(&wc)) {
        throw std::runtime_error("window class not registered");
    }

    this->target_hwnd = FindWindow(class_name.data(), window_name.data());
    GetWindowRect(target_hwnd, &target_rect);

    overlay_hwnd = CreateWindowExA(
            WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
            "wnd",
            "",
            WS_VISIBLE | WS_POPUP,
            target_rect.left, target_rect.top, target_rect.width(), target_rect.height(),
            nullptr, nullptr, nullptr, nullptr
    );

    MARGINS m = {
            target_rect.left,
            target_rect.top,
            target_rect.width(),
            target_rect.height()
    };

    DwmExtendFrameIntoClientArea(overlay_hwnd, &m);
    SetLayeredWindowAttributes(
            overlay_hwnd,
            RGB(0, 0, 0),
            255,
            LWA_ALPHA
    );
    ShowWindow(overlay_hwnd, SW_SHOW);
}

LRESULT WindowBinder::m_win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    switch (msg) {
        case WM_DESTROY:
            exit(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return LRESULT();
}
