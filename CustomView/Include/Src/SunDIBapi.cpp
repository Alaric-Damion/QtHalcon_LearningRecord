//  dibapi.cpp
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  PaintDIB()          - Painting routine for a DIB
//  CreateDIBPalette()  - Creates a palette from a DIB
//  FindDIBBits()       - Returns a pointer to the DIB bits
//  DIBWidth()          - Gets the width of the DIB
//  DIBHeight()         - Gets the height of the DIB
//  PaletteSize()       - Gets the size required to store the DIB's palette
//  DIBNumColors()      - Calculates the number of colors
//                        in the DIB's color table
//  CopyHandle()        - Makes a copy of the given global memory block
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
#include "stdafx.h"
#include "SunDIBapi.h"
#include <io.h>
#include <errno.h>
#include <math.h>
#include <direct.h>


/*************************************************************************
 *
 * PaintDIB()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * HDIB hDIB        - handle to global memory with a DIB spec
 *                    in it followed by the DIB bits
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * CPalette* pPal   - pointer to CPalette containing DIB's palette
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 ************************************************************************/
namespace sunLib{


BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
		return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	 *  buffer
	 */
	lpDIBHdr  = (LPSTR)hDIB;
	lpDIBBits = FindDIBBits(lpDIBHdr);

	// Get the DIB's palette, then select it into DC
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// Select as background since we have
		// already realized in forground if needed
		hOldPal = SelectPalette(hDC, hPal, TRUE);
	}

	/* Make sure to use the stretching mode best for color pictures */
	//::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
    else{
        int iImageHeight=DIBHeight((LPSTR)hDIB);
        CRect rTemp;
        rTemp.left  =lpDIBRect->left;
        rTemp.right =lpDIBRect->right;
        rTemp.top   =iImageHeight-lpDIBRect->top;
        rTemp.bottom=iImageHeight-lpDIBRect->bottom;
        rTemp.NormalizeRect();

	  bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
            rTemp.left,                     // SrcX
            rTemp.top,                      // SrcY
            rTemp.Width(),                  // wSrcWidth
            rTemp.Height(),                 // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP
    }

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

/*************************************************************************
 *
 * CreateDIBPalette()
 *
 * Parameter:
 *
 * HDIB hDIB        - specifies the DIB
 *
 * Return Value:
 *
 * HPALETTE         - specifies the palette
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 ************************************************************************/


BOOL WINAPI CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	DWORD wNumColors;        // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (hDIB == NULL)
	  return FALSE;

   lpbi = (LPSTR)hDIB;

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal =new _TCHAR[sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)* wNumColors];

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			return FALSE;
		}

		lpPal = (LPLOGPALETTE)hLogPal;

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = pPal->CreatePalette(lpPal);
		delete hLogPal;
	}

	return bResult;
}

HPALETTE WINAPI GetSystemPalette(void) 
{ 

    //DC句柄
    HDC hDC;

    //调色板句柄 
    static HPALETTE hPal = NULL; 

     
//逻辑调色板指针
    LPLOGPALETTE  lpLogPal;  

    //逻辑调色板句柄
HANDLE hLogPal;

    //颜色数
    WORD nColors; 
 
    
   //获取DC句柄 
    hDC = GetDC(NULL); 
 
 
    //获取句柄失败
    if (hDC==NULL) 
        return NULL; 
 

    //调色板颜色数, PLANES是颜色层数
    nColors = (WORD)(1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));  
    ASSERT(nColors); 
    
    //给逻辑调色板分配内存
    hLogPal = new _TCHAR[sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY)]; 
 

    //分配失败 
    if (hLogPal==NULL) 
        return NULL; 
 

    //锁定内存
    lpLogPal = (LPLOGPALETTE)hLogPal; 
 
    //设置版本号
    lpLogPal->palVersion = PALVERSION; 
   
    //颜色数 
    lpLogPal->palNumEntries = nColors; 
 
    //拷贝当前调色板到逻辑调色板 
    GetSystemPaletteEntries(hDC, 0, nColors, 
            (LPPALETTEENTRY)(lpLogPal->palPalEntry)); 
 
    //创建调色板 
    hPal = CreatePalette(lpLogPal); 
 
    //释放内存
    delete hLogPal; 

    //释放DC
    ReleaseDC(NULL, hDC); 
 
    //返回系统调色板句柄
    return hPal; 

} 
 

