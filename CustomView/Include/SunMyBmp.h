
/*****************************************************************************
// 功能：DIB 图像操作类
// 作者：梅文海
// 日期：2003
// 版本：1.0
// 说明：该模块为自由版本，可任意使用，转载请一同附上本说明信息
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
    // 功能：图像转换类
    // 支持的格式：
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

        // 从文件读取
        bool LoadFromFile(CString a_sFileName);
        // 从文件读取
        bool LoadFromFile(CString a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);
        // 从内存读取
        bool LoadFromMemory(SunMemoryStream* a_pMemory);
        bool LoadFromMemory(byte* a_pBuffer,__int64 a_iSize);

        // 保存到内存
        bool SaveToMemory(SunMemoryStream* a_pMemory,CString a_sFormat,UINT a_iQuality);
    // 保存到内存(需要自行销毁内存)
    bool SaveToMemory(CString a_sFormat,UINT a_iQuality,OUT byte** a_pBuffer,OUT __int64* a_iSize);
        // 保存为文件
        bool SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality);
        // 保存为文件
        bool SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);
        // 获得图像对象
        Bitmap* GetImage(){return m_pImage;}
    // 从 CBMP 读取图像数据
    bool LoadFromBMPObject(CBMP* a_pBMP);
        // 将图像存入 BMP 对象
        bool SaveToBMPObject(CBMP* a_pBMP);
        // 显示图像
        void DrawImage(HDC a_hDC);
    protected:
        Bitmap*     m_pImage;         // GDI+ 图像对象
    };


    class SUN_DLL_DEFINE CBMP 
    {
    protected:    // 内部使用
        HDC         m_hScrDC, m_hMemDC;         // 屏幕和内存设备描述表
        HBITMAP     m_hOldBitmap;               // 旧的位图句柄
        void _ReadyGDIInfo();                   // 准备绘图信息
        void _RevertGDIInfo();                  // 还原绘图信息

    protected:
        HDIB        m_hDIB;
        CPalette*   m_palDIB;
        HBITMAP     m_hBmp;

        // 创建位图资源
        //BOOL CreateBitmap(HDC hdc,int nWidth,int nHeight);
        // 释放 GDI 资源
        BOOL DeleteGDIObject(HGDIOBJ AObj);
        // 转换 DDB 为一个 DIB 位图
        LPSTR BitmapToDIB(const int a_iBits);
    public:
        CBMP();
        ~CBMP();

        // 拷贝一个 CBMP 对象
        bool Assign(CBMP& a_bmp);
        // 释放 DIB 占用的内存
        void FreeDIB();
        __int64 GetDIBCountByte();
        // 获得颜色深度(即图像的位数)
        WORD GetBitCount(){return DIBBitCount(GetDIB());};
        // 获得图像的数据指针
        byte* GetImageAddress(){return (byte*)FindDIBBits(GetDIB());};
        // 重设图像的大小
        BOOL ReSize(LPSTR* ADIB,int AWidth,int AHeight,byte ABitmapBit);
        // 清空图像数据
        void ClearImage();
        // 获取图像的行指针
        byte* GetBits(int ALine=0){return sunLib::GetBits(GetDIB(),ALine);};
        // 获取一个像素的指针
        byte* GetBits(int x,int y){return sunLib::GetBits(GetDIB(),x,y);};

        HDIB GetHDIB()const {return m_hDIB;}
        char** GetHDIBAdd(){return (char**)&m_hDIB;};
        // 获得 DIB 位图指针
        LPSTR GetDIB();
        CPalette* GetPalette()const {return m_palDIB;};
        // 取得屏幕的图像数据
        LPSTR GetScreenRect(CRect a_rRect,const int a_iBits);
        // 获取指定设备的图像数据
        LPSTR GetDCRect(const HDC a_hDC,CRect a_rRect);
        // 取得文本图像数据
        LPSTR GetTextImage(CString AText,COLORREF AColor,int ASize,int AWidth,int AHeight);

        // 填充图像
        void FillImage(COLORREF AFillColor);
        // 绘制多边形
        LPSTR DrawPolygon(POINT* APoint,int ACount,CPoint AOffset,COLORREF AColor,COLORREF AFillColor,
            bool AFill=true,int APenWidth=1);
        // 绘制线段
        void DrawLine(CPoint APoint1,CPoint APoint2,COLORREF AColor,int AWidth=1);
        // 显示该位图
        void DisplayToScreen(CDC* ADC,int xDest,int yDest,CRect ASrc);
        // 从图像文件中读取数据
        LPSTR ReadDIBFile(CString AFileName);
        LPSTR ReadDIBFile(CString AFileName,CRect ARect);
        LPSTR ReadDIBOfBuffer(const byte* a_pBuffer,const __int64 a_iSize);
        // 保存数据到图像文件
        BOOL SaveToFile(CString AFileName);
        BOOL SaveToBuffer(OUT byte** a_pBuffer,OUT __int64* a_iSize);
        // 获得图像的宽度
        long GetWidth(){return DIBWidth(GetDIB());};
        // 获得图像的高度
        long GetHeight(){return DIBHeight(GetDIB());};
        // 获得图像的大小
        CRect GetRect(){return CRect(0,0,GetWidth(),GetHeight());};
        // 设置调色板
        void SetPalette(int ABitCount);
        // 开始绘制
        void BeginDraw(){_ReadyGDIInfo();};
        // 结束绘制
        void EndDraw(){_RevertGDIInfo();BitmapToDIB(GetBitCount());};
        // 获得兼容设备句柄(需要先调用 BeginDraw)
        HDC GetMemDC(){return m_hMemDC;}
        // 无损图像旋转(只支持 90,180和270度的旋转) 2017.6.2
        void RotateImage(int a_iAngle);
        // 翻转图像
        void MirrorImage(bool a_bHorz,bool a_bVert);
        // 从数据包获取图像
        BOOL LoadImageFromPack(CPackTreeBase* a_pPack,CString a_sImageName);
        // 将DIB位图转换为标准图像 2016.2.26
        void TranDIB2LineImage(OUT byte** a_pOutImage);
        // 将标准图像转换为 DIB 位图 2016.2.26
        void TranLineImage2DIB(byte* a_pImage,int a_iWidth,int a_iHeight,int a_iBit);
        // 从 HBITMAP 获得一张图像
        LPSTR LoadImageFromHBITMAP(HBITMAP a_hBit,const int a_iBits);
        // 从文件加载任意类型的图像文件（bmp,jpeg,gif,tiff,png等）
        bool LoadImage(CString a_sImageFile);
        // 加载缩略图
        CSize LoadThumbnailImage(CString a_sImageFile,const int a_iWidth,const int a_iHeight);
        CSize LoadThumbnailImage(CString a_sImageFile,const double a_fHScale,const double a_fVScale);
        // 保存到内存
        bool SaveToMemory(SunMemoryStream* a_pMemory,CString a_sFormat,UINT a_iQuality);
        // 保存到内存(需要自行销毁内存)
        bool SaveToMemory(CString a_sFormat,UINT a_iQuality,OUT byte** a_pBuffer,OUT __int64* a_iSize);
        // 保存为文件
        bool SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality);
        // 从内存加载任意类型的图像文件
        bool LoadImageFromMemory(byte* a_pBuffer,__int64 a_iSize);
        // 缩放图片
        void ZoomImage(CBMP* a_pSource,int a_iWidth,int a_iHeight);
        void ZoomImage(CBMP* a_pSource,double a_fHScale,double a_fVScale);

        // 高速抓图代码 --------------------------------------------------------------
    private:
        HDC         m_hScreenDC;            // 屏幕设备
        CSize       m_szScreenSize;         // 屏幕大小
    public:
        // 初始化屏幕相关代码
        void InitScreen();
        // 获取屏幕图像
        void QuickGetScreenImage(CRect a_rRect,const int a_iBits);
        // 释放屏幕相关资源
        void DestroyScreen();
    };

}
#endif // !defined(AFX_MYBMP_H__34151075_C57B_11D1_94F8_0000B431BBA1__INCLUDED_)
