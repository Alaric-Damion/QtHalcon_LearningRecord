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

    //DC���
    HDC hDC;

    //��ɫ���� 
    static HPALETTE hPal = NULL; 

     
//�߼���ɫ��ָ��
    LPLOGPALETTE  lpLogPal;  

    //�߼���ɫ����
HANDLE hLogPal;

    //��ɫ��
    WORD nColors; 
 
    
   //��ȡDC��� 
    hDC = GetDC(NULL); 
 
 
    //��ȡ���ʧ��
    if (hDC==NULL) 
        return NULL; 
 

    //��ɫ����ɫ��, PLANES����ɫ����
    nColors = (WORD)(1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));  
    ASSERT(nColors); 
    
    //���߼���ɫ������ڴ�
    hLogPal = new _TCHAR[sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY)]; 
 

    //����ʧ�� 
    if (hLogPal==NULL) 
        return NULL; 
 

    //�����ڴ�
    lpLogPal = (LPLOGPALETTE)hLogPal; 
 
    //���ð汾��
    lpLogPal->palVersion = PALVERSION; 
   
    //��ɫ�� 
    lpLogPal->palNumEntries = nColors; 
 
    //������ǰ��ɫ�嵽�߼���ɫ�� 
    GetSystemPaletteEntries(hDC, 0, nColors, 
            (LPPALETTEENTRY)(lpLogPal->palPalEntry)); 
 
    //������ɫ�� 
    hPal = CreatePalette(lpLogPal); 
 
    //�ͷ��ڴ�
    delete hLogPal; 

    //�ͷ�DC
    ReleaseDC(NULL, hDC); 
 
    //����ϵͳ��ɫ����
    return hPal; 

} 
 

HPALETTE WINAPI CreateIdentifyPalette(HPALETTE  hPalSrc)
{
    
    //��ɫ��
    int i, iSysColors, iPalEntries;

    //��ɫ����
    HPALETTE hPalette, hpalOld;

   
    //Դ��ɫ��Ϊ��
	if (! hPalSrc)
		return NULL;


	//��Դ��ɫ�忽��һ���µ�ɫ��
	hPalette = CopyPalette(hPalSrc);

    // ��ȡ��ĻDC
    HDC hdcScreen = GetDC(NULL);
    ASSERT(hdcScreen);


    // ����ɫ�������
    if (!(GetDeviceCaps(hdcScreen, RASTERCAPS) & RC_PALETTE)) 
	{
        TRACE("Not a palettized device");
        return NULL;
    }


    //ϵͳ��ɫ�屣����ɫ��
    iSysColors = GetDeviceCaps(hdcScreen, NUMCOLORS);

    //��ɫ����ɫ��
    iPalEntries = GetDeviceCaps(hdcScreen, SIZEPALETTE);


    // ������ɫ������256�����贴����ͬ��ɫ��
    if (iSysColors > 256) 
      {
        ::ReleaseDC(NULL, hdcScreen);
        return  NULL;
      }


    //���õ�ǰ��ɫ�� 
    SetSystemPaletteUse(hdcScreen, SYSPAL_NOSTATIC);
    SetSystemPaletteUse(hdcScreen, SYSPAL_STATIC);

    //ѡ���ɫ��
    hpalOld = ::SelectPalette(hdcScreen,
                              hPalette, 
                              FALSE);

    //ʵ�ֵ�ɫ��
    ::RealizePalette(hdcScreen);

    //ѡ��Դ��ɫ�壬����ʵ��
    ::SelectPalette(hdcScreen, hpalOld, FALSE);

    // ������ɫ�壬��ʱϵͳ��ɫ�屣����ɫ�������ڵ�ɫ������
    PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hdcScreen, 
                            0,
                            iPalEntries,
                            pe);

    
//��ϵͳ������ɫ��PALETTEENTRY��peFlags��־��ΪPC-NOCOLAPSE �����߼���ɫ��
//��PALETTEENTRY��peFlags��־��ΪNULL��
    for (i = 0; i < iSysColors/2; i++) {
        pe[i].peFlags = 0;
    }

    for (; i < iPalEntries-iSysColors/2; i++) {
        pe[i].peFlags = PC_NOCOLLAPSE;
    }

    for (; i < iPalEntries; i++) {
        pe[i].peFlags = 0;
    }


    //���ڵ�ɫ���С
    ResizePalette(hPalette, iPalEntries);

    // �����߼���ɫ�����
    SetPaletteEntries(hPalette, 0, iPalEntries, pe);

    //�ͷ�DC
    ::ReleaseDC(NULL, hdcScreen);

    //���ص�ͬ��ɫ��
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

