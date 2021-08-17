
/********************************************************************
  功能：位图处理函数库
  作者：梅文海
  日期：2003
  版本：1.0
  说明：
*********************************************************************/

#ifndef _sun_dibproc_h
#define _sun_dibproc_h

#include "SunDefine.h"

namespace sunLib{


// ******************************* 图像处理相关 *******************************
typedef unsigned char TUInt8; // [0..255]
struct TARGB8      //32 bit color
{
    TUInt8  gray;          // A is alpha
};
struct TARGB24      //32 bit color
{
    TUInt8  B,G,R;          // A is alpha
};
struct TARGB32      //32 bit color
{
    TUInt8  B,G,R,A;          // A is alpha
};

struct TPicRegion  //一块颜色数据区的描述，便于参数传递
{
    TARGB32*    pdata;         //颜色数据首地址
    long        byte_width;    //一行数据的物理宽度(字节宽度)；
    //abs(byte_width)有可能大于等于width*sizeof(TARGB32);
    long        width;         //像素宽度
    long        height;        //像素高度
};

//那么访问一个点的函数可以写为：
inline TARGB32& Pixels(const TPicRegion& pic,const long x,const long y){
    return ( (TARGB32*)((TUInt8*)pic.pdata+pic.byte_width*y) )[x];
}
// void PicZoom8(const TPicRegion& Dst,const TPicRegion& Src);
// void PicZoom24(const TPicRegion& Dst,const TPicRegion& Src);
// void PicZoom32(const TPicRegion& Dst,const TPicRegion& Src);
SUN_DLL_DEFINE void PicZoom8_Table(const TPicRegion& Dst,const TPicRegion& Src);
SUN_DLL_DEFINE void PicZoom24_Table(const TPicRegion& Dst,const TPicRegion& Src);
SUN_DLL_DEFINE void PicZoom32_Table(const TPicRegion& Dst,const TPicRegion& Src);

// 获取图像的行指针
SUN_DLL_DEFINE byte* GetBits(LPSTR ADIB,int ALine=0);
SUN_DLL_DEFINE byte* GetBits(LPSTR ADIB,int x,int y);
// 填充图像数据
SUN_DLL_DEFINE bool FillDIB(LPSTR ADIB,byte AValue,CRect ARect=CRect(0,0,0,0));
// 清空图像数据
SUN_DLL_DEFINE bool ClearDIB(LPSTR ADIB);
// 获得图像的大小
SUN_DLL_DEFINE CRect GetDIBRect(LPSTR ADIB);
// 复制图像数据(要求图像位数和大小一致)
SUN_DLL_DEFINE bool CopyDIBToDIB(LPSTR ASource,LPSTR ATarget);
// 获得两幅图像的差别图像
SUN_DLL_DEFINE void GetDifferImage(LPSTR AFirst,LPSTR ASecond,LPSTR AResult);
// 复制一个位图数据
SUN_DLL_DEFINE void CopyDIBRect(LPSTR ASourceDIB,CRect ARect,LPSTR ATargetDIB,int ADesX,int ADesY);
// 拷贝屏幕到位图
SUN_DLL_DEFINE HBITMAP CopyScreenToBitmap(LPRECT lpRect);
// 拷贝设备图像到位图
SUN_DLL_DEFINE HBITMAP CopyDCToBitmap(const HDC a_hDC,LPRECT lpRect);
// 绘制文字到位图上
SUN_DLL_DEFINE HBITMAP DrawTextToBitmap(CString AText,COLORREF AColor,int ASize,int AWidth,int AHeight);
// 保存位图到文件
SUN_DLL_DEFINE BOOL SaveBitmapToFile(HBITMAP hBitmap , LPCTSTR lpFileName);
// 显示位图文件
SUN_DLL_DEFINE BOOL DisplayBitmap(CDC* pDC,LPCTSTR lpszBitmap);
// 显示一个 DIB 位图 (能处理 8、24和32 位)　注：仅用于试验
SUN_DLL_DEFINE void DisplayDIB(LPSTR ADIB,CDC* ADC,int AX,int AY);
// 绘制一个矩形框
SUN_DLL_DEFINE void DrawRectangle(CDC* hdc,int x1,int y1,int x2,int y2,COLORREF AColor,BOOL AFill=FALSE);
SUN_DLL_DEFINE void DrawRectangle(CDC* hdc,LPCRECT ARect,COLORREF AColor,BOOL AFill=FALSE);
// 获得一个位图的头结构
SUN_DLL_DEFINE BITMAPINFOHEADER GetBmpFileHeader(CString AFileName);


// 以下为保留函数，不保证下一个版本提供，请注意 //////////////////////////////

// 二值化处理
SUN_DLL_DEFINE void DIBDoubleProc(LPSTR ADIB,LPSTR ATargetDIB,CRect ARect,byte AThresh);
// 根据一列值进行调整的二值化
SUN_DLL_DEFINE void OptimizeDbl(LPSTR ADIB,LPSTR ATargetDIB,CRect ARect,byte AThresh);
// 灰度图像投影
SUN_DLL_DEFINE void DIBProjectionGray(LPSTR ADIB,CArrayLong* AData,BOOL AGray=TRUE,CRect ARect=CRect(0,0,0,0),int AOrient=0);
// 两幅图像是否相同
SUN_DLL_DEFINE int DIBMatchImage(LPSTR ASource,LPSTR ATarget);
// 求模板图像在源图像的差方和
SUN_DLL_DEFINE double DiffSqrCount(LPSTR ASource, LPSTR AModel,int dx, int dy);
// 获得指定图像中一个颜色的总数和
SUN_DLL_DEFINE _int64 GetImageSpecColorCount(LPSTR ADIB,COLORREF AColor);
// 获得指定图像中所有颜色值的总和
SUN_DLL_DEFINE _int64 GetImageColorCount(LPSTR ADIB);





}

#endif