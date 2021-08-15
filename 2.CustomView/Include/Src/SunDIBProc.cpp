
#include "stdafx.h"
#include "SunDIBProc.h"
#include "SunDIBApi.h"


namespace sunLib{

void PicZoom8_Table(const TPicRegion& Dst,const TPicRegion& Src)
{
    if (  (0==Dst.width)||(0==Dst.height)
        ||(0==Src.width)||(0==Src.height)) return;
    unsigned long dst_width=Dst.width;
    unsigned long* SrcX_Table = new unsigned long[dst_width];
    for (unsigned long x=0;x<dst_width;++x)//生成表 SrcX_Table
    {
        SrcX_Table[x]=(x*Src.width/Dst.width);
    }

    TARGB8* pDstLine=(TARGB8*)Dst.pdata;
    for (int y=0;y<Dst.height;++y)
    {
        unsigned long srcy=(y*Src.height/Dst.height);
        TARGB8* pSrcLine=((TARGB8*)((TUInt8*)Src.pdata+Src.byte_width*srcy));
        for (unsigned long x=0;x<dst_width;++x)
            pDstLine[x]=pSrcLine[SrcX_Table[x]];
        ((TUInt8*&)pDstLine)+=Dst.byte_width;
    }

    delete [] SrcX_Table;
}
void PicZoom24_Table(const TPicRegion& Dst,const TPicRegion& Src)
{
    if (  (0==Dst.width)||(0==Dst.height)
        ||(0==Src.width)||(0==Src.height)) return;
    unsigned long dst_width=Dst.width;
    unsigned long* SrcX_Table = new unsigned long[dst_width];
    for (unsigned long x=0;x<dst_width;++x)//生成表 SrcX_Table
    {
        SrcX_Table[x]=(x*Src.width/Dst.width);
    }

    TARGB24* pDstLine=(TARGB24*)Dst.pdata;
    for (int y=0;y<Dst.height;++y)
    {
        unsigned long srcy=(y*Src.height/Dst.height);
        TARGB24* pSrcLine=((TARGB24*)((TUInt8*)Src.pdata+Src.byte_width*srcy));
        for (unsigned long x=0;x<dst_width;++x)
            pDstLine[x]=pSrcLine[SrcX_Table[x]];
        ((TUInt8*&)pDstLine)+=Dst.byte_width;
    }

    delete [] SrcX_Table;
}
void PicZoom32_Table(const TPicRegion& Dst,const TPicRegion& Src)
{
    if (  (0==Dst.width)||(0==Dst.height)
        ||(0==Src.width)||(0==Src.height)) return;
    unsigned long dst_width=Dst.width;
    unsigned long* SrcX_Table = new unsigned long[dst_width];
    for (unsigned long x=0;x<dst_width;++x)//生成表 SrcX_Table
    {
        SrcX_Table[x]=(x*Src.width/Dst.width);
    }

    TARGB32* pDstLine=Dst.pdata;
    for (int y=0;y<Dst.height;++y)
    {
        unsigned long srcy=(y*Src.height/Dst.height);
        TARGB32* pSrcLine=((TARGB32*)((TUInt8*)Src.pdata+Src.byte_width*srcy));
        for (unsigned long x=0;x<dst_width;++x)
            pDstLine[x]=pSrcLine[SrcX_Table[x]];
        ((TUInt8*&)pDstLine)+=Dst.byte_width;
    }

    delete [] SrcX_Table;
}



// void PicZoom8(const TPicRegion& Dst,const TPicRegion& Src)
// {
//     if (  (0==Dst.width)||(0==Dst.height)
//         ||(0==Src.width)||(0==Src.height)) return;
//     unsigned long xrIntFloat_16=(Src.width<<16)/Dst.width+1; 
//     unsigned long yrIntFloat_16=(Src.height<<16)/Dst.height+1;
//     unsigned long dst_width=Dst.width;
//     TARGB8* pDstLine=(TARGB8*)Dst.pdata;
//     unsigned long srcy_16=0;
//     for (unsigned long y=0;y<Dst.height;++y)
//     {
//         TARGB8* pSrcLine=((TARGB8*)((TUInt8*)Src.pdata+Src.byte_width*(srcy_16>>16)));
//         unsigned long srcx_16=0;
//         for (unsigned long x=0;x<dst_width;++x)
//         {
//             pDstLine[x]=pSrcLine[srcx_16>>16];
//             srcx_16+=xrIntFloat_16;
//         }
//         srcy_16+=yrIntFloat_16;
//         ((TUInt8*&)pDstLine)+=Dst.byte_width;
//     }
// }
// 
// void PicZoom24(const TPicRegion& Dst,const TPicRegion& Src)
// {
//     if (  (0==Dst.width)||(0==Dst.height)
//         ||(0==Src.width)||(0==Src.height)) return;
//     unsigned long xrIntFloat_16=(Src.width<<16)/Dst.width+1; 
//     unsigned long yrIntFloat_16=(Src.height<<16)/Dst.height+1;
//     unsigned long dst_width=Dst.width;
//     TARGB24* pDstLine=(TARGB24*)Dst.pdata;
//     unsigned long srcy_16=0;
//     for (unsigned long y=0;y<Dst.height;++y)
//     {
//         TARGB24* pSrcLine=((TARGB24*)((TUInt8*)Src.pdata+Src.byte_width*(srcy_16>>16)));
//         unsigned long srcx_16=0;
//         for (unsigned long x=0;x<dst_width;++x)
//         {
//             pDstLine[x]=pSrcLine[srcx_16>>16];
//             srcx_16+=xrIntFloat_16;
//         }
//         srcy_16+=yrIntFloat_16;
//         ((TUInt8*&)pDstLine)+=Dst.byte_width;
//     }
// }
// 
// void PicZoom32(const TPicRegion& Dst,const TPicRegion& Src)
// {
//     if (  (0==Dst.width)||(0==Dst.height)
//         ||(0==Src.width)||(0==Src.height)) return;
//     unsigned long xrIntFloat_16=(Src.width<<16)/Dst.width+1; 
//     unsigned long yrIntFloat_16=(Src.height<<16)/Dst.height+1;
//     unsigned long dst_width=Dst.width;
//     TARGB32* pDstLine=Dst.pdata;
//     unsigned long srcy_16=0;
//     for (unsigned long y=0;y<Dst.height;++y)
//     {
//         TARGB32* pSrcLine=((TARGB32*)((TUInt8*)Src.pdata+Src.byte_width*(srcy_16>>16)));
//         unsigned long srcx_16=0;
//         for (unsigned long x=0;x<dst_width;++x)
//         {
//             pDstLine[x]=pSrcLine[srcx_16>>16];
//             srcx_16+=xrIntFloat_16;
//         }
//         srcy_16+=yrIntFloat_16;
//         ((TUInt8*&)pDstLine)+=Dst.byte_width;
//     }
// }


// 复制一个位图数据
// 2004.8.16
void CopyDIBRect(LPSTR ASourceDIB,CRect ARect,LPSTR ATargetDIB,int ADesX,int ADesY)
{
    int iPixelBytes=(BYTE)(DIBBitCount(ASourceDIB)/8);
    int iCopyLineBytes=ARect.Width()*iPixelBytes;
    for(int y=ARect.top;y<ARect.bottom;y++){
        memcpy(GetBits(ATargetDIB,ADesX,ADesY+(y-ARect.top)),GetBits(ASourceDIB,ARect.left,y),iCopyLineBytes);
    }
}

// 显示位图文件
// 2004.7.18
BOOL DisplayBitmap(CDC* pDC,LPCTSTR lpszBitmap)
{
    BITMAP		bmp;
    CBitmap		bitmap;
    CDC			dcView;
    //if(FileExists(lpszBitmap)) AfxMessageBox("找到文件!");
    //if(!bitmap.LoadBitmap(AfxGetInstanceHandle(),lpszBitmap)) return FALSE;
    HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,lpszBitmap,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
    
    bitmap.Attach(hBitmap);
    
    // 获得图像文件的信息
    bitmap.GetObject(sizeof(BITMAP),&bmp);
    
    if(!dcView.CreateCompatibleDC(pDC)) return FALSE;
    
    dcView.SelectObject(&bitmap);
    // 显示图像
    pDC->BitBlt(0,0,bmp.bmWidth,bmp.bmHeight,&dcView,0,0,SRCCOPY);
    return TRUE;
}

// 拷贝屏幕到位图
// 2004.7.24
HBITMAP CopyScreenToBitmap(LPRECT lpRect)
//lpRect 代表选定区域
{
    HDC       hScrDC;      
    // 确保选定区域不为空矩形
    if (IsRectEmpty(lpRect))
        return NULL;
    //为屏幕创建设备描述表
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    HBITMAP hImage=CopyDCToBitmap(hScrDC,lpRect);
    DeleteDC(hScrDC);
    // 返回位图句柄
    return hImage;
}

// 拷贝设备图像到位图
// 2010.11.29
HBITMAP CopyDCToBitmap(const HDC a_hDC,LPRECT lpRect)
//lpRect 代表选定区域
{
    //int iBits = GetDeviceCaps(a_hDC, BITSPIXEL) * GetDeviceCaps(a_hDC, PLANES);
    HDC       hScrDC, hMemDC;      
    // 屏幕和内存设备描述表
    HBITMAP    hBitmap, hOldBitmap;   
    // 位图句柄
    int       nX, nY, nX2, nY2;      
    // 选定区域坐标
    int       nWidth, nHeight;      
    // 位图宽度和高度
    int       xScrn, yScrn;         
    // 屏幕分辨率
    // 确保选定区域不为空矩形
    if (IsRectEmpty(lpRect))
        return NULL;
    //为屏幕创建设备描述表
    hScrDC = a_hDC;
    //为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);
    // 获得选定区域坐标
    nX = lpRect->left;
    nY = lpRect->top;
    nX2 = lpRect->right;
    nY2 = lpRect->bottom;
    // 获得屏幕分辨率
    xScrn = GetDeviceCaps(hScrDC, HORZRES);
    yScrn = GetDeviceCaps(hScrDC, VERTRES);
    //确保选定区域是可见的
    if (nX < 0)
        nX = 0;
    if (nY < 0)
        nY = 0;
    if (nX2 > xScrn)
        nX2 = xScrn;
    if (nY2 > yScrn)
        nY2 = yScrn;
    nWidth = nX2 - nX;
    nHeight = nY2 - nY;
    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
//     BITMAP bitMapInfo;
//     bitMapInfo.bmBitsPixel  =iBits;
//     bitMapInfo.bmBits       =0;
//     bitMapInfo.bmWidth      =nWidth;
//     bitMapInfo.bmHeight     =nHeight;
//     bitMapInfo.bmPlanes     =1;
//     bitMapInfo.bmType       =0;
//     bitMapInfo.bmWidthBytes =WIDTHBYTES(nWidth*iBits);
//     hBitmap = CreateBitmapIndirect(&bitMapInfo);
    if(!hBitmap) 
    {
        //        CString s;
        //        s.Format("创建兼容位图失败! 错误号:%d",GetLastError());
        //        AfxMessageBox(s);
        return NULL;
    }
    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
    //得到屏幕位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    //清除 
    DeleteDC(hMemDC);
    // 返回位图句柄
    return hBitmap;
}

// 绘制文字到位图上
// 2004.8.8
HBITMAP DrawTextToBitmap(CString AText,COLORREF AColor,int ASize,int AWidth,int AHeight)
{
    // 屏幕和内存设备描述表
    HDC       hScrDC, hMemDC;      
    // 位图句柄
    HBITMAP    hBitmap, hOldBitmap;   
    int       nWidth, nHeight;      
    //为屏幕创建设备描述表
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    //为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);

    nWidth = AWidth;
    nHeight = AHeight;
    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    //BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
    // 绘制文字
    CFont font;
    VERIFY(font.CreateFont(
        ASize,                     // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        _T("宋体")));                  // lpszFacename
    
    SelectObject(hMemDC,font);

    SetBkMode(hMemDC,TRANSPARENT);
    SetTextColor(hMemDC,AColor);
    TextOut(hMemDC,0, 0, AText, AText.GetLength());
    
    font.DeleteObject();
    
    //得到屏幕位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    //清除 
    DeleteDC(hScrDC);
    DeleteDC(hMemDC);
    // 返回位图句柄
    return hBitmap;
}

// 保存位图到文件
// 2004.7.24
BOOL SaveBitmapToFile(HBITMAP hBitmap , LPCTSTR lpFileName) //hBitmap 为刚才的屏幕位图句柄
{    	 //lpFileName 为位图文件名
	HDC            hDC;         
  	 //设备描述表
	int            iBits;      
	//当前显示分辨率下每个像素所占字节数
	__int64            wBitCount;   
   	//位图中每个像素所占字节数
	//定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数
    __int64  dwPaletteSize=0;
    DWORD  dwWritten,  dwBmBitsSize,   dwDIBSize;
	BITMAP          Bitmap;        
	//位图属性结构
	BITMAPFILEHEADER   bmfHdr;        
	//位图文件头结构
	BITMAPINFOHEADER   bi;            
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi;          
	//指向位图信息头结构
	HANDLE          fh, hDib;
    HPALETTE hPal,hOldPal=NULL;
	//定义文件，分配内存句柄，调色板句柄
	//计算位图文件每个像素所占字节数
	hDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * 
		GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)	        wBitCount = 1;
	else if (iBits <= 4)	wBitCount = 4;
	else if (iBits <= 8)    wBitCount = 8;
    else if (iBits <= 16)   wBitCount = 16;
	else if (iBits <= 24)   wBitCount = 24;
    else if (iBits <= 32)   wBitCount = 32;
	//计算调色板大小
	if (wBitCount <= 8)	dwPaletteSize = ((__int64)1 <<  wBitCount) *sizeof(RGBQUAD);
	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize               = sizeof(BITMAPINFOHEADER);
	bi.biWidth              = Bitmap.bmWidth;
	bi.biHeight             = Bitmap.bmHeight;
	bi.biPlanes             = 1;
	bi.biBitCount           = (WORD)wBitCount;
	bi.biCompression        = BI_RGB;
	bi.biSizeImage          = 0;
	bi.biXPelsPerMeter      = 0;
	bi.biYPelsPerMeter      = 0;
	bi.biClrUsed            = 0;
	bi.biClrImportant       = 0;
	dwBmBitsSize = ((Bitmap.bmWidth *(DWORD)wBitCount+31)/32)* 4*Bitmap.bmHeight ;
	//为位图内容分配内存
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板   
	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC  = GetDC(NULL);
		hOldPal = SelectPalette(hDC,hPal,FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	//创建位图文件    
	fh = CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)	return FALSE;
	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ (DWORD)dwPaletteSize + dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ (DWORD)dwPaletteSize;
	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

    return TRUE;
}

// 显示一个 DIB 位图 (能处理 8 位灰度、24和32 位)
void DisplayDIB(LPSTR ADIB,CDC* ADC,int AX,int AY)
{
    int         x,y,w,h;
    byte        byBit=(BYTE)(DIBBitCount(ADIB)/8);
    byte        r,g,b;
    byte        byGray;
    DWORD       dwColor=0;
    // 获得 DIB 的数据地址
    LPSTR lpDIBAddress=FindDIBBits(ADIB);
    UINT32 iAddress,iBeginAddress;
    
    w=DIBWidth(ADIB);
    h=DIBHeight(ADIB);
    //计算原图像每行的字节数
    LONG lLineBytes = WIDTHBYTES(w * byBit<<3);	
    for(y=0;y<h;y++)
    {
        iBeginAddress=lLineBytes*(h-y-1);
        for(x=0;x<w;x++)
        {
            iAddress=iBeginAddress+x*byBit;
            if(byBit==1) 
            {
                byGray=*(lpDIBAddress+iAddress);
                dwColor=RGB(byGray,byGray,byGray);
            }
            if(byBit==3 || byBit==4) {
                b=(BYTE)*(lpDIBAddress+iAddress);
                g=(BYTE)*(lpDIBAddress+iAddress+1);
                r=(BYTE)*(lpDIBAddress+iAddress+2);
                dwColor=RGB(r,g,b);
            }
            ADC->SetPixel(x+AX,y+AY,dwColor);
        }
    }
}

// 绘制一个矩形框
// 2004.7.29       HDC hdc
void DrawRectangle(CDC* hdc,int x1,int y1,int x2,int y2,COLORREF AColor,BOOL AFill)
{
    CPen pen,penOld;
    CBrush brush,brushOld;
    
    pen.CreatePen(PS_SOLID,1,AColor);
    if(AFill)
    {
        brush.CreateSolidBrush(AColor);
    }
    else
    {
        LOGBRUSH logBrush;
        logBrush.lbStyle=BS_NULL;
        logBrush.lbColor=AColor;
        brush.CreateBrushIndirect(&logBrush);
    }
    penOld.m_hObject=hdc->SelectObject(&pen);
    brushOld.m_hObject=hdc->SelectObject(&brush);
    // 绘制
    hdc->Rectangle(x1,y1,x2,y2);
    hdc->SelectObject(penOld);
    hdc->SelectObject(brushOld);
    brush.DeleteObject();
    pen.DeleteObject();
    penOld.DeleteObject();
    brushOld.DeleteObject();
}

void DrawRectangle(CDC* hdc,LPCRECT ARect,COLORREF AColor,BOOL AFill)
{
    DrawRectangle(hdc,ARect->left,ARect->top,ARect->right,ARect->bottom,AColor,AFill);
}

// 获取图像的行指针
byte* GetBits(LPSTR ADIB,int ALine)
{
    int w,h;
    int iBit=DIBBitCount(ADIB);
    LPSTR lpDIBAddress=FindDIBBits(ADIB);
    
    w=DIBWidth(ADIB);
    h=DIBHeight(ADIB);
    
    //计算原图像每行的字节数
    LONG lLineBytes = WIDTHBYTES(w * iBit);	
    
    return (byte*)(lpDIBAddress+lLineBytes*(h-1-ALine));
}

// 获取一个像素的指针
byte* GetBits(LPSTR ADIB,int x,int y)
{
    int w,h;
    int iBit=DIBBitCount(ADIB);
    int iPixelBytes=iBit/8;

    LPSTR lpDIBAddress=FindDIBBits(ADIB);
    
    w=DIBWidth(ADIB);
    h=DIBHeight(ADIB);
    
    //计算原图像每行的字节数
    LONG lLineBytes = WIDTHBYTES(w * iBit);	
    
    return (byte*)(lpDIBAddress+lLineBytes*(h-1-y)+x*iPixelBytes);
}

// 填充图像数据
// 2006.3.11
bool FillDIB(LPSTR ADIB,byte AValue,CRect ARect)
{
    int iHeight=DIBHeight(ADIB);
    int iWidth=DIBWidth(ADIB);
    CRect rRect(ARect);
    if(ARect.Width()<=0){
        rRect.left  =0;
        rRect.top   =0;
        rRect.right =iWidth;
        rRect.bottom=iHeight;
    }
    int y;
    try{
        int iCopyByte=rRect.Width()*DIBBitCount(ADIB)/8;
        for(y=rRect.top;y<rRect.bottom;y++){
            memset(GetBits(ADIB,rRect.left,y),AValue,iCopyByte);
        }
    }
    catch (...) {
        return false;
    }
    return true;
}
// 清空图像数据
bool ClearDIB(LPSTR ADIB)
{
    return FillDIB(ADIB,0);
}

// 获得图像的大小
// 2006.3.26
CRect GetDIBRect(LPSTR ADIB){
    return CRect(0,0,DIBWidth(ADIB),DIBHeight(ADIB));
}

// 复制图像数据(要求图像位数和大小一致)
// 2006.3.9
bool CopyDIBToDIB(LPSTR ASource,LPSTR ATarget)
{
    int y;
    int iHeight=DIBHeight(ASource);
    int iWidth=DIBWidth(ASource)*DIBBitCount(ASource)/8;
    for(y=0;y<iHeight;y++){
        memcpy(GetBits(ATarget,y),GetBits(ASource,y),iWidth);
    }
    return true;
}

// 获得两幅图像的差别图像
// 2004.8.28
void GetDifferImage(LPSTR AFirst,LPSTR ASecond,LPSTR AResult)
{
    byte byBit=(BYTE)(DIBBitCount(AFirst)/8);
    byte byResultBit=(BYTE)(DIBBitCount(AResult)/8);
    
    UINT32 dwLineBeginAddress,dwResultLineBeginAddress;
    UINT32 dwLineByte,dwResultLineByte;
    // 进行比较
    char* lpFirstAddress=FindDIBBits(AFirst);
    char* lpSecondAddress=FindDIBBits(ASecond);
    char* lpResultAddress=FindDIBBits(AResult);
    int iWidth=DIBWidth(AFirst);
    int iHeight=DIBHeight(AFirst);
    
    //计算原图像每行的字节数
    LONG lLineBytes = WIDTHBYTES(iWidth * byBit<<3);
    LONG lResultLineBytes = WIDTHBYTES(iWidth * byResultBit<<3);
    
    for(int y=0;y<iHeight;y++)
    {
        dwLineBeginAddress=(iHeight-1-y)*lLineBytes;
        dwResultLineBeginAddress=(iHeight-1-y)*lResultLineBytes;
        for(int x=0;x<iWidth;x++)
        {
            // 以下代码如提取到行，可提高速度
            dwLineByte=dwLineBeginAddress+x*byBit;
            dwResultLineByte=dwResultLineBeginAddress+x*byResultBit;
            
            if((*(lpFirstAddress+dwLineByte) != *(lpSecondAddress+dwLineByte)) ||
                (*(lpFirstAddress+dwLineByte+1) != *(lpSecondAddress+dwLineByte+1)) ||
                (*(lpFirstAddress+dwLineByte+2) != *(lpSecondAddress+dwLineByte+2)))
            {
                *(lpResultAddress+dwResultLineByte) = *(lpFirstAddress+dwLineByte);
                *(lpResultAddress+dwResultLineByte+1) = *(lpFirstAddress+dwLineByte+1);
                *(lpResultAddress+dwResultLineByte+2) = *(lpFirstAddress+dwLineByte+2);
            }
            else
            {
                *(lpResultAddress+dwResultLineByte) = 0;
                *(lpResultAddress+dwResultLineByte+1) = 0;
                *(lpResultAddress+dwResultLineByte+2) = 0;
            }
        }
    }
}

// 二值化处理
// ARect    : 处理的区域
// AThresh  : 阈值，255 为基数
// 2004.8.13
void DIBDoubleProc(LPSTR ADIB,LPSTR ATargetDIB,CRect ARect,byte AThresh)
{
    byte byGray,r,g,b;
    byte* pSAddrLine;
    byte* pTAddrLine;
    byte* pSAddress;
    byte* pTAddress;
    byte bySBit=(BYTE)(DIBBitCount(ADIB)>>3);
    int x,y;
    for(y=ARect.top;y<ARect.bottom;y++)
    {
        pSAddrLine=GetBits(ADIB,y);
        pTAddrLine=GetBits(ATargetDIB,y);
        for(x=ARect.left;x<ARect.right;x++)
        {
            pSAddress=pSAddrLine+x*bySBit;
            pTAddress=pTAddrLine+x;
            b=(*pSAddress);
            if(bySBit>1){
                g=(*(pSAddress+1));
                r=(*(pSAddress+2));
                byGray=(byte)((77*r+151*g+28*b)>>8);
            }
            else{
                byGray=b;
            }
            
            if(byGray>AThresh)
            {
                byGray=255;
            }
            else
            {
                byGray=0;
            }
            *pTAddress=byGray;
        }
    }
}

// 根据一列值进行调整的二值化
// 2005.3.15
void OptimizeDbl(LPSTR ADIB,LPSTR ATargetDIB,CRect ARect,byte AThresh)
{
    byte byGray,r,g,b;
    // 计算非字符区的均值
    double dblCount=0;  // 总灰度值
    int    iCount=0;    // 总点数
    byte bySBit=(BYTE)(DIBBitCount(ADIB)>>3);
    LONG lSLineBytes = WIDTHBYTES(DIBWidth(ADIB) * bySBit<<3);
    LONG lTLineBytes = WIDTHBYTES(DIBWidth(ATargetDIB)*(1<<3));
    LPSTR pSAddrBytes=FindDIBBits(ADIB);;
    LPSTR pTAddrBytes=FindDIBBits(ATargetDIB);
    LPSTR pSAddress;
    LPSTR pTAddress;
    
    int x,y;
    int iHeight=DIBHeight(ADIB);
    long lCount;
    long lGrayCount;
    byte byAver=0;
    for(x=ARect.left;x<ARect.right;x++)
    {
        lCount=0;
        lGrayCount=0;
        for(y=ARect.top+1;y<ARect.bottom-1;y++)
        {
            pSAddress=pSAddrBytes+lSLineBytes*y+x*bySBit;
            // 只处理背景区域
            b=(*pSAddress);
            if(bySBit>1){
                g=(*(pSAddress+1));
                r=(*(pSAddress+2));
                byGray=(byte)((77*r+151*g+28*b)>>8);
            }
            else{
                byGray=b;
            }
            // 如果大于指定阈值则参与运算
            if(byGray<=120)
            {
                lCount++;
                lGrayCount+=byGray;
            }
        }
        // 求平均灰度
        byAver=(BYTE)(70+(byte)((double)45/(double)125*(double)lGrayCount/(double)lCount));
        if(byAver>255) byAver=255;
        for(y=ARect.top;y<ARect.bottom;y++)
        {
            pSAddress=pSAddrBytes+lSLineBytes*y+x*bySBit;
            pTAddress=pTAddrBytes+lTLineBytes*y+x;
            // 只处理背景区域
            b=(*pSAddress);
            if(bySBit>1){
                g=(*(pSAddress+1));
                r=(*(pSAddress+2));
                byGray=(byte)((77*r+151*g+28*b)>>8);
            }
            else{
                byGray=b;
            }
            // 如果大于指定阈值则参与运算
            if(byGray>byAver)
            {
                byGray=255;
            }
            else{
                byGray=0;
            }
            *pTAddress=byGray;
        }
    }
}

// 灰度图像投影
// AGray   : =TRUE 为灰度图，否则按照二值图计算(黑色为 0 ,白色为 1)
// AOrient : =0 为从横向投影，否则为纵向投影
// 2005.1.21
void DIBProjectionGray(LPSTR ADIB,CArrayLong* AData,BOOL AGray,CRect ARect,int AOrient)
{
    AData->RemoveAll();
    CRect rRect;
    long iCount=0;
    byte byGray;
    byte* pAddress;
    int x,y;
    int iWidth=DIBWidth(ADIB);
    int iHeight=DIBHeight(ADIB);
    if(ARect.Width()==0) rRect=CRect(0,0,iWidth,iHeight);
    else                 rRect=ARect;
    
    int iBit=DIBBitCount(ADIB)/8;
    LPSTR lpDIBAddress=FindDIBBits(ADIB);
    LONG lLineBytes = WIDTHBYTES(iWidth * iBit<<3);
    
    if(AOrient==0){
        for(x=rRect.left;x<rRect.right;x++)
        {
            iCount=0;
            for(y=rRect.top;y<rRect.bottom;y++)
            {
                //pAddress=(byte*)(lpDIBAddress+lLineBytes*(iHeight-1-y)+x*iBit);
                pAddress=GetBits(ADIB,x,y);
                byGray=(*pAddress);
                if(AGray) iCount+=byGray;
                else      iCount+=(byGray==0?0:1);
            }
            AData->Add(iCount);
        }
    }
    else{
        for(y=rRect.top;y<rRect.bottom;y++)
        {
            iCount=0;
            for(x=rRect.left;x<rRect.right;x++)
            {
                pAddress=(byte*)(lpDIBAddress+lLineBytes*(iHeight-1-y)+x*iBit);
                byGray=(*pAddress);
                if(AGray) iCount+=byGray;
                else      iCount+=(byGray==0?0:1);
            }
            AData->Add(iCount);
        }
    }
}

// 两幅图像是否相同
// 返回差别的像素点数
// 2005.1.22
int DIBMatchImage(LPSTR ASource,LPSTR ATarget)
{
    CSize szSource(DIBWidth(ASource),DIBHeight(ASource));
    CSize szTarget(DIBWidth(ATarget),DIBHeight(ATarget));
    // 大小不一致不进行比对
    if(szSource.cx!=szTarget.cx || szSource.cy!=szTarget.cy) return szSource.cx*szSource.cy;

    byte bySBit=(BYTE)(DIBBitCount(ASource)>>3);
    byte byTBit=(BYTE)(DIBBitCount(ATarget)>>3);
    // 图像位数不一致不进行比对
    if(bySBit!=byTBit) return szSource.cx*szSource.cy;
    
    byte* pSAddrLine;
    byte* pSAddress;
    byte* pTAddrLine;
    byte* pTAddress;
    int iDiffCount=0;
    for(int y=0;y<szSource.cy;y++)
    {
        pSAddrLine=GetBits(ASource,y);
        pTAddrLine=GetBits(ATarget,y);
        for(int x=0;x<szSource.cx;x++)
        {
            pSAddress=pSAddrLine+x*bySBit;
            pTAddress=pTAddrLine+x*bySBit;
            if(bySBit>1)
            {
                if(*(pSAddress+1)!=*(pTAddress+1)) {
                    iDiffCount++;
                    continue;
                }
                if(*(pSAddress+2)!=*(pTAddress+2)) {
                    iDiffCount++;
                    continue;
                }
            }
            if(*pSAddress!=*pTAddress) {
                iDiffCount++;
                continue;
            }
        }
    }
    return iDiffCount;
}

// 获得一个图像中一个颜色的总数和
// AColor : 如果为灰度，则为 byte 类型
// 2005.1.22
_int64 GetImageSpecColorCount(LPSTR ADIB,COLORREF AColor)
{
    byte byBit=(BYTE)(DIBBitCount(ADIB)>>3);
    int iWidth=DIBWidth(ADIB);
    int iHeight=DIBHeight(ADIB);
    byte* pAddrLine;
    byte* pAddress;
    _int64 iCount=0;
    byte tR,tG,tB;
    byte R=(byte)(AColor & 0xFF);
    byte G=(byte)((AColor>>8) & 0xFF);
    byte B=(byte)((AColor>>16) & 0xFF);
    for(int y=0;y<iHeight;y++)
    {
        pAddrLine=GetBits(ADIB,y);
        for(int x=0;x<iWidth;x++)
        {
            pAddress=pAddrLine+x*byBit;
            tB=*(pAddress);
            if(byBit>1){
                tG=*(pAddress+1);
                tR=*(pAddress+2);
                if(RGB(tR,tG,tB)==AColor) iCount++;
            }
            else{
                if(tB==AColor) iCount++;
            }
        }
    }
    return iCount;
}

// 获得一个图像中所有颜色的总和
_int64 GetImageColorCount(LPSTR ADIB)
{
    byte byBit=(BYTE)(DIBBitCount(ADIB)>>3);
    int iWidth=DIBWidth(ADIB);
    int iHeight=DIBHeight(ADIB);
    byte* pAddrLine;
    byte* pAddress;
    _int64 iCount=0;
    byte tR,tG,tB;
    for(int y=0;y<iHeight;y++)
    {
        pAddrLine=GetBits(ADIB,y);
        for(int x=0;x<iWidth;x++)
        {
            pAddress=pAddrLine+x*byBit;
            tB=*(pAddress);
            if(byBit>1){
                tG=*(pAddress+1);
                tR=*(pAddress+2);
                iCount+=(tB+tR+tG);
            }
            else{
                iCount+=tB;
            }
        }
    }
    return iCount;
}

// 求模板图像在源图像的差方和
// 2005.1.9
double DiffSqrCount(LPSTR ASource, LPSTR AModel,int dx, int dy)
{
    double dblValue=0;
    byte bySBit=(BYTE)(DIBBitCount(ASource)/8);
    byte byTBit=(BYTE)(DIBBitCount(AModel)/8);
    int nSrcWidth, nSrcHeight;
    int nTmpWidth, nTmpHeight;
    nSrcWidth=DIBWidth(ASource);
    nSrcHeight=DIBHeight(ASource);
    nTmpWidth=DIBWidth(AModel);
    nTmpHeight=DIBHeight(AModel);
    int x1,y1;
    byte rS,gS,bS,rT,gT,bT;
    BYTE * pSrcLine, * pTmpLine,* pSrcAddr, * pTmpAddr;
    for(y1=0;y1<nTmpHeight;y1++)
    {
        pSrcLine=GetBits(ASource,dy+y1)+dx*bySBit;
        pTmpLine = GetBits(AModel,y1);
        for(x1=0;x1<nTmpWidth;x1++)
        {
            pSrcAddr=pSrcLine+x1*bySBit;
            pTmpAddr=pTmpLine+x1*byTBit;
            bS=(byte)*(pSrcAddr);
            gS=0;
            rS=0;
            //gS=(byte)*(pSrcAddr+1);
            //rS=(byte)*(pSrcAddr+2);
            bT=(byte)*(pTmpAddr);
            gT=0;
            rT=0;
            //gT=(byte)*(pTmpAddr+1);
            //rT=(byte)*(pTmpAddr+2);
            // 求差方和
            if(bT>10){
                dblValue+=((bS-bT)*(bS-bT)+(gS-gT)*(gS-gT)+(rS-rT)*(rS-rT));
            }
        }
    }
    return dblValue;
}

// 获得一个位图的头结构
BITMAPINFOHEADER GetBmpFileHeader(CString AFileName)
{
    CFile file;
    BITMAPFILEHEADER bmfHeader={0};
    BITMAPINFOHEADER bmfInfo={0};
    //打开文件失败
    if (!file.Open(AFileName, CFile::modeRead | CFile::shareDenyWrite))
    {
//        AfxMessageBox("打开 "+AFileName+" 文件失败!");
        return bmfInfo;
    }
    file.Read(&bmfHeader,sizeof(bmfHeader));
    file.Read(&bmfInfo,sizeof(bmfInfo));
    return bmfInfo;
}



}