HPALETTE WINAPI CreateIdentifyPalette(HPALETTE  hPalSrc)
{
    
    //颜色数
    int i, iSysColors, iPalEntries;

    //调色板句柄
    HPALETTE hPalette, hpalOld;

   
    //源调色板为空
	if (! hPalSrc)
		return NULL;


	//从源调色板拷贝一个新调色板
	hPalette = CopyPalette(hPalSrc);

    // 获取屏幕DC
    HDC hdcScreen = GetDC(NULL);
    ASSERT(hdcScreen);


    // 检测调色板管理器
    if (!(GetDeviceCaps(hdcScreen, RASTERCAPS) & RC_PALETTE)) 
	{
        TRACE("Not a palettized device");
        return NULL;
    }


    //系统调色板保留颜色数
    iSysColors = GetDeviceCaps(hdcScreen, NUMCOLORS);

    //调色板颜色数
    iPalEntries = GetDeviceCaps(hdcScreen, SIZEPALETTE);


    // 保留颜色数大于256，无需创建等同调色板
    if (iSysColors > 256) 
      {
        ::ReleaseDC(NULL, hdcScreen);
        return  NULL;
      }


    //设置当前调色板 
    SetSystemPaletteUse(hdcScreen, SYSPAL_NOSTATIC);
    SetSystemPaletteUse(hdcScreen, SYSPAL_STATIC);

    //选入调色板
    hpalOld = ::SelectPalette(hdcScreen,
                              hPalette, 
                              FALSE);

    //实现调色板
    ::RealizePalette(hdcScreen);

    //选入源调色板，但不实现
    ::SelectPalette(hdcScreen, hpalOld, FALSE);

    // 拷贝调色板，此时系统调色板保留颜色将被置于调色板两端
    PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hdcScreen, 
                            0,
                            iPalEntries,
                            pe);

    
//对系统保留颜色，PALETTEENTRY的peFlags标志设为PC-NOCOLAPSE ，但逻辑调色板
//的PALETTEENTRY的peFlags标志设为NULL。
    for (i = 0; i < iSysColors/2; i++) {
        pe[i].peFlags = 0;
    }

    for (; i < iPalEntries-iSysColors/2; i++) {
        pe[i].peFlags = PC_NOCOLLAPSE;
    }

    for (; i < iPalEntries; i++) {
        pe[i].peFlags = 0;
    }


    //调节调色板大小
    ResizePalette(hPalette, iPalEntries);

    // 重设逻辑调色板表项
    SetPaletteEntries(hPalette, 0, iPalEntries, pe);

    //释放DC
    ::ReleaseDC(NULL, hdcScreen);

    //返回等同调色板
    return  hPalette;
}

/*************************************************************************
 *
 * FindDIBBits()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * LPSTR            - pointer to the DIB bits
 *
 * Description:
 *
 * This function calculates the address of the DIB's bits and returns a
 * pointer to the DIB bits.
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	//return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
    return (lpbi + ((BITMAPINFOHEADER*)lpbi)->biSize + PaletteSize(lpbi));
}

/*************************************************************************
 *
 * DIBWidth()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * width field if it is an other-style DIB.
 *
 ************************************************************************/

int WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth>0?lpbmi->biWidth:-lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return lpbmc->bcWidth>0?(DWORD)lpbmc->bcWidth:(-(int)lpbmc->bcWidth);
}


/*************************************************************************
 *
 * DIBHeight()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * height field if it is an other-style DIB.
 *
 ************************************************************************/


int WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight>0?lpbmi->biHeight:-lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return lpbmc->bcHeight>0?(DWORD)lpbmc->bcHeight:(-(int)lpbmc->bcHeight);
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
 * style DIB).
 *
 ************************************************************************/


WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/


DWORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;
		case 4:
			return 16;
		case 8:
			return 256;
		default:
			return 0;
	}
}

