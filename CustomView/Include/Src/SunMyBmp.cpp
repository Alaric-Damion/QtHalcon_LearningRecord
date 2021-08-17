// MyBmp.cpp: implementation of the MyBmp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunMyBmp.h"

#ifdef _DEBUG
#undef THIS_FILE
static _TCHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace sunLib{

//////////////////////////////////////////////////////////////////////////
// CImageConvert
//////////////////////////////////////////////////////////////////////////
wstring ImageConvertMutilByteToWide(const string& _src)
{
    // �����ַ��� string ת�� wchar_t ֮��ռ�õ��ڴ��ֽ���
    int nBufSize = MultiByteToWideChar(GetACP(),0,_src.c_str(),-1,NULL,0);

    // Ϊ wsbuf �����ڴ� BufSize ���ֽ�
    wchar_t *wsBuf = new wchar_t[nBufSize];

    // ת��Ϊ unicode �� WideString
    MultiByteToWideChar(GetACP(),0,_src.c_str(),-1,wsBuf,nBufSize);

    wstring wstrRet(wsBuf);

    delete []wsBuf;
    wsBuf = NULL;

    return wstrRet;
}

CImageConvert::CImageConvert(void)
{
    m_pImage=NULL;
}


CImageConvert::~CImageConvert(void)
{
    if(m_pImage) delete m_pImage;
}

CLSID GetEncoderClsid(_TCHAR* format)
{   
    CLSID id;
    memset(&id,0,sizeof(CLSID));
    UINT num = 0;                     // number of image encoders   
    UINT size = 0;                   // size of the image encoder array in bytes   
    ImageCodecInfo* pImageCodecInfo = NULL;   
    GetImageEncodersSize(&num, &size);   
    if(size == 0)   
        return id;     //   Failure   

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));   
    if(pImageCodecInfo == NULL)   
        return id;     //   Failure   

    wstring sFormat;
#ifdef _UNICODE    
    sFormat=format;
#else
    sFormat=ImageConvertMutilByteToWide(format);
#endif
    GetImageEncoders(num, size, pImageCodecInfo);   
    for(UINT j = 0; j < num; ++j)   
    {   
        if(wcscmp(pImageCodecInfo[j].MimeType, sFormat.c_str()) == 0 )   
        {   
            id = pImageCodecInfo[j].Clsid;   
            free(pImageCodecInfo);   
            return id;     //   Success   
        }           
    }   
    free(pImageCodecInfo);   
    return id;     //   Failure   
}

// ���ļ���ȡ
bool CImageConvert::LoadFromFile(CString a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback)
{
    bool bResult=false;
    SunMemoryStream memory;
    bResult=Bool(memory.LoadFromFile(a_sFileName,a_pOwner,a_pCallback));
    if(!bResult) return false;
    bResult=LoadFromMemory(&memory);
    return bResult;
}

// ���ļ���ȡ
bool CImageConvert::LoadFromFile( CString a_sFileName)
{
    if(m_pImage) delete m_pImage;
#ifdef _UNICODE
    m_pImage=new Bitmap(a_sFileName.GetBuffer(0));
#else
    wstring sw=ImageConvertMutilByteToWide(a_sFileName.GetBuffer(0));
    m_pImage=new Bitmap(sw.c_str());
#endif

    return true;
}

bool CImageConvert::LoadFromMemory(byte* a_pBuffer,__int64 a_iSize){
    // ������  
    IStream* pStream = NULL;  
    CreateStreamOnHGlobal( NULL, TRUE, &pStream);  
    if (pStream == NULL){  
        return false;  
    }  

    // ��ʼ����  
    LARGE_INTEGER liTemp = {0};  
    ULARGE_INTEGER uLiZero = {0};  
    ULONG ulRealSize = 0;  
    pStream->Seek(liTemp, STREAM_SEEK_SET,NULL);  
    pStream->SetSize(uLiZero);  

    HRESULT hr;
    ULONG iWrite=0;
    hr=pStream->Write(a_pBuffer,(ULONG)a_iSize,&iWrite);  // ��λͼ�ŵ�����  
    pStream->Seek(liTemp, STREAM_SEEK_SET,NULL);   

    if(m_pImage) delete m_pImage;
    m_pImage=new Bitmap(pStream);

    return true;
}

// ���ڴ��ȡ
bool CImageConvert::LoadFromMemory( SunMemoryStream* a_pMemory)
{
    return LoadFromMemory(a_pMemory->GetBuffer(),a_pMemory->GetSize());
}

// ����Ϊ�ļ�
bool CImageConvert::SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality,void* a_pOwner,PSunMemoryStatusCallback a_pCallback)
{
    SunMemoryStream memory;
    if(SaveToMemory(&memory,a_sFormat,a_iQuality)){
        memory.SaveToFile(a_sFileName,a_pOwner,a_pCallback);
    }
    return true;
}

// ����Ϊ�ļ�
bool CImageConvert::SaveToFile( CString a_sFileName,CString a_sFormat,UINT a_iQuality)
{
    return SaveToFile(a_sFileName,a_sFormat,a_iQuality,NULL,NULL);
}

// ���浽�ڴ�
bool CImageConvert::SaveToMemory(CString a_sFormat,UINT a_iQuality,OUT byte** a_pBuffer,OUT __int64* a_iSize){
    SunMemoryStream msMemory;
    SaveToMemory(&msMemory,a_sFormat,a_iQuality);
    if(msMemory.GetSize()>0){
        *a_pBuffer=new byte[msMemory.GetSize()];
        memcpy(*a_pBuffer,msMemory.GetBuffer(),msMemory.GetSize());
        return true;
    }
    return false;
}

