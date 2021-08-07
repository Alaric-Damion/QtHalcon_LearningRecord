// dibapi.h
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#if !defined(_INC_DIBAPI__EA4BD1C7_732B_4E97_8EE3_1F14A27C1F89__INCLUDED_)
#define _INC_DIBAPI__EA4BD1C7_732B_4E97_8EE3_1F14A27C1F89__INCLUDED_

/* Handle to a DIB */
DECLARE_HANDLE(HDIB);

namespace sunLib{

/* DIB constants */
#define PALVERSION   0x300

/* DIB Macros*/

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// PCX文件头结构
typedef struct{
		 BYTE bManufacturer;
		 BYTE bVersion;
		 BYTE bEncoding;
		 BYTE bBpp;
		 WORD wLeft;
		 WORD wTop;
		 WORD wRight;
		 WORD wBottom;
		 WORD wXResolution;
		 WORD wYResolution;
		 BYTE bPalette[48];
		 BYTE bReserved;
		 BYTE bPlanes;
		 WORD wLineBytes;
		 WORD wPaletteType;
		 WORD wSrcWidth;
		 WORD wSrcDepth;
		 BYTE bFiller[54];
} PCXHEADER;

/* Function prototypes */
SUN_DLL_DEFINE BOOL      WINAPI  PaintDIB (HDC, LPRECT, HDIB, LPRECT, CPalette* pPal);
SUN_DLL_DEFINE BOOL      WINAPI  CreateDIBPalette(HDIB hDIB, CPalette* cPal);
SUN_DLL_DEFINE LPSTR     WINAPI  FindDIBBits (LPSTR lpbi);
SUN_DLL_DEFINE int     WINAPI  DIBWidth (LPSTR lpDIB);
SUN_DLL_DEFINE int     WINAPI  DIBHeight (LPSTR lpDIB);
SUN_DLL_DEFINE WORD      WINAPI  PaletteSize (LPSTR lpbi);
SUN_DLL_DEFINE DWORD     WINAPI  DIBNumColors (LPSTR lpbi);
SUN_DLL_DEFINE WORD      WINAPI  DIBBitCount(LPSTR lpbi);      // 返回调色板颜色占用的位数
SUN_DLL_DEFINE HGLOBAL   WINAPI  CopyHandle (HGLOBAL h);

SUN_DLL_DEFINE BOOL      WINAPI  SaveDIB (HDIB hDib, CFile& file);
SUN_DLL_DEFINE HDIB      WINAPI  ReadDIBFile(CFile& file);
SUN_DLL_DEFINE HDIB      WINAPI ReadDIBFile(CFile& file,CRect ARect);

SUN_DLL_DEFINE HPALETTE  WINAPI  GetSystemPalette(void);
SUN_DLL_DEFINE HPALETTE  WINAPI  CreateIdentifyPalette(HPALETTE  hPalSrc);
SUN_DLL_DEFINE HPALETTE  WINAPI  CopyPalette(HPALETTE hPalSrc);


SUN_DLL_DEFINE BOOL      WINAPI  DIBToPCX256(HDIB hDib,CFile& file);
SUN_DLL_DEFINE HDIB      WINAPI  ReadPCX256(CFile& file);

}
#endif //!_INC_DIBAPI__EA4BD1C7_732B_4E97_8EE3_1F14A27C1F89__INCLUDED_