// 返回调色板颜色占用的位数
// 2004.7.31
WORD WINAPI DIBBitCount(LPSTR lpbi)
{
    ASSERT(lpbi);
    if(!lpbi) {
        AfxMessageBox(_T("获得位图调色板颜色位数失败！"));
        return 0;
    }
    WORD wBitCount;
    if(IS_WIN30_DIB(lpbi))
    {
        wBitCount=((LPBITMAPINFOHEADER)lpbi)->biBitCount;
    }
    else
    {
        wBitCount=((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
    }
    return wBitCount;
}

//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyHandle (from SDK DibView sample clipbrd.c)
//
// Purpose:    Makes a copy of the given global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
//             Routine stolen verbatim out of ShowDIB.
//
// Parms:      h == Handle to global memory to duplicate.
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	SIZE_T dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

//  myfile.cpp
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  SaveDIB()           - Saves the specified dib in a file
//  ReadDIBFile()       - Loads a DIB from a file
//
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

/*
 * Dib Header Marker - used in writing DIBs to files
 */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')


/*************************************************************************
 *
 * SaveDIB()
 *
 * Saves the specified DIB into the specified CFile.  The CFile
 * is opened and closed by the caller.
 *
 * Parameters:
 *
 * HDIB hDib - Handle to the dib to save
 *
 * CFile& file - open CFile used to save DIB
 *
 * Return value: TRUE if successful, else FALSE or CFileException
 *
 *************************************************************************/


BOOL WINAPI SaveDIB(HDIB hDib, CFile& file)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize;

	if (hDib == NULL)
		return FALSE;

	/*
	 * Get a pointer to the DIB memory, the first of which contains
	 * a BITMAPINFO structure
	 */
	lpBI = (LPBITMAPINFOHEADER)hDib;
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/*
	 * Fill in the fields of the file header
	 */

	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps).
	//
	// So, instead let's calculate the size manually (if we can)
	//
	// First, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.

	dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);
	TRY
	{
		// Write the file header
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		THROW_LAST();
	}
	END_CATCH

	return TRUE;
}


/*************************************************************************

  Function:  ReadDIBFile (CFile&)

   Purpose:  Reads in the specified DIB file into a global chunk of
			 memory.

   Returns:  A handle to a dib (hDIB) if successful.
			 NULL if an error occurs.

  Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything
			 from the end of the BITMAPFILEHEADER structure on is
			 returned in the global memory handle.

*************************************************************************/


HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	ULONGLONG dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	/*
	 * get length of DIB in bytes for use when reading
	 */

	dwBitsSize = file.GetLength();

	/*
	 * Go read the DIB file header and check if it's valid.
	 */
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return NULL;

	/*
	 * Allocate memory for DIB
	 */
	hDIB = (HDIB)new _TCHAR[dwBitsSize];
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR)hDIB;

	/*
	 * Go read the bits.
	 */
	if (file.Read(pDIB, (UINT)dwBitsSize - sizeof(BITMAPFILEHEADER)) !=(UINT)dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		delete hDIB;
		return NULL;
	}
	return hDIB;
}

