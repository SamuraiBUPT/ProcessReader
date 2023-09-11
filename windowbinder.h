//
// Created by D on 2023-08-05.
//

#include <string>
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

#ifndef GAMECHEATINGTOOL_WINDOWBINDER_H
#define GAMECHEATINGTOOL_WINDOWBINDER_H


class WindowBinder {
public:
    WindowBinder(std::string window_name, std::string class_name);
    ~WindowBinder();

    void create_overlay();



    static LRESULT CALLBACK m_win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    struct window_rect: public RECT{
        int width(){
            return right - left;
        }
        int height(){
            return bottom - top;
        }
    };

public:
    std::string window_name;
    std::string class_name;

    HWND target_hwnd;
    HWND overlay_hwnd;
    window_rect target_rect;
    window_rect overlay_rect;

};


#endif //GAMECHEATINGTOOL_WINDOWBINDER_H