// ���浽�ڴ�
// a_iQuality: ѹ��������ȡֵ��Χ��0~100 ,100 Ϊ�������
bool CImageConvert::SaveToMemory( SunMemoryStream* a_pMemory,CString a_sFormat,UINT a_iQuality)
{
    // ������  
    IStream* pStream = NULL;  
    CreateStreamOnHGlobal( NULL, TRUE, &pStream);  
    if (pStream == NULL){  
        return false;  
    }  

    // ��ʼ����  
    LARGE_INTEGER liTemp = {0};  
    ULARGE_INTEGER uLiZero = {0};  
    ULONG ulRealSize = 0;  
    pStream->Seek(liTemp, STREAM_SEEK_SET,NULL);  
    pStream->SetSize(uLiZero);  

    // ���������  
    IStream* pStreamOut = NULL;  

    //�������ڴ滺��������
    if (CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut) != S_OK){  
        pStream->Release();  
        pStream = NULL;  
        return false;  
    }  

    // ��ʼ�������  
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL );   
    pStreamOut->SetSize(uLiZero);   

    // ��λͼ����JPG�ĸ�ʽ���浽�������  
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    ULONG             quality=a_iQuality;
    encoderParameters.Parameter[0].Value = &quality;

    CString sLowFormat=a_sFormat;
    sLowFormat.MakeLower();
    CString sFormat=_T("image/")+sLowFormat;

    CLSID iClsid=GetEncoderClsid(sFormat.GetBuffer(0));
    m_pImage->Save(pStreamOut, &iClsid,&encoderParameters); 

    // ��ȡ�������С  
    bool bRet = false;  
    ULARGE_INTEGER libNewPos = {0};  
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);                      // ����ָ��ָ�����λ�ã��Ӷ���ȡ���Ĵ�С   
    a_pMemory->SetSize(libNewPos.QuadPart);
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                            // ����ָ��ָ��ʼλ��  
    pStreamOut->Read(a_pMemory->GetBuffer(), libNewPos.LowPart, &ulRealSize);   // ��ת�����ͼƬ�������û�  
    bRet = true;  

    // �ͷ��ڴ�  
    if (pStream != NULL){  
        pStream->Release();  
        pStream = NULL;  
    }  
    if (pStreamOut != NULL){  
        pStreamOut->Release();  
        pStreamOut = NULL;  
    }  
    return true;
}

// �� CBMP ��ȡͼ������
bool CImageConvert::LoadFromBMPObject(CBMP* a_pBMP){
    if(a_pBMP){
        byte* pBuffer;
        __int64 iSize;
        if(a_pBMP->SaveToBuffer(&pBuffer,&iSize)){
            if(LoadFromMemory(pBuffer,iSize)){
                return true;
            }
            delete pBuffer;
        }
    }
    return false;
}

// ��ͼ����� BMP ����
bool CImageConvert::SaveToBMPObject(CBMP* a_pBMP){
    // ���� 1:
    //     SunMemoryStream msStream;
    //     OutputDebugString(L"1");
    //     if(SaveToMemory(&msStream,_T("bmp"),100)){
    //         OutputDebugString(L"2");
    //         return a_pBMP->ReadDIBOfBuffer(msStream.GetBuffer(),msStream.GetSize());
    //     }
    //     else {
    //         return false;
    //     }

    // ���� 2:
    //     HBITMAP hBMP;
    //     m_pImage->GetHBITMAP(0,&hBMP);
    //      
    //     int iPixelFormat=m_pImage->GetPixelFormat();
    //     int iBits=24;
    //     switch(iPixelFormat){
    //         case PixelFormat1bppIndexed:iBits=1;
    //         break;
    //         case PixelFormat4bppIndexed:iBits=4;
    //         break;
    //         case PixelFormat8bppIndexed:iBits=8;
    //         break;
    //         case PixelFormat24bppRGB:   iBits=24;
    //         break;
    //         case PixelFormat32bppRGB:   iBits=32;
    //         break;
    //     }
    //     a_pBMP->LoadImageFromHBITMAP(hBMP,iBits);

    // ���� 3: 
    int iWidth=m_pImage->GetWidth();
    int iHeight=m_pImage->GetHeight();
    int iPixelFormat=m_pImage->GetPixelFormat();
    int iBits=24;
    switch(iPixelFormat){
    case PixelFormat1bppIndexed:iBits=1;
        break;
    case PixelFormat4bppIndexed:iBits=4;
        break;
    case PixelFormat8bppIndexed:iBits=8;
        break;
    case PixelFormat24bppRGB:   iBits=24;
        break;
    case PixelFormat32bppRGB:   iBits=32;
        break;
    }
    BitmapData bdData;
    Gdiplus::Rect r(0,0,iWidth,iHeight);
    m_pImage->LockBits(&r,ImageLockModeRead,iPixelFormat,&bdData);
    if(!a_pBMP->ReSize(a_pBMP->GetHDIBAdd(),iWidth,iHeight,iBits)) return false;
    int iWidthBytes=WIDTHBYTES(iWidth*iBits);

    for(int y=0;y<iHeight;y++){
        memcpy(a_pBMP->GetBits(y),(void*)((__int64)bdData.Scan0+iWidthBytes*y),iWidthBytes);
    }
    m_pImage->UnlockBits(&bdData);
    return true;
}

// ��ʾͼ��
void CImageConvert::DrawImage(HDC a_hDC){
    Graphics mGraphics(a_hDC);
    mGraphics.DrawImage(m_pImage,0,0,m_pImage->GetWidth(),m_pImage->GetHeight());
}

//////////////////////////////////////////////////////////////////////////
// CBMP
//////////////////////////////////////////////////////////////////////////

// ���캯��
// 2004.7.28
CBMP::CBMP()
{
    m_hScrDC    =NULL;
    m_hMemDC    =NULL;
    m_hOldBitmap=NULL;

    m_hDIB      =NULL;
    //��ɫ����
    m_palDIB    =NULL;
    m_hBmp      =NULL;
    m_hScreenDC =NULL;
    m_szScreenSize=CSize(0,0);
}