// ���ص�ɫ����ɫռ�õ�λ��
// 2004.7.31
WORD WINAPI DIBBitCount(LPSTR lpbi)
{
    ASSERT(lpbi);
    if(!lpbi) {
        AfxMessageBox(_T("���λͼ��ɫ����ɫλ��ʧ�ܣ�"));
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

// ��ȡ DIB �ļ�ָ��λ�õ�ͼ��(֧�� 8��24 �� 32 λͼ��)
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
    // ����ͼ���С
    bmInfoHeader.biWidth    =ARect.Width();
    bmInfoHeader.biHeight   =ARect.Height();

    // ��ȡÿһ�е�����
    // ÿһ�е��ֽ���
    DWORD dwSourceLineSize=WIDTHBYTES(bmInfoHeader.biBitCount*dwSourceWidth);
    DWORD dwTargetLineSize=WIDTHBYTES(bmInfoHeader.biBitCount*ARect.Width());
    LPSTR pSourceDIBAddr=(LPSTR)(sizeof(bmfHeader) + bmInfoHeader.biSize + PaletteSize((LPSTR)&bmInfoHeader));
    LPSTR pTargetDIBAddr=(LPSTR)(bmInfoHeader.biSize + (DWORD_PTR)PaletteSize((LPSTR)&bmInfoHeader));
    // ����ռ���ֽڴ�С
    bmInfoHeader.biSizeImage=dwTargetLineSize*ARect.Height();

    DWORD dwReadSize=bmInfoHeader.biSize+PaletteSize((LPSTR)&bmInfoHeader)+bmInfoHeader.biSizeImage;
    hDIB = (HDIB)new _TCHAR[dwReadSize];
    if (hDIB == 0)
    {
        return NULL;
    }
    pDIB = (LPSTR)hDIB;
    
    memcpy(pDIB,&bmInfoHeader,sizeof(bmInfoHeader));
    // ��ȡ��ɫ��
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
    //��ɫ�����������ʼ��Ϊ0
    int             nNumEntries=0;
    
    //ָ���߼���ɫ���ָ��
    PLOGPALETTE     plogPal;

    //��ɫ����
    HPALETTE        hPal;

    //�ڴ���
	HANDLE			h;

    //��ȡ��ɫ�������
	nNumEntries = GetPaletteEntries(hPalSrc, 0, nNumEntries, NULL);


    //��ɫ��Ϊ�գ�����NULL
    if (nNumEntries == 0) 
        return (HPALETTE) NULL;


    //���߼���ɫ������ڴ�
	h =new _TCHAR[sizeof(DWORD) + sizeof(PALETTEENTRY)*nNumEntries];


    //�ڴ����ʧ��
	if (! h)
        return (HPALETTE) NULL;


    //�����ڴ�
	plogPal = (PLOGPALETTE)h;


    //����ʧ��
    if (! plogPal) 
        return (HPALETTE) NULL;


    //�߼���ɫ��汾��
    plogPal->palVersion = 0x300;

    //�߼���ɫ�������
    plogPal->palNumEntries = (WORD) nNumEntries;

    //����Դ��ɫ�嵽�߼���ɫ��
    GetPaletteEntries(hPalSrc, 0, nNumEntries, plogPal->palPalEntry);

    //�����߼���ɫ��
    hPal = CreatePalette(plogPal);


    //�ͷ��ڴ�
    delete h;

    //�����߼���ɫ����
    return hPal;

}


/*************************************************************************
 *
 * �������ƣ�
 *
 *   DIBToPCX256()
 *
 * ����:
 *
 *   HDIB hDib        // ָ��DIB����ľ��
 *
 *   CFile& file      // Ҫ������ļ�
 *
 * ����ֵ:
 *
 *   BOOL             // �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *
 *   �ú�����ָ����256ɫDIB���󱣴�Ϊ256ɫPCX�ļ���
 *
 *************************************************************************/

BOOL WINAPI DIBToPCX256(HDIB hDib, CFile& file)
{	
	// ָ��Դͼ�����ص�ָ��
	BYTE *	lpSrc;
	
	// ָ������ͼ�����ݵ�ָ��
	BYTE *	lpDst;

	// DIB�߶�
	LONG	lHeight;
	
	// DIB���
	LONG	lWidth;

	//ָ��DIBͼ���ָ��
	LPSTR   lpDIB;
	
	// �м����
	BYTE	bChar1;
	BYTE	bChar2;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// �ظ����ؼ���
	int		iCount;
	
	// ��������ʹ�õ��ֽ���
	DWORD	dwBuffUsed;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;

	// ѭ������
	LONG	i;
	LONG	j;
	
	//��ȡָ��DIBͼ���ָ��
	lpDIB=(LPSTR)hDib;

	// ��ȡDIB�߶�
	lHeight = (WORD) DIBHeight(lpDIB);
	
	// ��ȡDIB���
	lWidth  = (WORD) DIBWidth(lpDIB);
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = FindDIBBits(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);	
	
	// PCX�ļ�ͷ
	PCXHEADER pcxHdr;
	
	// ���ļ�ͷ��ֵ
	
	// PCX��ʶ��
	pcxHdr.bManufacturer = 0x0A;
	
	// PCX�汾��
	pcxHdr.bVersion = 5;
	
	// PCX���뷽ʽ��1��ʾRLE���룩
	pcxHdr.bEncoding = 1;
	
	// ����λ����256ɫΪ8λ��
	pcxHdr.bBpp = 8;
	
	// ͼ���������Ļ�����Ͻ�X���꣨������Ϊ��λ��
	pcxHdr.wLeft = 0;
	
	// ͼ���������Ļ�����Ͻ�Y���꣨������Ϊ��λ��
	pcxHdr.wTop = 0;
	
	// ͼ���������Ļ�����½�X���꣨������Ϊ��λ��
	pcxHdr.wRight = (WORD)(lWidth - 1);
	
	// ͼ���������Ļ�����½�Y���꣨������Ϊ��λ��
	pcxHdr.wBottom = (WORD)(lHeight - 1);
	
	// ͼ���ˮƽ�ֱ���
	pcxHdr.wXResolution = (WORD)lWidth;
	
	// ͼ��Ĵ�ֱ�ֱ���
	pcxHdr.wYResolution = (WORD)lHeight;
	
	// ��ɫ�����ݣ�����256ɫPCX���������壬����ֱ�Ӹ�ֵΪ0��
	for (i = 0; i < 48; i ++)
	{
		pcxHdr.bPalette[i] = 0;
	}
	
	// �������趨Ϊ0��
	pcxHdr.bReserved = 0;
	
	// ͼ��ɫ��ƽ����Ŀ������256ɫPCX�趨Ϊ1����
	pcxHdr.bPlanes = 1;
	
	// ͼ��Ŀ��
	pcxHdr.wLineBytes = (WORD)lWidth;
	
	// ͼ���ɫ������ͣ�1��ʾ��ɫ���ߵ�ɫͼ��2��ʾͼ���ǻҶ�ͼ��
	pcxHdr.wPaletteType = 1;
	
	// ������ͼ�����Ļ��ȣ�����Ϊ��λ��
	pcxHdr.wSrcWidth = 0;
	
	// ������ͼ�����Ļ�߶ȣ�����Ϊ��λ��
	pcxHdr.wSrcDepth = 0;
	
	// ������ȡֵ�趨Ϊ0��
	for (i = 0; i < 54; i ++)
	{
		pcxHdr.bFiller[i] = 0;
	}
	
	// д���ļ�ͷ
	file.Write((LPSTR)&pcxHdr, sizeof(PCXHEADER));
	
	// ��ʼ����
	
	// ����һƬ�������Ա������������ע��˴�����Ĵ�С��ԭͼ���С��
	//������������Ϊ�п���PCX����û��ѹ��ͼ�񣬷������������ݵĴ洢��
	lpDst = new BYTE[lHeight * lWidth * 2];
	
	// ָ����ǰ�Ѿ����˶��ٻ�����
	dwBuffUsed = 0;
	
	// ÿ��
	for (i = 0; i < lHeight; i++)
	{
		// ָ��DIB��i�У���0�����ص�ָ��
		lpSrc = (BYTE *)lpDIBBits + lLineBytes * (lHeight - 1 - i);
		
		bChar1 = *lpSrc;
		
		iCount = 1;
		
		// ʣ����
		for (j = 1; j < lWidth; j ++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc++;
			
			// ��ȡ��һ������
			bChar2 = *lpSrc;
			
			// �ж�ǰ�����������Ƿ���ͬ����iCount < 63
			if ((bChar1 == bChar2) && (iCount < 63))
			{
				// ��ͬ��������1
				iCount ++;
				
			}

			// ��ͬ������iCount = 63
			else
			{				
				if ((iCount > 1) || (bChar1 >= 0xC0))
				{
					// �ȱ����볤��Ϣ����������ֵ�ظ��Ĵ���
					lpDst[dwBuffUsed] = (BYTE)(iCount | 0xC0);
					
					// �ٱ�������ֵ
					lpDst[dwBuffUsed + 1] = bChar1;
					
					// ���õĻ�������Ŀ��2
					dwBuffUsed += 2;
				}
				else
				{
					// ֱ�ӱ����ֵ
					lpDst[dwBuffUsed] = bChar1;
					
					// ���õĻ�������Ŀ��1
					dwBuffUsed ++;
				}
				
				// �����Ժ�������ؽ��д���
				bChar1 = bChar2;				
				iCount = 1;
			}
		}
		
		// ����ÿ�����һ���ֱ���
		if ((iCount > 1) || (bChar1 >= 0xC0))
		{
			// �ȱ����볤��Ϣ����������ֵ�ظ��Ĵ���
			lpDst[dwBuffUsed] = (BYTE)(iCount | 0xC0);
			
			// �ٱ�������ֵ
			lpDst[dwBuffUsed + 1] = bChar1;
			
			// ���õĻ�������Ŀ��2
			dwBuffUsed += 2;
		}
		else
		{
			// ֱ�ӱ����ֵ
			lpDst[dwBuffUsed] = bChar1;
			
			// ���õĻ�������Ŀ��1
			dwBuffUsed ++;
		}
	}
	
	// д�������
	file.Write((LPSTR)lpDst, dwBuffUsed);	

	// �ͷ��ڴ�
	delete lpDst;
	
	// д���ɫ����Ϣ
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWin30DIB;
	
	// ����һƬ�������Ա����ɫ�壬ע���СΪ769��256X3��1
	lpDst = new BYTE[769];
	
	// ��ɫ���һ���ֽ�
	* lpDst = 0x0C;
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// �ж��Ƿ���WIN3.0��DIB
	bWin30DIB = IS_WIN30_DIB(lpDIB);
	
	// ��ȡ��ǰDIB��ɫ��
	for (i = 0; i < 256; i ++)
	{
		if (bWin30DIB)
		{
			// ��ȡDIB��ɫ���ɫ����
			lpDst[i * 3 + 1] = lpbmi->bmiColors[i].rgbRed;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 2] = lpbmi->bmiColors[i].rgbGreen;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 3] = lpbmi->bmiColors[i].rgbBlue;
		}
		else
		{
			// ��ȡDIB��ɫ���ɫ����
			lpDst[i * 3 + 1] = lpbmc->bmciColors[i].rgbtRed;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 2] = lpbmc->bmciColors[i].rgbtGreen;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 3] = lpbmc->bmciColors[i].rgbtBlue;
		}
	}
	
	// д���ɫ����Ϣ
	file.Write((LPSTR)lpDst, 769);
	
	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *
 *   ReadPCX256()
 *
 * ����:
 *
 *   CFile& file         // Ҫ��ȡ���ļ�
 *
 * ����ֵ:
 *
 *   HDIB                // �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *
 *   �ú�����ȡָ����256ɫPCX�ļ���������һ��δѹ��
 * �����DIB�����С�
 *
 *************************************************************************/

