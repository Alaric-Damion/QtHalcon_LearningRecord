#include "stdafx.h"
#include "..\SunDirectDraw.h"


CSunDirectDraw::CSunDirectDraw()
{
}


CSunDirectDraw::~CSunDirectDraw()
{
}


//******************************************************************
//函数：InitDDraw()
//功能：初始化DirectDraw环境并实现其功能。包括：创建DirectDraw对象，
// 设置显示模式，创建主页面，输出文字。
//******************************************************************
BOOL CSunDirectDraw::InitDDraw(HWND a_hWnd)
{
    DDSURFACEDESC ddsd; //页面描述
    HDC hdc; //设备环境句柄

    //创建DirectCraw对象
    if ( DirectDrawCreate( NULL, &m_lpDD, NULL ) != DD_OK ) return FALSE;

    // 取得独占和全屏模式
    if ( m_lpDD->SetCooperativeLevel( a_hWnd,DDSCL_NORMAL) != DD_OK)
        return FALSE;

    //设置显示模式
    int nScreenX=::GetSystemMetrics(SM_CXSCREEN);
    int nScreenY=::GetSystemMetrics(SM_CYSCREEN);
    if(m_lpDD->SetDisplayMode(nScreenX,nScreenY,32)!=DD_OK) return FALSE;
    //if ( m_lpDD->SetDisplayMode( 640, 480, 8 ) != DD_OK) return FALSE;

    //填充主页面信息
    memset(&ddsd,0,sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddsd.dwBackBufferCount=1;
    //创建主页面对象
    if ( m_lpDD->CreateSurface( &ddsd, &m_lpDDSPrimary, NULL ) != DD_OK)  return FALSE;
    LPDIRECTDRAWCLIPPER pMyClipper;
    if(m_lpDD->CreateClipper(0, &pMyClipper, NULL)!=DD_OK) return FALSE;
    pMyClipper->SetHWnd(0, a_hWnd);  
    m_lpDDSPrimary->SetClipper(pMyClipper);
    //输出文字
    if ( m_lpDDSPrimary->GetDC(&hdc) != DD_OK) return FALSE;
    m_lpDDSPrimary->ReleaseDC(hdc);
    return TRUE;
}

//******************************************************************
//函数：FreeDDraw()
//功能：释放所有的DirectDraw对象。
//******************************************************************
void CSunDirectDraw::FreeDDraw( void )
{
    if( m_lpDD != NULL )
    {
        if( m_lpDDSPrimary != NULL )
        {
            m_lpDDSPrimary->Release();
            m_lpDDSPrimary = NULL;
        }
        m_lpDD->Release();
        m_lpDD = NULL;
    }
}