// ��������
// 2004.7.28
CBMP::~CBMP()
{   
    // �ͷ�λͼ
    if(m_hBmp) 
    {
        DeleteObject(m_hBmp);
        m_hBmp=NULL;
    }
    //�ͷŵ�ɫ��
    if (m_palDIB)
    {
        delete m_palDIB;
    }
    // �ͷ� DIB ռ�õ��ڴ�
    FreeDIB();
}

// ����λͼ��Դ
// 2004.7.28
/*
BOOL CBMP::CreateBitmap(HDC hdc,int nWidth,int nHeight)
{
    // �����µ�ɫ��
    if(!m_palDIB) m_palDIB = new CPalette;
    //����ʧ��
    if (!m_palDIB)
    {
        return FALSE;
    }
    //����CreateDIBPalette����������ɫ��
    if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
    {
        return FALSE;
    }
    if(DeleteGDIObject(m_hBmp)) 
    {
        m_hBmp=CreateCompatibleBitmap(hdc, nWidth, nHeight);
        return TRUE;
    }
    else return FALSE;
}
*/

// �ͷ� GDI ��Դ
// 2004.7.28
BOOL CBMP::DeleteGDIObject(HGDIOBJ AObj)
{
    if(AObj)
    {
        DeleteObject(AObj);
        AObj=NULL;
    }
    return TRUE;
}

// ����һ�� CBMP ����
// 2007.8.7
bool CBMP::Assign(CBMP& a_bmp)
{
    ReSize(GetHDIBAdd(),a_bmp.GetWidth(),a_bmp.GetHeight(),(byte)a_bmp.GetBitCount());
    bool bResult=CopyDIBToDIB(a_bmp.GetDIB(),GetDIB());
    if(GetBitCount()<=8) SetPalette(GetBitCount());
    return bResult;
}

// �ͷ� DIB ռ�õ��ڴ�
// 2004.7.28
void CBMP::FreeDIB()
{
    if(m_hDIB) 
    {
        delete[] m_hDIB;
        m_hDIB=NULL;
    }
}

// ��� DIB λͼָ��
// 2004.8.8
LPSTR CBMP::GetDIB()
{
    LPSTR lpDIB;
    lpDIB=(LPSTR)m_hDIB;
    return lpDIB;
}

// ����ͼ��Ĵ�С
// ABitmapBit ��λ�� 8,16,24,32 ...
// 2004.8.1
BOOL CBMP::ReSize(LPSTR* ADIB,int AWidth,int AHeight,byte ABitmapBit)
{
    // �ͷ� DIB ռ�õ��ڴ�
    FreeDIB();

    DWORD           dwPaletteSize=0;
    if (ABitmapBit <= 8)	dwPaletteSize = (WORD)(1 <<  ABitmapBit) *sizeof(RGBQUAD);
    DWORD dwBmBitsSize = WIDTHBYTES(ABitmapBit*AWidth)*AHeight;
    //Ϊλͼ���ݷ����ڴ�
    m_hDIB  = (HDIB)(new _TCHAR[dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER)]);
    *ADIB	= (LPSTR)m_hDIB;
    ((BITMAPINFOHEADER*)(*ADIB))->biSize        = sizeof(BITMAPINFOHEADER);
    ((BITMAPINFOHEADER*)(*ADIB))->biWidth       = AWidth;
    ((BITMAPINFOHEADER*)(*ADIB))->biHeight      = AHeight;
    ((BITMAPINFOHEADER*)(*ADIB))->biBitCount    = ABitmapBit;
    ((BITMAPINFOHEADER*)(*ADIB))->biCompression = BI_RGB;
    ((BITMAPINFOHEADER*)(*ADIB))->biPlanes             = 1;
    ((BITMAPINFOHEADER*)(*ADIB))->biSizeImage          = 0;
    ((BITMAPINFOHEADER*)(*ADIB))->biXPelsPerMeter      = 0;
    ((BITMAPINFOHEADER*)(*ADIB))->biYPelsPerMeter      = 0;
    ((BITMAPINFOHEADER*)(*ADIB))->biClrUsed            = 0;
    ((BITMAPINFOHEADER*)(*ADIB))->biClrImportant       = 0;
    
    return TRUE;
}

// ���ͼ������
void CBMP::ClearImage()
{
    ClearDIB(GetDIB());
}

// ת�� DDB Ϊһ�� DIB λͼ
// 2004.8.8
LPSTR CBMP::BitmapToDIB(const int a_iBits)
{
    return LoadImageFromHBITMAP(m_hBmp,a_iBits);
}