HDIB WINAPI ReadPCX256(CFile& file)
{
	// PCX�ļ�ͷ
	PCXHEADER pcxHdr;
	
	// DIB��С���ֽ�����
	DWORD	dwDIBSize;
	
	// DIB���
	HDIB	hDIB;
	
	// DIBָ��
	LPSTR	pDIB;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// �ظ����ؼ���
	int		iCount;
	
	// DIB�߶�
	LONG	lHeight;
	
	// DIB���
	LONG	lWidth;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// �м����
	BYTE	bChar;
	
	// ָ��Դͼ�����ص�ָ��
	BYTE *	lpSrc;
	
	// ָ������ͼ�����ݵ�ָ��
	BYTE *	lpDst;
	
	// ��ʱָ��
	BYTE *	lpTemp;
	
	// ���Զ�ȡPCX�ļ�ͷ
	if (file.Read((LPSTR)&pcxHdr, sizeof(PCXHEADER)) != sizeof(PCXHEADER))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}
	
	// �ж��Ƿ���256ɫPCX�ļ�������һ���ֽ��Ƿ���0x0A��
	if ((pcxHdr.bManufacturer != 0x0A) || (pcxHdr.bBpp != 8) || (pcxHdr.bPlanes != 1))
	{
		// ��256ɫPCX�ļ�������NULL��
		return NULL;
	}
	
	// ��ȡͼ��߶�
	lHeight = pcxHdr.wBottom - pcxHdr.wTop + 1;
	
	// ��ȡͼ����
	lWidth  = pcxHdr.wRight - pcxHdr.wLeft + 1;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ����DIB���ȣ��ֽڣ�
	dwDIBSize = sizeof(BITMAPINFOHEADER) + 1024 + lHeight * lLineBytes;
	
	// ΪDIB�����ڴ�
	hDIB = (HDIB)new _TCHAR[dwDIBSize];
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	pDIB = (LPSTR)hDIB;
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpbmi;
	
	// ��ֵ
	lpbmi = (LPBITMAPINFOHEADER) pDIB;
	
	// ��lpbmi��Ա��ֵ
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
	
	// �����ڴ��Զ�ȡ����������
	lpSrc = new BYTE[file.GetLength() - sizeof(PCXHEADER) - 769];
	lpTemp = lpSrc;
	
	// ��ȡ����������
	if (file.Read(lpSrc, (UINT)file.GetLength() - sizeof(PCXHEADER) - 769) !=(UINT)file.GetLength() - sizeof(PCXHEADER) - 769 )
	{
		// �ͷ��ڴ�
		delete hDIB;
		
		// ����NULL��
		return NULL;
	}
	
	// ����DIB������λ��
	lpDst = (BYTE *) FindDIBBits(pDIB);
	
	// һ��һ�н���
	for (j = 0; j <lHeight; j++)
	{
		i = 0;
		while (i < lWidth)
		{
			// ��ȡһ���ֽ�
			bChar = *lpTemp;
			lpTemp++;
			
			if ((bChar & 0xC0) == 0xC0)
			{
				// �г�
				iCount = bChar & 0x3F;
				
				// ��ȡ��һ���ֽ�
				bChar = *lpTemp;
				lpTemp++;
				
				// bChar�ظ�iCount�α���
				memset(&lpDst[(lHeight - j - 1) * lLineBytes + i], bChar, iCount);

				// �Ѿ���ȡ���صĸ�����iCount
				i += iCount;
			}
			else
			{
				// ���浱ǰ�ֽ�
				lpDst[(lHeight - j - 1) * lLineBytes + i] = bChar;
				
				// �Ѿ���ȡ���صĸ�����1
				i += 1;
			}
		}
	}
	
	// �ͷ��ڴ�
	delete lpSrc;
	
	// ��ɫ��
	
	// ����ɫ���־λ
	file.Read(&bChar, 1);
	if (bChar != 0x0C)
	{
		// �ͷ��ڴ�
		delete hDIB;
		
		// ����NULL��
		return NULL;
	}
	
	// �����ڴ��Զ�ȡ����������
	lpSrc = new BYTE[768];
	
	// ����DIB�е�ɫ���λ��
	lpDst = (BYTE *) pDIB + sizeof(BITMAPINFOHEADER);
	
	// ��ȡ��ɫ��
	if (file.Read(lpSrc, 768) != 768)
	{
		// ��С���ԡ�
		
		// �ͷ��ڴ�
		delete hDIB;
		
		// ����NULL��
		return NULL;
	}
	
	// ����ɫ�帳ֵ
	for (i = 0; i < 256; i++)
	{
		lpDst[i * 4] = lpSrc[i * 3 + 2];
		lpDst[i * 4 + 1] = lpSrc[i * 3 + 1];
		lpDst[i * 4 + 2] = lpSrc[i * 3];
		lpDst[i * 4 + 3] = 0;
	}
	
	// �ͷ��ڴ�
	delete lpSrc;
	
	// ����DIB���
	return hDIB;
}
}