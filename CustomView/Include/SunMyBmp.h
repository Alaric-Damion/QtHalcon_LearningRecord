
/*****************************************************************************
// ���ܣ�DIB ͼ�������
// ���ߣ�÷�ĺ�
// ���ڣ�2003
// �汾��1.0
// ˵������ģ��Ϊ���ɰ汾��������ʹ�ã�ת����һͬ���ϱ�˵����Ϣ
*****************************************************************************/

#if !defined(AFX_MYBMP_H__34151075_C57B_11D1_94F8_0000B431BBA1__INCLUDED_)
#define AFX_MYBMP_H__34151075_C57B_11D1_94F8_0000B431BBA1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include "DIBapi.h"
#include "SunDefine.h"
#include "SunDIBapi.h"
#include "SunDIBProc.h"
#include "TreePackInterface.h"
#include "SunMemory.h"
#pragma comment( lib, "gdiplus.lib" )
#include "gdiplus.h"
using namespace Gdiplus;


//using namespace sunLib

namespace sunLib {

    class SUN_DLL_DEFINE CGDIPlusInit
    {
    public:
        CGDIPlusInit()        {
            gdiplusToken=NULL;
            Init();
        }
//         CGDIPlusInit(bool a_bInit)        {
//             gdiplusToken=NULL;
//             if(a_bInit) Init();
//         }
        ~CGDIPlusInit(void)        {
            if(gdiplusToken) GdiplusShutdown(gdiplusToken);
        }
    protected:
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR           gdiplusToken;
        void Init(){
            GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        }
    };


    class CBMP;
    //////////////////////////////////////////////////////////////////////////
    // ���ܣ�ͼ��ת����
    // ֧�ֵĸ�ʽ��
    //     bmp 
    //     jpeg 
    //     gif 
    //     tiff 
    //     png 
    //////////////////////////////////////////////////////////////////////////
    class SUN_DLL_DEFINE CImageConvert
    {
    public:
        CImageConvert(void);
        ~CImageConvert(void);

        // ���ļ���ȡ
        bool LoadFromFile(CString a_sFileName);
        // ���ļ���ȡ
        bool LoadFromFile(CString a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);
        // ���ڴ��ȡ
        bool LoadFromMemory(SunMemoryStream* a_pMemory);
        bool LoadFromMemory(byte* a_pBuffer,__int64 a_iSize);

