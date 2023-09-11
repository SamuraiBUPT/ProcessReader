//
// Created by D on 2023-08-05.
//
#include "dxrender.h"
#include <stdexcept>

using namespace std;
dxRender::dxRender(HWND hwnd_wnd, WindowBinder wnd)
{
    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);  //�õ��ʴ�

    D3DPRESENT_PARAMETERS pp;

    ZeroMemory(&pp, sizeof pp);

    pp.Windowed = true;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferFormat = D3DFMT_A8R8G8B8;

    // get rectangle
    pp.BackBufferWidth = wnd.target_rect.width();
    pp.BackBufferHeight = wnd.target_rect.height();

    pp.hDeviceWindow = hwnd_wnd;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


    if ((m_d3d->CreateDevice(D3DADAPTER_DEFAULT,
                             D3DDEVTYPE_HAL,
                             hwnd_wnd,
                             D3DCREATE_HARDWARE_VERTEXPROCESSING,
                             &pp,
                             &m_device
    )) != D3D_OK)throw std::runtime_error("My_device create failed");
    m_initialize = true;
}

dxRender::~dxRender()
{
    if (m_line)m_line->Release();
    if (m_font)m_font->Release();
    m_d3d = nullptr;
    m_device = nullptr;
}

void dxRender::init()
{
    if (m_device == nullptr) {
        throw std::runtime_error("m_device missing");
    }
    if ((D3DXCreateLine(m_device, &m_line)) != D3D_OK) {
        throw std::runtime_error("create line failed");
    }
    if ((D3DXCreateFont(m_device, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL,
                        NULL, NULL, "Tahoma",
                        &m_font)) != S_OK) {
        if (m_device == nullptr)throw std::runtime_error("create font failed");
    }
}

void dxRender::begin_render()
{
    m_device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
    //��ʼһ��������
    m_device->BeginScene();
}

void dxRender::end_render()
{
    //����ͨ������ IDirect3DDevice9::BeginScene ��ʼ�ĳ�����
    m_device->EndScene();
    m_device->Present(nullptr, nullptr, nullptr, nullptr);
}

void dxRender::draw_rect(float x0, float y0, float width, float height, unsigned long color)
{
    draw_line(x0, y0, x0 + width, y0, color);//��һ������
    draw_line(x0, y0, x0, y0 + height, color);//����
    draw_line(x0, y0 + height, x0 + width, y0 + height, color);
    draw_line(x0 + width, y0, x0 + width, y0 + height, color);
}

void dxRender::draw_line(float x0, float y0, float x1, float y1, unsigned long color)
{
    //std::cout << x0 << " " << y0 << " " << x1 << " " << y1 << " " << std::endl;
    D3DXVECTOR2 line[2] = { D3DXVECTOR2(x0,y0),D3DXVECTOR2(x1,y1) };//�����Ļ������ͻ����յ�
    //std::cout << line[0].x << " " << line[0].y << " " << line[1].x << " " << line[1].y << std::endl;
    m_line->Begin();
    HRESULT receiver = m_line->Draw(line, 2, color);
    if (receiver == D3DERR_INVALIDCALL)throw std::runtime_error("D3DERR_INVALIDCALL");
    if (receiver == D3DXERR_INVALIDDATA)throw std::runtime_error("D3DXERR_INVALIDDATA");
    m_line->End();
}

void dxRender::draw_text(std::string text, int x0, int y0, unsigned long color)
{
    RECT r{ x0,y0,x0,y0 };
    m_font->DrawTextA(nullptr, text.data(), -1, &r, DT_NOCLIP, color);
}