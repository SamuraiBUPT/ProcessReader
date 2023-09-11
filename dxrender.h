//
// Created by D on 2023-08-05.
//

#include "windowbinder.h"
#include <string>
#include <d3dx9.h>
#include <d3dx9core.h>

#ifndef GAMECHEATINGTOOL_DXRENDER_H
#define GAMECHEATINGTOOL_DXRENDER_H

class dxRender{
public:
    IDirect3D9 *m_d3d = nullptr;
    IDirect3DDevice9 *m_device = nullptr;
    bool m_initialize = false;

    dxRender(HWND hwnd_wnd, WindowBinder wnd);			//��Ҫ��Ⱦ�Ĵ���  ���������Ľӿ�
    ~dxRender();
    void init();

    void begin_render();
    void end_render();
    void draw_rect(float x0, float y0, float width, float height, unsigned long color);
    void draw_line(float x0, float y0, float x1, float y1, unsigned long color);
    void draw_text(std::string text, int x0, int y0, unsigned long color);
private:

    ID3DXFont *m_font = nullptr;
    ID3DXLine *m_line = nullptr;
};

#endif //GAMECHEATINGTOOL_DXRENDER_H