// ��ʼ����Ļ��ش��� 2010.12.24
void CBMP::InitScreen()
{
    if(m_hBmp){
        DeleteGDIObject(m_hBmp);
        m_hBmp=NULL;
    }
    //Ϊ��Ļ�����豸������
    m_hScreenDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    
    //int iBits = GetDeviceCaps(a_hDC, BITSPIXEL) * GetDeviceCaps(a_hDC, PLANES);
    // λͼ���
    // ѡ����������
    int       nWidth, nHeight;      
    // λͼ��Ⱥ͸߶�
    // ��Ļ�ֱ���
    // ȷ��ѡ������Ϊ�վ���
    //Ϊ��Ļ�����豸������
    //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
    m_hMemDC = CreateCompatibleDC(m_hScreenDC);
    // �����Ļ�ֱ���
    m_szScreenSize.cx = GetDeviceCaps(m_hScreenDC, HORZRES);
    m_szScreenSize.cy = GetDeviceCaps(m_hScreenDC, VERTRES);
    nWidth = m_szScreenSize.cx;
    nHeight = m_szScreenSize.cy;
    // ����һ������Ļ�豸��������ݵ�λͼ
    m_hBmp = CreateCompatibleBitmap(m_hScreenDC, nWidth, nHeight);
    if(!m_hBmp){
        return;
    }
}
// ��ȡ��Ļͼ��
void CBMP::QuickGetScreenImage(CRect a_rRect,const int a_iBits)
{
    if (IsRectEmpty(a_rRect))
        return;
    GdiFlush();
    // ����λͼѡ���ڴ��豸��������
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);
    // ����Ļ�豸�����������ڴ��豸��������
    BitBlt(m_hMemDC,0,0,a_rRect.Width(),a_rRect.Height(),m_hScreenDC,a_rRect.left,a_rRect.top,SRCCOPY);
    //�õ���Ļλͼ�ľ��
    m_hBmp = (HBITMAP)SelectObject(m_hMemDC,m_hOldBitmap);
    BitmapToDIB(a_iBits);
}
// �ͷ���Ļ�����Դ
void CBMP::DestroyScreen()
{
    if(m_hScreenDC) {
        DeleteDC(m_hScreenDC);
        m_hScreenDC=NULL;
    }
    if(m_hMemDC) {
        DeleteDC(m_hMemDC);
        m_hMemDC=NULL;
    }
}

// ȡ����Ļ��ͼ������
// 2004.7.28
LPSTR CBMP::GetScreenRect(CRect a_rRect,const int a_iBits)
{
    if(a_rRect.Width()<=0 || a_rRect.Height()<=0){
        return NULL;
    }
    if(m_hBmp){
        DeleteGDIObject(m_hBmp);
        m_hBmp=NULL;
    }
    m_hBmp=sunLib::CopyScreenToBitmap(a_rRect);
    
    return BitmapToDIB(a_iBits);
}

// ��ȡָ���豸��ͼ������(��Ҫ�Ȱ�һ��ͼ����� CBitmap ����ֱ������Ļ�ľ��)
LPSTR CBMP::GetDCRect(const HDC a_hDC,CRect a_rRect)
{
    if(a_rRect.Width()<=0 || a_rRect.Height()<=0){
        return NULL;
    }
    if(m_hBmp){
        DeleteGDIObject(m_hBmp);
        m_hBmp=NULL;
    }
    m_hBmp=sunLib::CopyDCToBitmap(a_hDC,a_rRect);
    
    return BitmapToDIB(GetBitCount());
}

// ȡ���ı�ͼ������
// 2004.8.8
LPSTR CBMP::GetTextImage(CString AText,COLORREF AColor,int ASize,int AWidth,int AHeight)
{
    m_hBmp=sunLib::DrawTextToBitmap(AText,AColor,ASize,AWidth,AHeight);
    
    return BitmapToDIB(GetBitCount());
}

// ׼����ͼ��Ϣ
// 2006.4.14
void CBMP::_ReadyGDIInfo()
{
    int       nWidth, nHeight;      
    //Ϊ��Ļ�����豸������
    m_hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
    m_hMemDC = CreateCompatibleDC(m_hScrDC);
    //hMemDC=GetDC(0);
    
    nWidth = GetWidth();
    nHeight = GetHeight();
    if(m_hBmp) {
        DeleteObject(m_hBmp);
        m_hBmp=NULL;
    }
    // ����һ������Ļ�豸��������ݵ�λͼ
    if(!m_hBmp){
        m_hBmp = CreateCompatibleBitmap(m_hScrDC, nWidth, nHeight);
    }
    // ����λͼѡ���ڴ��豸��������
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);
    // ����Ļ�豸�����������ڴ��豸��������
    //BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
    SetBkMode(m_hMemDC,OPAQUE);
}

// ��ԭ��ͼ��Ϣ
// 2006.4.14
void CBMP::_RevertGDIInfo()
{
    //�õ���Ļλͼ�ľ��
    (HBITMAP)SelectObject(m_hMemDC, m_hOldBitmap);
    //��� 
    DeleteDC(m_hScrDC);
    DeleteDC(m_hMemDC);
}

// ���ͼ��
// 2006.9.1
void CBMP::FillImage(COLORREF AFillColor)
{
    CBrush brush;
    brush.CreateSolidBrush(AFillColor);
    SelectObject(m_hMemDC,brush);
    // ��䱳��ɫ
    FillRect(m_hMemDC,CRect(0,0,GetWidth(),GetHeight()),(HBRUSH)brush.m_hObject);
}

// ���ƶ����
// APoint   : ���ж���
// ACount   : ���ж�������
// 2005.11.23
LPSTR CBMP::DrawPolygon(POINT* APoint,int ACount,CPoint AOffset,COLORREF AColor,COLORREF AFillColor,
                        bool AFill,int APenWidth)
{
    // ׼�� GDI
    //_ReadyGDIInfo();

    SetBkMode(m_hMemDC,TRANSPARENT);
    CBrush brush1;
    if(AFill){          // �Ƿ����
        brush1.CreateSolidBrush(AFillColor);
        SelectObject(m_hMemDC,brush1);
    }
    
    CPen pen;
    pen.CreatePen(PS_SOLID,APenWidth,AColor);
    SelectObject(m_hMemDC,pen);
    
    POINT* pPoint=new POINT[ACount];
    for(int i=0;i<ACount;i++){
        pPoint[i].x=APoint[i].x+AOffset.x;
        pPoint[i].y=APoint[i].y+AOffset.y;
    }
    //SetTextColor(hMemDC,AColor);
    // �������
    Polygon(m_hMemDC,pPoint,ACount);
    //Polyline(hMemDC,APoint,ACount);

    delete[] pPoint;

    // ��ԭ GDI
    //_RevertGDIInfo();
    //return BitmapToDIB();
    return 0;
}

