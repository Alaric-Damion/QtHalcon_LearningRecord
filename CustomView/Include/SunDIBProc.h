
/********************************************************************
  ���ܣ�λͼ��������
  ���ߣ�÷�ĺ�
  ���ڣ�2003
  �汾��1.0
  ˵����
*********************************************************************/

#ifndef _sun_dibproc_h
#define _sun_dibproc_h

#include "SunDefine.h"

namespace sunLib{


// ******************************* ͼ������� *******************************
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

struct TPicRegion  //һ����ɫ�����������������ڲ�������
{
    TARGB32*    pdata;         //��ɫ�����׵�ַ
    long        byte_width;    //һ�����ݵ�������(�ֽڿ��)��
    //abs(byte_width)�п��ܴ��ڵ���width*sizeof(TARGB32);
    long        width;         //���ؿ��
    long        height;        //���ظ߶�
};

//��ô����һ����ĺ�������дΪ��
inline TARGB32& Pixels(const TPicRegion& pic,const long x,const long y){
    return ( (TARGB32*)((TUInt8*)pic.pdata+pic.byte_width*y) )[x];
}
// void PicZoom8(const TPicRegion& Dst,const TPicRegion& Src);
// void PicZoom24(const TPicRegion& Dst,const TPicRegion& Src);
// void PicZoom32(const TPicRegion& Dst,const TPicRegion& Src);
SUN_DLL_DEFINE void PicZoom8_Table(const TPicRegion& Dst,const TPicRegion& Src);
SUN_DLL_DEFINE void PicZoom24_Table(const TPicRegion& Dst,const TPicRegion& Src);
SUN_DLL_DEFINE void PicZoom32_Table(const TPicRegion& Dst,const TPicRegion& Src);

// ��ȡͼ�����ָ��
SUN_DLL_DEFINE byte* GetBits(LPSTR ADIB,int ALine=0);
SUN_DLL_DEFINE byte* GetBits(LPSTR ADIB,int x,int y);
// ���ͼ������
SUN_DLL_DEFINE bool FillDIB(LPSTR ADIB,byte AValue,CRect ARect=CRect(0,0,0,0));
// ���ͼ������
SUN_DLL_DEFINE bool ClearDIB(LPSTR ADIB);
// ���ͼ��Ĵ�С
SUN_DLL_DEFINE CRect GetDIBRect(LPSTR ADIB);
// ����ͼ������(Ҫ��ͼ��λ���ʹ�Сһ��)
SUN_DLL_DEFINE bool CopyDIBToDIB(LPSTR ASource,LPSTR ATarget);
// �������ͼ��Ĳ��ͼ��
SUN_DLL_DEFINE void GetDifferImage(LPSTR AFirst,LPSTR ASecond,LPSTR AResult);
// ����һ��λͼ����
SUN_DLL_DEFINE void CopyDIBRect(LPSTR ASourceDIB,CRect ARect,LPSTR ATargetDIB,int ADesX,int ADesY);
// ������Ļ��λͼ
SUN_DLL_DEFINE HBITMAP CopyScreenToBitmap(LPRECT lpRect);
// �����豸ͼ��λͼ
SUN_DLL_DEFINE HBITMAP CopyDCToBitmap(const HDC a_hDC,LPRECT lpRect);
// �������ֵ�λͼ��
SUN_DLL_DEFINE HBITMAP DrawTextToBitmap(CString AText,COLORREF AColor,int ASize,int AWidth,int AHeight);
// ����λͼ���ļ�
SUN_DLL_DEFINE BOOL SaveBitmapToFile(HBITMAP hBitmap , LPCTSTR lpFileName);
// ��ʾλͼ�ļ�
SUN_DLL_DEFINE BOOL DisplayBitmap(CDC* pDC,LPCTSTR lpszBitmap);
// ��ʾһ�� DIB λͼ (�ܴ��� 8��24��32 λ)��ע������������
SUN_DLL_DEFINE void DisplayDIB(LPSTR ADIB,CDC* ADC,int AX,int AY);
// ����һ�����ο�
SUN_DLL_DEFINE void DrawRectangle(CDC* hdc,int x1,int y1,int x2,int y2,COLORREF AColor,BOOL AFill=FALSE);
SUN_DLL_DEFINE void DrawRectangle(CDC* hdc,LPCRECT ARect,COLORREF AColor,BOOL AFill=FALSE);
// ���һ��λͼ��ͷ�ṹ
SUN_DLL_DEFINE BITMAPINFOHEADER GetBmpFileHeader(CString AFileName);


// ����Ϊ��������������֤��һ���汾�ṩ����ע�� //////////////////////////////

// ��ֵ������
SUN_DLL_DEFINE void DIBDoubleProc(LPSTR ADIB,LPSTR ATargetDIB,CRect ARect,byte AThresh);
// ����һ��ֵ���е����Ķ�ֵ��
SUN_DLL_DEFINE void OptimizeDbl(LPSTR ADIB,LPSTR ATargetDIB,CRect ARect,byte AThresh);
// �Ҷ�ͼ��ͶӰ
SUN_DLL_DEFINE void DIBProjectionGray(LPSTR ADIB,CArrayLong* AData,BOOL AGray=TRUE,CRect ARect=CRect(0,0,0,0),int AOrient=0);
// ����ͼ���Ƿ���ͬ
SUN_DLL_DEFINE int DIBMatchImage(LPSTR ASource,LPSTR ATarget);
// ��ģ��ͼ����Դͼ��Ĳ��
SUN_DLL_DEFINE double DiffSqrCount(LPSTR ASource, LPSTR AModel,int dx, int dy);
// ���ָ��ͼ����һ����ɫ��������
SUN_DLL_DEFINE _int64 GetImageSpecColorCount(LPSTR ADIB,COLORREF AColor);
// ���ָ��ͼ����������ɫֵ���ܺ�
SUN_DLL_DEFINE _int64 GetImageColorCount(LPSTR ADIB);





}

#endif