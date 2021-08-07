#pragma once

#include "ddraw.h"
#pragma comment(lib,"ddraw.lib")

class CSunDirectDraw
{
public:
    CSunDirectDraw();
    ~CSunDirectDraw();

    BOOL InitDDraw(HWND a_hWnd);                        // 初始化 DD
    void FreeDDraw( void );                             // 销毁 DD
    HDC GetDC(){
        HDC hDC;
        if(m_lpDDSPrimary->GetDC(&hDC)!=DD_OK) return NULL;
        return hDC;
    }
    void ReleaseDC(HDC a_hDC){m_lpDDSPrimary->ReleaseDC(a_hDC);}
protected:
    LPDIRECTDRAW        m_lpDD;                         // DirectDraw对象
    LPDIRECTDRAWSURFACE m_lpDDSPrimary;                 // DirectDraw主页面
};