// �����߶�
// 2006.4.14
void CBMP::DrawLine(CPoint APoint1,CPoint APoint2,COLORREF AColor,int AWidth)
{
    // ׼�� GDI
    //_ReadyGDIInfo();
    SetBkMode(m_hMemDC,TRANSPARENT);
    CPen pen;
    pen.CreatePen(PS_SOLID,AWidth,AColor);
    SelectObject(m_hMemDC,pen);
    MoveToEx(m_hMemDC,APoint1.x,APoint1.y,NULL);
    LineTo(m_hMemDC,APoint2.x,APoint2.y);

    // ��ԭ GDI
    //_RevertGDIInfo();
    //BitmapToDIB();
}

// ��ͼ���ļ��ж�ȡ����
// 2004.7.30
LPSTR CBMP::ReadDIBFile(CString AFileName)
{
    // �ͷ� DIB ռ�õ��ڴ�
    FreeDIB();

    LPSTR lpbi;
    CFile file;
    //���ļ�ʧ��
    if (!file.Open(AFileName, CFile::modeRead | CFile::shareDenyWrite))
    {
        ASSERT("���ļ�����!");
        return NULL;
    }
    m_hDIB = sunLib::ReadDIBFile(file);
    file.Close();
    lpbi = (LPSTR)m_hDIB;

    return (LPSTR)lpbi;
}

LPSTR CBMP::ReadDIBFile(CString AFileName,CRect ARect)
{
    // �ͷ� DIB ռ�õ��ڴ�
    FreeDIB();
    
    LPSTR lpbi;
    CFile file;
    //���ļ�ʧ��
    if (!file.Open(AFileName, CFile::modeRead | CFile::shareDenyWrite))
    {
        ASSERT("���ļ�����!");
        return NULL;
    }
    m_hDIB = sunLib::ReadDIBFile(file,ARect);
    file.Close();
    lpbi = (LPSTR)m_hDIB;
    
    return (LPSTR)lpbi;
}

LPSTR CBMP::ReadDIBOfBuffer(const byte* a_pBuffer,const __int64 a_iSize)
{
    // �ͷ� DIB ռ�õ��ڴ�
    FreeDIB();
    
//     LPSTR lpbi;
//     CMemFile mfFile;
//     mfFile.Write(a_pBuffer,(UINT)a_iSize);
//     mfFile.SeekToBegin();
//     m_hDIB = sunLib::ReadDIBFile(mfFile);
//     mfFile.Close();
//     lpbi = (LPSTR)m_hDIB;
    __int64 iNewSize=a_iSize-sizeof(BITMAPFILEHEADER);
    m_hDIB=(HDIB)new byte[iNewSize];
    memcpy(m_hDIB,a_pBuffer+sizeof(BITMAPFILEHEADER),iNewSize);
    return (LPSTR)m_hDIB;
}

// �������ݵ�ͼ���ļ�
// 2004.8.1
BOOL CBMP::SaveToFile(CString AFileName)
{
    if(GetBitCount()<=8) SetPalette(GetBitCount());
    CFile file;
    //���ļ�
    if (!file.Open(AFileName, CFile::modeCreate |
        CFile::modeReadWrite | CFile::shareExclusive))
    {
        return FALSE;
    }
    BOOL bSuccess = FALSE;
    TRY
    {
        //����ΪDIBͼ��
        bSuccess = SaveDIB(m_hDIB,file);
        
        file.Close();
    }
    //����ʧ��
    CATCH (CException, eSave)
    {
        file.Abort();
        return FALSE;
    }
    END_CATCH
    return bSuccess;
}

BOOL CBMP::SaveToBuffer(OUT byte** a_pBuffer,OUT __int64* a_iSize)
{
    if(DIBBitCount(GetDIB())<=8) SetPalette(GetBitCount());
    CMemFile file;
    BOOL bSuccess = FALSE;
    TRY
    {
        //����ΪDIBͼ��
        bSuccess = SaveDIB(m_hDIB,file);
        if(bSuccess){
            file.SeekToBegin();
            *a_iSize=(UINT)file.GetLength();
            *a_pBuffer=new byte[*a_iSize];
            file.Read(*a_pBuffer,(UINT)*a_iSize);
        }
        file.Close();
    }
    //����ʧ��
    CATCH (CException, eSave)
    {
        file.Abort();
        return FALSE;
    }
    END_CATCH
    return bSuccess;
}

// ��ʾ��λͼ
// xDest,yDest  : Ŀ����ʾλ��
// ASrc         : λͼ��ԭʼλ��
// 2004.8.1
void CBMP::DisplayToScreen(CDC* ADC,int xDest,int yDest,CRect ASrc)
{
    if(!m_hDIB) return;
    CRect rSrc=ASrc;
    if(ASrc.Width()<=0 || ASrc.Height()<=0){
        rSrc=GetRect();
    }

    CRect rDest;
    rDest.left=xDest;       rDest.top=yDest;
    rDest.right=xDest+rSrc.Width();
    rDest.bottom=yDest+rSrc.Height();
    PaintDIB(ADC->m_hDC,rDest, GetHDIB(), rSrc, GetPalette());
}

// �������ͼ��������ռ���ֽ���
// 2004.12.14
__int64 CBMP::GetDIBCountByte()
{   
    if(!m_hDIB) {
        AfxMessageBox(_T("���λͼ��ɫ����ɫλ��ʧ�ܣ�"));
        return 0;
    }
    WORD        byBit=DIBBitCount(GetDIB());
	DWORD           dwPaletteSize=0;
    if (byBit <= 8)	dwPaletteSize = (WORD)(1 <<  byBit) *sizeof(RGBQUAD);
    DWORD dwBmBitsSize = WIDTHBYTES(byBit*DIBWidth(GetDIB()))*GetHeight();
    //Ϊλͼ���ݷ����ڴ�
    return dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER);
}