        // ���浽�ڴ�
        bool SaveToMemory(SunMemoryStream* a_pMemory,CString a_sFormat,UINT a_iQuality);
    // ���浽�ڴ�(��Ҫ���������ڴ�)
    bool SaveToMemory(CString a_sFormat,UINT a_iQuality,OUT byte** a_pBuffer,OUT __int64* a_iSize);
        // ����Ϊ�ļ�
        bool SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality);
        // ����Ϊ�ļ�
        bool SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);
        // ���ͼ�����
        Bitmap* GetImage(){return m_pImage;}
    // �� CBMP ��ȡͼ������
    bool LoadFromBMPObject(CBMP* a_pBMP);
        // ��ͼ����� BMP ����
        bool SaveToBMPObject(CBMP* a_pBMP);
        // ��ʾͼ��
        void DrawImage(HDC a_hDC);
    protected:
        Bitmap*     m_pImage;         // GDI+ ͼ�����
    };


    class SUN_DLL_DEFINE CBMP 
    {
    protected:    // �ڲ�ʹ��
        HDC         m_hScrDC, m_hMemDC;         // ��Ļ���ڴ��豸������
        HBITMAP     m_hOldBitmap;               // �ɵ�λͼ���
        void _ReadyGDIInfo();                   // ׼����ͼ��Ϣ
        void _RevertGDIInfo();                  // ��ԭ��ͼ��Ϣ

    protected:
        HDIB        m_hDIB;
        CPalette*   m_palDIB;
        HBITMAP     m_hBmp;

        // ����λͼ��Դ
        //BOOL CreateBitmap(HDC hdc,int nWidth,int nHeight);
        // �ͷ� GDI ��Դ
        BOOL DeleteGDIObject(HGDIOBJ AObj);
        // ת�� DDB Ϊһ�� DIB λͼ
        LPSTR BitmapToDIB(const int a_iBits);
    public:
        CBMP();
        ~CBMP();

        // ����һ�� CBMP ����
        bool Assign(CBMP& a_bmp);
        // �ͷ� DIB ռ�õ��ڴ�
        void FreeDIB();
        __int64 GetDIBCountByte();
        // �����ɫ���(��ͼ���λ��)
        WORD GetBitCount(){return DIBBitCount(GetDIB());};
        // ���ͼ�������ָ��
        byte* GetImageAddress(){return (byte*)FindDIBBits(GetDIB());};
        // ����ͼ��Ĵ�С
        BOOL ReSize(LPSTR* ADIB,int AWidth,int AHeight,byte ABitmapBit);
        // ���ͼ������
        void ClearImage();
        // ��ȡͼ�����ָ��
        byte* GetBits(int ALine=0){return sunLib::GetBits(GetDIB(),ALine);};
        // ��ȡһ�����ص�ָ��
        byte* GetBits(int x,int y){return sunLib::GetBits(GetDIB(),x,y);};

        HDIB GetHDIB()const {return m_hDIB;}
        char** GetHDIBAdd(){return (char**)&m_hDIB;};
        // ��� DIB λͼָ��
        LPSTR GetDIB();
        CPalette* GetPalette()const {return m_palDIB;};
        // ȡ����Ļ��ͼ������
        LPSTR GetScreenRect(CRect a_rRect,const int a_iBits);
        // ��ȡָ���豸��ͼ������
        LPSTR GetDCRect(const HDC a_hDC,CRect a_rRect);
        // ȡ���ı�ͼ������
        LPSTR GetTextImage(CString AText,COLORREF AColor,int ASize,int AWidth,int AHeight);

        // ���ͼ��
        void FillImage(COLORREF AFillColor);
        // ���ƶ����
        LPSTR DrawPolygon(POINT* APoint,int ACount,CPoint AOffset,COLORREF AColor,COLORREF AFillColor,
            bool AFill=true,int APenWidth=1);
        // �����߶�
        void DrawLine(CPoint APoint1,CPoint APoint2,COLORREF AColor,int AWidth=1);
        // ��ʾ��λͼ
        void DisplayToScreen(CDC* ADC,int xDest,int yDest,CRect ASrc);
        // ��ͼ���ļ��ж�ȡ����
        LPSTR ReadDIBFile(CString AFileName);
        LPSTR ReadDIBFile(CString AFileName,CRect ARect);
        LPSTR ReadDIBOfBuffer(const byte* a_pBuffer,const __int64 a_iSize);
        // �������ݵ�ͼ���ļ�
        BOOL SaveToFile(CString AFileName);
        BOOL SaveToBuffer(OUT byte** a_pBuffer,OUT __int64* a_iSize);
        // ���ͼ��Ŀ��
        long GetWidth(){return DIBWidth(GetDIB());};
        // ���ͼ��ĸ߶�
        long GetHeight(){return DIBHeight(GetDIB());};
        // ���ͼ��Ĵ�С
        CRect GetRect(){return CRect(0,0,GetWidth(),GetHeight());};
        // ���õ�ɫ��
        void SetPalette(int ABitCount);
        // ��ʼ����
        void BeginDraw(){_ReadyGDIInfo();};
        // ��������
        void EndDraw(){_RevertGDIInfo();BitmapToDIB(GetBitCount());};
        // ��ü����豸���(��Ҫ�ȵ��� BeginDraw)
        HDC GetMemDC(){return m_hMemDC;}
        // ����ͼ����ת(ֻ֧�� 90,180��270�ȵ���ת) 2017.6.2
        void RotateImage(int a_iAngle);
        // ��תͼ��
        void MirrorImage(bool a_bHorz,bool a_bVert);
        // �����ݰ���ȡͼ��
        BOOL LoadImageFromPack(CPackTreeBase* a_pPack,CString a_sImageName);
        // ��DIBλͼת��Ϊ��׼ͼ�� 2016.2.26
        void TranDIB2LineImage(OUT byte** a_pOutImage);
        // ����׼ͼ��ת��Ϊ DIB λͼ 2016.2.26
        void TranLineImage2DIB(byte* a_pImage,int a_iWidth,int a_iHeight,int a_iBit);
        // �� HBITMAP ���һ��ͼ��
        LPSTR LoadImageFromHBITMAP(HBITMAP a_hBit,const int a_iBits);
        // ���ļ������������͵�ͼ���ļ���bmp,jpeg,gif,tiff,png�ȣ�
        bool LoadImage(CString a_sImageFile);
        // ��������ͼ
        CSize LoadThumbnailImage(CString a_sImageFile,const int a_iWidth,const int a_iHeight);
        CSize LoadThumbnailImage(CString a_sImageFile,const double a_fHScale,const double a_fVScale);
        // ���浽�ڴ�
        bool SaveToMemory(SunMemoryStream* a_pMemory,CString a_sFormat,UINT a_iQuality);
        // ���浽�ڴ�(��Ҫ���������ڴ�)
        bool SaveToMemory(CString a_sFormat,UINT a_iQuality,OUT byte** a_pBuffer,OUT __int64* a_iSize);
        // ����Ϊ�ļ�
        bool SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality);
        // ���ڴ�����������͵�ͼ���ļ�
        bool LoadImageFromMemory(byte* a_pBuffer,__int64 a_iSize);
        // ����ͼƬ
        void ZoomImage(CBMP* a_pSource,int a_iWidth,int a_iHeight);
        void ZoomImage(CBMP* a_pSource,double a_fHScale,double a_fVScale);

        // ����ץͼ���� --------------------------------------------------------------
    private:
        HDC         m_hScreenDC;            // ��Ļ�豸
        CSize       m_szScreenSize;         // ��Ļ��С
    public:
        // ��ʼ����Ļ��ش���
        void InitScreen();
        // ��ȡ��Ļͼ��
        void QuickGetScreenImage(CRect a_rRect,const int a_iBits);
        // �ͷ���Ļ�����Դ
        void DestroyScreen();
    };

}
#endif // !defined(AFX_MYBMP_H__34151075_C57B_11D1_94F8_0000B431BBA1__INCLUDED_)
