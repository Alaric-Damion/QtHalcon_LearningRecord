#include "stdafx.h"
#include "..\SunDirectDraw.h"


CSunDirectDraw::CSunDirectDraw()
{
}


CSunDirectDraw::~CSunDirectDraw()
{
}


//******************************************************************
//������InitDDraw()
//���ܣ���ʼ��DirectDraw������ʵ���书�ܡ�����������DirectDraw����
// ������ʾģʽ��������ҳ�棬������֡�
//******************************************************************
BOOL CSunDirectDraw::InitDDraw(HWND a_hWnd)
{
    DDSURFACEDESC ddsd; //ҳ������
    HDC hdc; //�豸�������

    //����DirectCraw����
    if ( DirectDrawCreate( NULL, &m_lpDD, NULL ) != DD_OK ) return FALSE;

    // ȡ�ö�ռ��ȫ��ģʽ
    if ( m_lpDD->SetCooperativeLevel( a_hWnd,DDSCL_NORMAL) != DD_OK)
        return FALSE;

    //������ʾģʽ
    int nScreenX=::GetSystemMetrics(SM_CXSCREEN);
    int nScreenY=::GetSystemMetrics(SM_CYSCREEN);
    if(m_lpDD->SetDisplayMode(nScreenX,nScreenY,32)!=DD_OK) return FALSE;
    //if ( m_lpDD->SetDisplayMode( 640, 480, 8 ) != DD_OK) return FALSE;

    //�����ҳ����Ϣ
    memset(&ddsd,0,sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddsd.dwBackBufferCount=1;
    //������ҳ�����
    if ( m_lpDD->CreateSurface( &ddsd, &m_lpDDSPrimary, NULL ) != DD_OK)  return FALSE;
    LPDIRECTDRAWCLIPPER pMyClipper;
    if(m_lpDD->CreateClipper(0, &pMyClipper, NULL)!=DD_OK) return FALSE;
    pMyClipper->SetHWnd(0, a_hWnd);  
    m_lpDDSPrimary->SetClipper(pMyClipper);
    //�������
    if ( m_lpDDSPrimary->GetDC(&hdc) != DD_OK) return FALSE;
    m_lpDDSPrimary->ReleaseDC(hdc);
    return TRUE;
}

//******************************************************************
//������FreeDDraw()
//���ܣ��ͷ����е�DirectDraw����
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