// ���õ�ɫ��
// ABitCount:һ������ռ�õ�λ��
// ��ɫ��ļ��㷽����
// n=��ɫ����ɫ����
// a=256/n
// i=��ǰ��ɫ�����
// �� n<16 ʱ����ɫ��ĵ�ǰֵ���㹫ʽΪ��
// x=i*(a+a/n)+(a/n-1)
// �� n>=16 ʱ����ɫ��ĵ�ǰֵ���㹫ʽΪ��
// x=(int)(i*(a+a/n)+(i/n*a))
// 2005.1.21
void CBMP::SetPalette(int ABitCount)
{
    tagPALETTEENTRY* pPal;
    LPSTR lpStr=(LPSTR)m_hDIB;
    pPal=(tagPALETTEENTRY*)(lpStr + ((BITMAPINFOHEADER*)lpStr)->biSize);
    byte* pAddr=(byte*)pPal;
    int n=1<<ABitCount;
    int a=256/n;
    int iBit=sizeof(RGBQUAD);
    byte* pTAddr;
    int iValue;
    if(n>=16){
        for(int i=0;i<n;i++){
            pTAddr=pAddr+i*iBit;
            iValue=(int)(i*(a+a/n)+(i/n*a));
            *(pTAddr)=(byte)iValue;
            *(pTAddr+1)=(byte)iValue;
            *(pTAddr+2)=(byte)iValue;
            *(pTAddr+3)=0;
        }
    }
    else{
        for(int i=0;i<n;i++){
            pTAddr=pAddr+i*iBit;
            if(i==0) iValue=0;
            else     iValue=i*(a+a/n)+(a/n-1);
            *(pTAddr)=(byte)iValue;
            *(pTAddr+1)=(byte)iValue;
            *(pTAddr+2)=(byte)iValue;
            *(pTAddr+3)=0;
        }
    }
}

// ����ͼ����ת����ʱ�룩 2017.6.2
void CBMP::RotateImage(int a_iAngle){
    CBMP bmpTemp;
    bmpTemp.Assign(*this);
    int iWidth=GetWidth();
    int iHeight=GetHeight();
    int iPixelBytes=GetBitCount()/8;
    int iAngle=a_iAngle % 360;
    if(iAngle==0){
        return; 
    }
    else if(iAngle==90){
        int iNewWidth=iHeight;
        int iNewHeight=iWidth;
        ReSize(GetHDIBAdd(),iNewWidth,iNewHeight,iPixelBytes*8);
        for(int y=0;y<iNewHeight;y++){
            byte* pTargetLine=GetBits(y);
            for(int x=0;x<iNewWidth;x++){
                for(int i=0;i<iPixelBytes;i++){
                    *(pTargetLine+x*iPixelBytes+i)=*bmpTemp.GetBits(iNewHeight-y-1,x);
                }
            }
        }
    }
    else if(iAngle==180){
        ReSize(GetHDIBAdd(),iWidth,iHeight,iPixelBytes*8);
        for(int y=0;y<iHeight;y++){
            byte* pTargetLine=GetBits(y);
            for(int x=0;x<iWidth;x++){
                for(int i=0;i<iPixelBytes;i++){
                    *(pTargetLine+x*iPixelBytes+i)=*bmpTemp.GetBits(iWidth-x-1,iHeight-y-1);
                }
            }
        }
    }
    else if(iAngle==270){
        int iNewWidth=iHeight;
        int iNewHeight=iWidth;
        ReSize(GetHDIBAdd(),iNewWidth,iNewHeight,iPixelBytes*8);
        for(int y=0;y<iNewHeight;y++){
            byte* pTargetLine=GetBits(y);
            for(int x=0;x<iNewWidth;x++){
                for(int i=0;i<iPixelBytes;i++){
                    *(pTargetLine+x*iPixelBytes+i)=*bmpTemp.GetBits(y,iNewWidth-x-1);
                }
            }
        }
    }
    if(GetBitCount()<=8) SetPalette(GetBitCount());
}

// ��תͼ��
// a_bHorz:ˮƽ��תͼ��
// a_bVert:��ֱ��תͼ��
// 2011.11.17
void CBMP::MirrorImage(bool a_bHorz,bool a_bVert)
{
    if((!a_bHorz) && (!a_bVert)) return ;
    int iBits   =GetBitCount();
    int iPixelBytes=iBits>>3;
    int iWidth  =GetWidth();
    int iHeight =GetHeight();
    CBMP bmp;
    bmp.ReSize(bmp.GetHDIBAdd(),iWidth,iHeight,iBits);
    int iLineBytes=WIDTHBYTES(iBits*iWidth);
    int x,y;
    // ˮƽ��ת
    for(y=0;y<iHeight;y++){
        byte* pSourceLine=GetBits(y);
        byte* pTargetline=bmp.GetBits(a_bVert?(iHeight-y-1):y);
        if(a_bHorz){
            for(x=0;x<iWidth;x++){
                memcpy(pTargetline+(iWidth-x-1)*iPixelBytes,pSourceLine+x*iPixelBytes,iPixelBytes);
            }
        }
        else{
            if(a_bVert){
                memcpy(pTargetline,pSourceLine,iLineBytes);
            }
        }
    }
    Assign(bmp);
}

// ��DIBλͼת��Ϊ��׼ͼ�� 2016.2.26
// �������ͷ��ڴ�
void CBMP::TranDIB2LineImage(OUT byte** a_pOutImage)
{
    int iWidth=GetWidth();
    int iHeight=GetHeight();
    int iBytesPixel=GetBitCount()>>3;

    *a_pOutImage=new byte[iWidth*iHeight*iBytesPixel];

    for(int i=0;i<iHeight;i++){
        memcpy(*a_pOutImage+i*iWidth*iBytesPixel,GetBits(i),iWidth*iBytesPixel);
    }
}