// 读取 DIB 文件指定位置的图像(支持 8、24 和 32 位图像)
// 2007.6.7
HDIB WINAPI ReadDIBFile(CFile& file,CRect ARect)
{
	BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bmInfoHeader;
	HDIB hDIB;
	LPSTR pDIB;

	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

    if(file.Read((LPSTR)&bmInfoHeader,sizeof(bmInfoHeader))!=sizeof(bmInfoHeader)){
        return NULL;
    }
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return NULL;

    DWORD dwSourceWidth =bmInfoHeader.biWidth;
    DWORD dwSourceHeight=bmInfoHeader.biHeight;
    // 重设图像大小
    bmInfoHeader.biWidth    =ARect.Width();
    bmInfoHeader.biHeight   =ARect.Height();

    // 读取每一行的数据
    // 每一行的字节数
    DWORD dwSourceLineSize=WIDTHBYTES(bmInfoHeader.biBitCount*dwSourceWidth);
    DWORD dwTargetLineSize=WIDTHBYTES(bmInfoHeader.biBitCount*ARect.Width());
    LPSTR pSourceDIBAddr=(LPSTR)(sizeof(bmfHeader) + bmInfoHeader.biSize + PaletteSize((LPSTR)&bmInfoHeader));
    LPSTR pTargetDIBAddr=(LPSTR)(bmInfoHeader.biSize + (DWORD_PTR)PaletteSize((LPSTR)&bmInfoHeader));
    // 重设占用字节大小
    bmInfoHeader.biSizeImage=dwTargetLineSize*ARect.Height();

    DWORD dwReadSize=bmInfoHeader.biSize+PaletteSize((LPSTR)&bmInfoHeader)+bmInfoHeader.biSizeImage;
    hDIB = (HDIB)new _TCHAR[dwReadSize];
    if (hDIB == 0)
    {
        return NULL;
    }
    pDIB = (LPSTR)hDIB;
    
    memcpy(pDIB,&bmInfoHeader,sizeof(bmInfoHeader));
    // 读取调色板
    file.Seek(sizeof(bmfHeader)+sizeof(bmInfoHeader),CFile::begin);
    file.Read(pDIB+sizeof(bmInfoHeader),PaletteSize((LPSTR)&bmInfoHeader));
    for(int y=ARect.top;y<ARect.bottom;y++){
        DWORD_PTR dwSourceLineAddr=(DWORD_PTR)pSourceDIBAddr+dwSourceLineSize*(dwSourceHeight-y-1)+ARect.left*(bmInfoHeader.biBitCount>>3);
        DWORD_PTR dwTargetLineAddr=(DWORD_PTR)pTargetDIBAddr+dwTargetLineSize*(ARect.bottom-y-1);

        file.Seek((DWORD_PTR)dwSourceLineAddr,CFile::begin);
        if(file.Read(pDIB+dwTargetLineAddr,dwTargetLineSize)!=dwTargetLineSize){
            delete hDIB;
            return NULL;
        }
    }

	return hDIB;
}

HPALETTE WINAPI CopyPalette(HPALETTE hPalSrc)
{        
    //调色板表项数，初始化为0
    int             nNumEntries=0;
    
    //指向逻辑调色板的指针
    PLOGPALETTE     plogPal;

    //调色板句柄
    HPALETTE        hPal;

    //内存句柄
	HANDLE			h;

    //获取调色板表项数
	nNumEntries = GetPaletteEntries(hPalSrc, 0, nNumEntries, NULL);


    //调色板为空，返回NULL
    if (nNumEntries == 0) 
        return (HPALETTE) NULL;


    //给逻辑调色板分配内存
	h =new _TCHAR[sizeof(DWORD) + sizeof(PALETTEENTRY)*nNumEntries];


    //内存分配失败
	if (! h)
        return (HPALETTE) NULL;


    //锁定内存
	plogPal = (PLOGPALETTE)h;


    //锁定失败
    if (! plogPal) 
        return (HPALETTE) NULL;


    //逻辑调色板版本号
    plogPal->palVersion = 0x300;

    //逻辑调色板表项数
    plogPal->palNumEntries = (WORD) nNumEntries;

    //拷贝源调色板到逻辑调色板
    GetPaletteEntries(hPalSrc, 0, nNumEntries, plogPal->palPalEntry);

    //创建逻辑调色板
    hPal = CreatePalette(plogPal);


    //释放内存
    delete h;

    //返回逻辑调色板句柄
    return hPal;

}


/*************************************************************************
 *
 * 函数名称：
 *
 *   DIBToPCX256()
 *
 * 参数:
 *
 *   HDIB hDib        // 指向DIB对象的句柄
 *
 *   CFile& file      // 要保存的文件
 *
 * 返回值:
 *
 *   BOOL             // 成功返回True，否则返回False。
 *
 * 说明:
 *
 *   该函数将指定的256色DIB对象保存为256色PCX文件。
 *
 *************************************************************************/