// ����׼ͼ��ת��Ϊ DIB λͼ 2016.2.26
void CBMP::TranLineImage2DIB(byte* a_pImage,int a_iWidth,int a_iHeight,int a_iBit){
    ReSize(GetHDIBAdd(),a_iWidth,a_iHeight,a_iBit);
    int iBytesPixel=a_iBit>>3;
    for(int i=0;i<a_iHeight;i++){
        memcpy(GetBits(i),a_pImage+i*a_iWidth*iBytesPixel,a_iWidth*iBytesPixel);
    }
    if(GetBitCount()<=8) SetPalette(GetBitCount());
}

// �����ݰ���ȡͼ��
BOOL CBMP::LoadImageFromPack(CPackTreeBase* a_pPack,CString a_sImageName)
{
    CImageDataHeader idhImage;
    byte* pImage=a_pPack->GetImage(a_sImageName,idhImage);
    if(!pImage) return FALSE;
    if(!ReSize(GetHDIBAdd(),idhImage.m_iWidth,idhImage.m_iHeight,idhImage.m_iBit)) return FALSE;
    int iPixelBytes=idhImage.m_iBit>>3;
    int iLineBytes;
    if(idhImage.m_itType==IT_DIB) iLineBytes=WIDTHBYTES(idhImage.m_iWidth*idhImage.m_iBit);
    else                          iLineBytes=iPixelBytes*idhImage.m_iWidth;
    for(int y=0;y<idhImage.m_iHeight;y++){
        if(idhImage.m_itType==IT_DIB) memcpy(GetBits(idhImage.m_iHeight-y-1),pImage+iLineBytes*y,iLineBytes);
        else                          memcpy(GetBits(y),pImage+iLineBytes*y,iLineBytes);
    }
    return TRUE;
}

// �� HBITMAP ���һ��ͼ�� 2018.1.17
LPSTR CBMP::LoadImageFromHBITMAP(HBITMAP a_hBit,const int a_iBits){
    int iBits;
    //     CBitmap bitMap;
    //     bitMap.Attach(m_hBmp);
    //     BITMAP bitMapInfo;
    //     bitMap.GetBitmap(&bitMapInfo);
    //     iBits=bitMapInfo.bmBitsPixel;
    //     bitMap.Detach();
    iBits=a_iBits;
    //if(m_hDIB) iBits=(int)DIBBitCount(GetDIB());
    // �ͷ� DIB ռ�õ��ڴ�
    FreeDIB();
    //�豸������
    //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
    WORD            wBitCount;   
    //λͼ��ÿ��������ռ�ֽ���
    //�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ���
    DWORD           dwPaletteSize=0,  dwBmBitsSize;
    BITMAP          Bitmap;        
    //λͼ�ļ�ͷ�ṹ
    BITMAPINFOHEADER   bi;            
    //λͼ��Ϣͷ�ṹ 
    LPBITMAPINFOHEADER lpbi;          
    //�����ļ��������ڴ�������ɫ����
    //����λͼ�ļ�ÿ��������ռ�ֽ���
    //hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
    // iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    //DeleteDC(hDC);
    if (iBits <= 1)	        wBitCount = 1;
    else if (iBits <= 4)	wBitCount = 4;
    else if (iBits <= 8)    wBitCount = 8;
    else if (iBits <= 16)   wBitCount = 16;
    else if (iBits <= 24)   wBitCount = 24;
    else if (iBits <= 32)   wBitCount = 32;
    //�����ɫ���С
    if (wBitCount <= 8)	dwPaletteSize = (WORD)(1 <<  wBitCount) *sizeof(RGBQUAD);
    //����λͼ��Ϣͷ�ṹ
    GetObject(a_hBit, sizeof(BITMAP), (LPSTR)&Bitmap);
    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = Bitmap.bmWidth;
    bi.biHeight             = Bitmap.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = wBitCount;
    bi.biCompression        = BI_RGB;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;
    dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;
    //Ϊλͼ���ݷ����ڴ�
    m_hDIB  = (HDIB)(new _TCHAR[dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER)]);
    lpbi = (LPBITMAPINFOHEADER)m_hDIB;
    *lpbi = bi;
    // �����ɫ��   
    HDC            hDC;         
    hDC=NULL;
    HPALETTE hPal,hOldPal=NULL;
    hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
    if (hPal){
        hDC  = GetDC(NULL);
        //hDC=m_hMemDC;
        hOldPal = SelectPalette(hDC,hPal,FALSE);
        RealizePalette(hDC);
    }
    //hDC=m_hMemDC;
    // ��ȡ�õ�ɫ�����µ�����ֵ
    GetDIBits(hDC, a_hBit, 0, (UINT)Bitmap.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        +dwPaletteSize,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
    //�ָ���ɫ��   
    if (hOldPal){
        SelectPalette(hDC, hOldPal, TRUE);
        RealizePalette(hDC);
    }
    if(hDC){
        ReleaseDC(NULL, hDC);
        DeleteDC(hDC);
        hDC=NULL;
    }
    if(hPal) DeleteObject(hPal);
    return (LPSTR)lpbi;
}

// ����ͼƬ 2018.1.19
// a_pSource:Դͼ��
// a_iWidth: ���ź�Ŀ��
// a_iHeight:���ź�ĸ߶� 
void CBMP::ZoomImage(CBMP* a_pSource,int a_iWidth,int a_iHeight){
    int iBitCount=a_pSource->GetBitCount();
    ReSize(GetHDIBAdd(),a_iWidth,a_iHeight,iBitCount);
    TPicRegion prDst,prSrc;
    prDst.width=a_iWidth;
    prDst.height=a_iHeight;
    prDst.pdata=(TARGB32*)GetImageAddress();
    prDst.byte_width=WIDTHBYTES(a_iWidth*iBitCount);
    prSrc.width=a_pSource->GetWidth();
    prSrc.height=a_pSource->GetHeight();
    prSrc.pdata=(TARGB32*)a_pSource->GetImageAddress();
    prSrc.byte_width=WIDTHBYTES(a_pSource->GetWidth()*a_pSource->GetBitCount());
    switch(iBitCount){
    case 8:
        PicZoom8_Table(prDst,prSrc);
        break;
    case 24:
        PicZoom24_Table(prDst,prSrc);
        break;
    case 32:
        PicZoom32_Table(prDst,prSrc);
        break;
    }
}

void CBMP::ZoomImage(CBMP* a_pSource,double a_fHScale,double a_fVScale){
    int iWidth=(int)(a_pSource->GetWidth()*a_fHScale);
    int iHeight=(int)(a_pSource->GetHeight()*a_fVScale);
    ZoomImage(a_pSource,iWidth,iHeight);
}

// ���ļ������������͵�ͼ���ļ���bmp,jpeg,gif,tiff,png�ȣ�
bool CBMP::LoadImage(CString a_sImageFile){
    CImageConvert icConvert;
    if(icConvert.LoadFromFile(a_sImageFile)){
        return icConvert.SaveToBMPObject(this);
    }
    return false;
}

// ��������ͼ 2018.1.23
CSize CBMP::LoadThumbnailImage(CString a_sImageFile,const int a_iWidth,const int a_iHeight){
#ifdef _UNICODE
    Image img(a_sImageFile);
#else
    wstring sw=MutilByteToWide(a_sImageFile.GetBuffer(0));
    Image img(sw.c_str());
#endif
    CSize szSize(img.GetWidth(),img.GetHeight());
    Image* pThumbnail=img.GetThumbnailImage(a_iWidth,a_iHeight);
    if(!pThumbnail) return CSize(0,0);
    int iPixelFormat=img.GetPixelFormat();
    int iBits=24;
    switch(iPixelFormat){
    case PixelFormat1bppIndexed:iBits=1;
        break;
    case PixelFormat4bppIndexed:iBits=4;
        break;
    case PixelFormat8bppIndexed:iBits=8;
        break;
    case PixelFormat24bppRGB:   iBits=24;
        break;
    case PixelFormat32bppRGB:   iBits=32;
        break;
    }
    ReSize(GetHDIBAdd(),a_iWidth,a_iHeight,iBits);
    BeginDraw();
    Graphics graphics(GetMemDC());
    graphics.DrawImage(pThumbnail,0,0,a_iWidth,a_iHeight);
    EndDraw();

    delete pThumbnail;
    return szSize;
}

CSize CBMP::LoadThumbnailImage(CString a_sImageFile,const double a_fHScale,const double a_fVScale){
#ifdef _UNICODE
    Image img(a_sImageFile);
#else
    wstring sw=MutilByteToWide(a_sImageFile.GetBuffer(0));
    Image img(sw.c_str());
#endif
    CSize szSize(img.GetWidth(),img.GetHeight());
    int iWidth=(int)((double)szSize.cx*a_fHScale);
    int iHeight=(int)((double)szSize.cy*a_fVScale);
    Image* pThumbnail=img.GetThumbnailImage(iWidth,iHeight);
    if(!pThumbnail) return CSize(0,0);
    int iPixelFormat=img.GetPixelFormat();
    int iBits=24;
    switch(iPixelFormat){
    case PixelFormat1bppIndexed:iBits=1;
        break;
    case PixelFormat4bppIndexed:iBits=4;
        break;
    case PixelFormat8bppIndexed:iBits=8;
        break;
    case PixelFormat24bppRGB:   iBits=24;
        break;
    case PixelFormat32bppRGB:   iBits=32;
        break;
    }
    ReSize(GetHDIBAdd(),iWidth,iHeight,iBits);
    BeginDraw();
    Graphics graphics(GetMemDC());
    graphics.DrawImage(pThumbnail,0,0,iWidth,iHeight);
    EndDraw();

    delete pThumbnail;
    return szSize;
}


// ���ڴ�����������͵�ͼ���ļ�
bool CBMP::LoadImageFromMemory(byte* a_pBuffer,__int64 a_iSize){
    CImageConvert icConvert;
    if(icConvert.LoadFromMemory(a_pBuffer,a_iSize)){
        return icConvert.SaveToBMPObject(this);
    }
    return false;
}

// ���浽�ڴ�
bool CBMP::SaveToMemory(SunMemoryStream* a_pMemory,CString a_sFormat,UINT a_iQuality){
    CImageConvert icConvert;
    if(icConvert.LoadFromBMPObject(this)){
        return icConvert.SaveToMemory(a_pMemory,a_sFormat,a_iQuality);
    }
    return false;
}

// ���浽�ڴ�(��Ҫ���������ڴ�)
bool CBMP::SaveToMemory(CString a_sFormat,UINT a_iQuality,OUT byte** a_pBuffer,OUT __int64* a_iSize){
    CImageConvert icConvert;
    if(icConvert.LoadFromBMPObject(this)){
        return icConvert.SaveToMemory(a_sFormat,a_iQuality,a_pBuffer,a_iSize);
    }
    return false;
}

// ����Ϊ�ļ�
bool CBMP::SaveToFile(CString a_sFileName,CString a_sFormat,UINT a_iQuality){
    CImageConvert icConvert;
    if(icConvert.LoadFromBMPObject(this)){
        return icConvert.SaveToFile(a_sFileName,a_sFormat,a_iQuality);
    }
    return false;
}


}