BOOL WINAPI DIBToPCX256(HDIB hDib, CFile& file)
{	
	// 指向源图像象素的指针
	BYTE *	lpSrc;
	
	// 指向编码后图像数据的指针
	BYTE *	lpDst;

	// DIB高度
	LONG	lHeight;
	
	// DIB宽度
	LONG	lWidth;

	//指向DIB图像的指针
	LPSTR   lpDIB;
	
	// 中间变量
	BYTE	bChar1;
	BYTE	bChar2;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 重复像素计数
	int		iCount;
	
	// 缓冲区已使用的字节数
	DWORD	dwBuffUsed;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 循环变量
	LONG	i;
	LONG	j;
	
	//获取指向DIB图像的指针
	lpDIB=(LPSTR)hDib;

	// 获取DIB高度
	lHeight = (WORD) DIBHeight(lpDIB);
	
	// 获取DIB宽度
	lWidth  = (WORD) DIBWidth(lpDIB);
	
	// 找到DIB图像象素起始位置
	lpDIBBits = FindDIBBits(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);	
	
	// PCX文件头
	PCXHEADER pcxHdr;
	
	// 给文件头赋值
	
	// PCX标识码
	pcxHdr.bManufacturer = 0x0A;
	
	// PCX版本号
	pcxHdr.bVersion = 5;
	
	// PCX编码方式（1表示RLE编码）
	pcxHdr.bEncoding = 1;
	
	// 像素位数（256色为8位）
	pcxHdr.bBpp = 8;
	
	// 图像相对于屏幕的左上角X坐标（以像素为单位）
	pcxHdr.wLeft = 0;
	
	// 图像相对于屏幕的左上角Y坐标（以像素为单位）
	pcxHdr.wTop = 0;
	
	// 图像相对于屏幕的右下角X坐标（以像素为单位）
	pcxHdr.wRight = (WORD)(lWidth - 1);
	
	// 图像相对于屏幕的右下角Y坐标（以像素为单位）
	pcxHdr.wBottom = (WORD)(lHeight - 1);
	
	// 图像的水平分辨率
	pcxHdr.wXResolution = (WORD)lWidth;
	
	// 图像的垂直分辨率
	pcxHdr.wYResolution = (WORD)lHeight;
	
	// 调色板数据（对于256色PCX该域无意义，可以直接赋值为0）
	for (i = 0; i < 48; i ++)
	{
		pcxHdr.bPalette[i] = 0;
	}
	
	// 保留域，设定为0。
	pcxHdr.bReserved = 0;
	
	// 图像色彩平面数目（对于256色PCX设定为1）。
	pcxHdr.bPlanes = 1;
	
	// 图像的宽度
	pcxHdr.wLineBytes = (WORD)lWidth;
	
	// 图像调色板的类型，1表示彩色或者单色图像，2表示图像是灰度图。
	pcxHdr.wPaletteType = 1;
	
	// 制作该图像的屏幕宽度（像素为单位）
	pcxHdr.wSrcWidth = 0;
	
	// 制作该图像的屏幕高度（像素为单位）
	pcxHdr.wSrcDepth = 0;
	
	// 保留域，取值设定为0。
	for (i = 0; i < 54; i ++)
	{
		pcxHdr.bFiller[i] = 0;
	}
	
	// 写入文件头
	file.Write((LPSTR)&pcxHdr, sizeof(PCXHEADER));
	
	// 开始编码
	
	// 开辟一片缓冲区以保存编码结果，请注意此处分配的大小是原图像大小的
	//两倍，这是因为有可能PCX不但没有压缩图像，反而增加了数据的存储量
	lpDst = new BYTE[lHeight * lWidth * 2];
	
	// 指明当前已经用了多少缓冲区
	dwBuffUsed = 0;
	
	// 每行
	for (i = 0; i < lHeight; i++)
	{
		// 指向DIB第i行，第0个象素的指针
		lpSrc = (BYTE *)lpDIBBits + lLineBytes * (lHeight - 1 - i);
		
		bChar1 = *lpSrc;
		
		iCount = 1;
		
		// 剩余列
		for (j = 1; j < lWidth; j ++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc++;
			
			// 读取下一个像素
			bChar2 = *lpSrc;
			
			// 判断前后两个像素是否相同并且iCount < 63
			if ((bChar1 == bChar2) && (iCount < 63))
			{
				// 相同，计数加1
				iCount ++;
				
			}

			// 不同，或者iCount = 63
			else
			{				
				if ((iCount > 1) || (bChar1 >= 0xC0))
				{
					// 先保存码长信息，即该像素值重复的次数
					lpDst[dwBuffUsed] = (BYTE)(iCount | 0xC0);
					
					// 再保存像素值
					lpDst[dwBuffUsed + 1] = bChar1;
					
					// 已用的缓冲区数目加2
					dwBuffUsed += 2;
				}
				else
				{
					// 直接保存该值
					lpDst[dwBuffUsed] = bChar1;
					
					// 已用的缓冲区数目加1
					dwBuffUsed ++;
				}
				
				// 继续对后面的像素进行处理
				bChar1 = bChar2;				
				iCount = 1;
			}
		}
		
		// 保存每行最后一部分编码
		if ((iCount > 1) || (bChar1 >= 0xC0))
		{
			// 先保存码长信息，即该像素值重复的次数
			lpDst[dwBuffUsed] = (BYTE)(iCount | 0xC0);
			
			// 再保存像素值
			lpDst[dwBuffUsed + 1] = bChar1;
			
			// 已用的缓冲区数目加2
			dwBuffUsed += 2;
		}
		else
		{
			// 直接保存该值
			lpDst[dwBuffUsed] = bChar1;
			
			// 已用的缓冲区数目加1
			dwBuffUsed ++;
		}
	}
	
	// 写入编码结果
	file.Write((LPSTR)lpDst, dwBuffUsed);	

	// 释放内存
	delete lpDst;
	
	// 写入调色板信息
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL bWin30DIB;
	
	// 开辟一片缓冲区以保存调色板，注意大小为769＝256X3＋1
	lpDst = new BYTE[769];
	
	// 调色板第一个字节
	* lpDst = 0x0C;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// 判断是否是WIN3.0的DIB
	bWin30DIB = IS_WIN30_DIB(lpDIB);
	
	// 读取当前DIB调色板
	for (i = 0; i < 256; i ++)
	{
		if (bWin30DIB)
		{
			// 读取DIB调色板红色分量
			lpDst[i * 3 + 1] = lpbmi->bmiColors[i].rgbRed;
			
			// 读取DIB调色板绿色分量
			lpDst[i * 3 + 2] = lpbmi->bmiColors[i].rgbGreen;
			
			// 读取DIB调色板蓝色分量
			lpDst[i * 3 + 3] = lpbmi->bmiColors[i].rgbBlue;
		}
		else
		{
			// 读取DIB调色板红色分量
			lpDst[i * 3 + 1] = lpbmc->bmciColors[i].rgbtRed;
			
			// 读取DIB调色板绿色分量
			lpDst[i * 3 + 2] = lpbmc->bmciColors[i].rgbtGreen;
			
			// 读取DIB调色板蓝色分量
			lpDst[i * 3 + 3] = lpbmc->bmciColors[i].rgbtBlue;
		}
	}
	
	// 写入调色板信息
	file.Write((LPSTR)lpDst, 769);
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *
 *   ReadPCX256()
 *
 * 参数:
 *
 *   CFile& file         // 要读取的文件
 *
 * 返回值:
 *
 *   HDIB                // 成功返回DIB的句柄，否则返回NULL。
 *
 * 说明:
 *
 *   该函数读取指定的256色PCX文件并保存在一个未压缩
 * 编码的DIB对象中。
 *
 *************************************************************************/

HDIB WINAPI ReadPCX256(CFile& file)
{
	// PCX文件头
	PCXHEADER pcxHdr;
	
	// DIB大小（字节数）
	DWORD	dwDIBSize;
	
	// DIB句柄
	HDIB	hDIB;
	
	// DIB指针
	LPSTR	pDIB;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 重复像素计数
	int		iCount;
	
	// DIB高度
	LONG	lHeight;
	
	// DIB宽度
	LONG	lWidth;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 中间变量
	BYTE	bChar;
	
	// 指向源图像象素的指针
	BYTE *	lpSrc;
	
	// 指向编码后图像数据的指针
	BYTE *	lpDst;
	
	// 临时指针
	BYTE *	lpTemp;
	
	// 尝试读取PCX文件头
	if (file.Read((LPSTR)&pcxHdr, sizeof(PCXHEADER)) != sizeof(PCXHEADER))
	{
		// 大小不对，返回NULL。
		return NULL;
	}
	
	// 判断是否是256色PCX文件，检查第一个字节是否是0x0A，
	if ((pcxHdr.bManufacturer != 0x0A) || (pcxHdr.bBpp != 8) || (pcxHdr.bPlanes != 1))
	{
		// 非256色PCX文件，返回NULL。
		return NULL;
	}
	
	// 获取图像高度
	lHeight = pcxHdr.wBottom - pcxHdr.wTop + 1;
	
	// 获取图像宽度
	lWidth  = pcxHdr.wRight - pcxHdr.wLeft + 1;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 计算DIB长度（字节）
	dwDIBSize = sizeof(BITMAPINFOHEADER) + 1024 + lHeight * lLineBytes;
	
	// 为DIB分配内存
	hDIB = (HDIB)new _TCHAR[dwDIBSize];
	if (hDIB == 0)
	{
		// 内存分配失败，返回NULL。
		return NULL;
	}
	
	// 锁定
	pDIB = (LPSTR)hDIB;
	
	// 指向BITMAPINFOHEADER的指针
	LPBITMAPINFOHEADER lpbmi;
	
	// 赋值
	lpbmi = (LPBITMAPINFOHEADER) pDIB;
	
	// 给lpbmi成员赋值
	lpbmi->biSize = 40;
	lpbmi->biWidth = lWidth;
	lpbmi->biHeight = lHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = 8;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = lHeight * lLineBytes;
	lpbmi->biXPelsPerMeter = pcxHdr.wXResolution;
	lpbmi->biYPelsPerMeter = pcxHdr.wYResolution;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;
	
	// 分配内存以读取编码后的象素
	lpSrc = new BYTE[file.GetLength() - sizeof(PCXHEADER) - 769];
	lpTemp = lpSrc;
	
	// 读取编码后的象素
	if (file.Read(lpSrc, (UINT)file.GetLength() - sizeof(PCXHEADER) - 769) !=(UINT)file.GetLength() - sizeof(PCXHEADER) - 769 )
	{
		// 释放内存
		delete hDIB;
		
		// 返回NULL。
		return NULL;
	}
	
	// 计算DIB中像素位置
	lpDst = (BYTE *) FindDIBBits(pDIB);
	
	// 一行一行解码
	for (j = 0; j <lHeight; j++)
	{
		i = 0;
		while (i < lWidth)
		{
			// 读取一个字节
			bChar = *lpTemp;
			lpTemp++;
			
			if ((bChar & 0xC0) == 0xC0)
			{
				// 行程
				iCount = bChar & 0x3F;
				
				// 读取下一个字节
				bChar = *lpTemp;
				lpTemp++;
				
				// bChar重复iCount次保存
				memset(&lpDst[(lHeight - j - 1) * lLineBytes + i], bChar, iCount);

				// 已经读取像素的个数加iCount
				i += iCount;
			}
			else
			{
				// 保存当前字节
				lpDst[(lHeight - j - 1) * lLineBytes + i] = bChar;
				
				// 已经读取像素的个数加1
				i += 1;
			}
		}
	}
	
	// 释放内存
	delete lpSrc;
	
	// 调色板
	
	// 读调色板标志位
	file.Read(&bChar, 1);
	if (bChar != 0x0C)
	{
		// 释放内存
		delete hDIB;
		
		// 返回NULL。
		return NULL;
	}
	
	// 分配内存以读取编码后的象素
	lpSrc = new BYTE[768];
	
	// 计算DIB中调色板的位置
	lpDst = (BYTE *) pDIB + sizeof(BITMAPINFOHEADER);
	
	// 读取调色板
	if (file.Read(lpSrc, 768) != 768)
	{
		// 大小不对。
		
		// 释放内存
		delete hDIB;
		
		// 返回NULL。
		return NULL;
	}
	
	// 给调色板赋值
	for (i = 0; i < 256; i++)
	{
		lpDst[i * 4] = lpSrc[i * 3 + 2];
		lpDst[i * 4 + 1] = lpSrc[i * 3 + 1];
		lpDst[i * 4 + 2] = lpSrc[i * 3];
		lpDst[i * 4 + 3] = 0;
	}
	
	// 释放内存
	delete lpSrc;
	
	// 返回DIB句柄
	return hDIB;
}
}