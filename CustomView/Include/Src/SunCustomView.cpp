#include "stdafx.h"
#include "SunCustomView.h"

namespace sunLib{

//////////////////////////////////////////////////////////////////////////
// CCustomViewImage
//////////////////////////////////////////////////////////////////////////

void CCustomViewImageZoom(CBMP* a_pImage,CBMP* a_pTargetImage,double a_fZoom,double a_fScale){
    int iWidth=(int)(a_pImage->GetWidth()*a_fZoom);
    int iHeight=(int)(a_pImage->GetHeight()*a_fZoom);
    a_pTargetImage->ReSize(a_pTargetImage->GetHDIBAdd(),iWidth,iHeight,(byte)a_pImage->GetBitCount());
    CRect rSource=a_pImage->GetRect();
    CRect rTarget=CRect(0,0,iWidth,iHeight);

    if(a_pTargetImage->GetBitCount()<=8) a_pTargetImage->SetPalette(a_pTargetImage->GetBitCount());
    int iPixelBytes=a_pTargetImage->GetBitCount()/8;
    int iSourceWidthBytes=WIDTHBYTES(a_pImage->GetWidth()*a_pImage->GetBitCount());
    // 强制为 2 倍 Binning
#pragma omp parallel for
    for(int y=0;y<iHeight;y++){
        int* pSum=new int[iPixelBytes];
        byte* pTargetAddr=a_pTargetImage->GetBits(y);
        byte* pSourceLine=a_pImage->GetBits(y*2);
        for(int x=0;x<iWidth;x++){
            memset(pSum,0,sizeof(int)*iPixelBytes);
            int iOfficeX=x*2*iPixelBytes;
            for(int j=0;j<2;j++){
                for(int i=0;i<2;i++){
                    //byte* pSourceAddr=a_pImage->GetBits(x*2+i,y*2+j);
                    byte* pSourceAddr=pSourceLine+iOfficeX+iSourceWidthBytes*j+i*iPixelBytes;
                    for(int k=0;k<iPixelBytes;k++){
                        pSum[k]+=pSourceAddr[k];
                    }
                }
            }
            for(int k=0;k<iPixelBytes;k++){
                pTargetAddr[x*iPixelBytes+k]=pSum[k]/4;
            }
        }
        delete[] pSum;
    }
}

// 设置图像
// a_fZoom: 缩放比例
// a_fScale:相对原图的缩放比例
void CCustomViewImage::SetImage(CBMP* a_pImage,double a_fZoom,double a_fScale){
    m_fScale=a_fScale;
    if(a_fScale>=1) m_bmpImage.Assign(*a_pImage);
    //else CCustomViewImageZoom(a_pImage,&m_bmpImage,a_fZoom,a_fScale);
    else m_bmpImage.ZoomImage(a_pImage,a_fZoom,a_fZoom);
    if(m_bmpImage.GetBitCount()<=8) m_bmpImage.SetPalette(m_bmpImage.GetBitCount());

    //         m_bmpImage.BeginDraw();
    //         SetStretchBltMode(m_bmpImage.GetMemDC(),STRETCH_HALFTONE);
    //         PaintDIB(m_bmpImage.GetMemDC(),rTarget,a_pImage->GetHDIB(),rSource,a_pImage->GetPalette());
    //         m_bmpImage.EndDraw();
}



//////////////////////////////////////////////////////////////////////////
// CImageBuffer
//////////////////////////////////////////////////////////////////////////

// 获得指定放大率下的图像指针和放大率
CCustomViewImage* CImageBuffer::GetScaleImage(double a_fScale)
{
    if(m_aImageList.GetItemCount()<=0) return NULL;
    int iIndex=0;
    CCustomViewImage* pImage=NULL;
    for(int i=m_aImageList.GetItemCount()-1;i>=0;i--){
        pImage=(CCustomViewImage*)m_aImageList.GetValue(i);
        if(a_fScale>=1){
            if(pImage->m_fScale>=1){
                iIndex=i;
                break;
            }
        }
        if(a_fScale<=pImage->m_fScale){
            iIndex=i;
            break;
        }
    }
    return (CCustomViewImage*)m_aImageList.GetValue(iIndex);
}

// 添加图像
void CImageBuffer::AddImage(CBMP* a_pImage){
    m_szImage.cx=a_pImage->GetWidth();
    m_szImage.cy=a_pImage->GetHeight();
    CBMP* pOldImage=a_pImage;
    // 添加原图
    CCustomViewImage* pImage=new CCustomViewImage;
    pImage->SetImage(pOldImage,1,1);
    m_aImageList.AddItem(pImage);
    for(int i=1;i<5;i++){
        pImage=new CCustomViewImage;
        double fScale=1.0f/pow(3.0f,i);
        pImage->SetImage(pOldImage,1.0f/3.0f,fScale);
        pOldImage=pImage->GetImage();
        m_aImageList.AddItem(pImage);
    }
}

// 将 1 位位图转换为 8 位位图
void Tran1BitTo8Bit(CBMP& a_bmp){
    int iWidth=a_bmp.GetWidth();
    int iHeight=a_bmp.GetHeight();
    CBMP bmpTarget;
    bmpTarget.ReSize(bmpTarget.GetHDIBAdd(),iWidth,iHeight,8);
    for(int y=0;y<iHeight;y++){
        byte* pSourceLine=a_bmp.GetBits(y);
        byte* pTargetLine=bmpTarget.GetBits(y);
        for(int x=0;x<iWidth;x+=8){
            byte* pTarget=pTargetLine+x;
            byte bySource=*(pSourceLine+x/8);
            for(int i=0;i<8;i++){
                if(((bySource>>i)&0x01)>0){
                    pTarget[i]=255;
                }
                else{
                    pTarget[i]=0;
                }
            }
        }
    }
}

void MyLoadImage(CBMP& a_bmp,CString a_sImageFile){
    CString sExt=a_sImageFile.Right(4);
    if(sExt.MakeLower()==_T(".bmp")){
        a_bmp.ReadDIBFile(a_sImageFile);
        if(a_bmp.GetBitCount()==1){
            Tran1BitTo8Bit(a_bmp);
        }
        if(a_bmp.GetBitCount()<=8){
            a_bmp.SetPalette(a_bmp.GetBitCount());
        }
    }
    else{
        a_bmp.LoadImage(a_sImageFile);
    }
}

void CImageBuffer::AddImage(CString a_sImageFile){
    CBMP bmpImage;
    MyLoadImage(bmpImage,a_sImageFile);
    AddImage(&bmpImage);
}

// 添加指定指定尺寸的图像到缓冲
// 如果尺寸和原始图像一致
void CImageBuffer::AddImageThumb(CString a_sImageFile){
#ifdef _UNICODE
    Image img(a_sImageFile);
#else
    Image img(MutilByteToWide(a_sImageFile.GetBuffer(0)).c_str());
#endif
    m_szImage.cx=img.GetWidth();
    m_szImage.cy=img.GetHeight();
    int iThumbWidth=200;
    int iThumbHeight=(int)((double)iThumbWidth*((double)m_szImage.cy/(double)m_szImage.cx));
    double fScale=(double)iThumbWidth/(double)img.GetWidth();

    CBMP* pBMP=new CBMP;
    CSize szSize=pBMP->LoadThumbnailImage(a_sImageFile,iThumbWidth,iThumbHeight);
    if(szSize.cx>0){
        CCustomViewImage* pImage=new CCustomViewImage;
        pImage->SetImage(pBMP,1.0f,fScale);
        m_aImageList.AddItem(pImage);
    }
    delete pBMP;
}

// 设置图像 2016.7.19
// 输入的图像可以自行销毁
void CImageBuffer::SetImage(CBMP* a_pImage){
    m_aImageList.Clear();
    m_szImage.cx=a_pImage->GetWidth();
    m_szImage.cy=a_pImage->GetHeight();
    AddImage(a_pImage);
}

void CImageBuffer::SetImage(CString a_sImageFile){
    CBMP bmpImage;
    MyLoadImage(bmpImage,a_sImageFile);
    SetName(a_sImageFile);
    SetImage(&bmpImage);
}

// 设置指定指定尺寸的图像到缓冲
// 如果尺寸和原始图像一致
void CImageBuffer::SetImageThumb(CString a_sImageFile){
    m_aImageList.Clear();
    SetName(a_sImageFile);
    AddImageThumb(a_sImageFile);
}


//////////////////////////////////////////////////////////////////////////
// CImageManage
//////////////////////////////////////////////////////////////////////////

CCustomViewImage* CImageManage::GetCurrImage(){
    CImageBuffer* pBuffer=GetCurrImageBuffer();
    if(!pBuffer) return NULL;
    return pBuffer->GetImage();
}

// 返回当前图像缓冲
CImageBuffer* CImageManage::GetCurrImageBuffer(){
    CImageBuffer* pBuffer=NULL;
    Lock();
    pBuffer=(CImageBuffer*)m_aImageList.GetValueOfName(m_sCurrImageName);
    Unlock();
    if(pBuffer) return pBuffer;

    Lock();
    pBuffer=(CImageBuffer*)m_aImageList.GetValueOfName(m_sPriorImageName);
    Unlock();
    return pBuffer;
}

// 添加图像到缓存
// 输入的图像不能自行销毁
void CImageManage::AddImage(CImageBuffer* a_pImageBuffer){
    Lock();
    m_aImageList.AddItem(a_pImageBuffer);
    Unlock();
}

// 设置当前图像
void CImageManage::SetCurrImage(CString a_sImageName){
    m_sPriorImageName=m_sCurrImageName;
    m_sCurrImageName=a_sImageName;
}

// 获得正常图像尺寸
CSize CImageManage::GetCurrImageSize(){
    CImageBuffer* pBuffer=GetCurrImageBuffer();
    if(pBuffer) return pBuffer->GetImageSize();
    return CSize(0,0);
}

// 添加的规则是：
// 1.不能删除当前图像
// 2.不能大于最大数量，如果大于则需要删除一张最老的图像，但不能是当前图像
void CImageManage::AddImage(CString a_sImageFile){
    CImageBuffer* pBuffer=(CImageBuffer*)m_aImageList.GetValueOfName(a_sImageFile);
    if(pBuffer){    // 是否图像已经存在
        m_pDealImageInterface->OnDraw();
        return;
    }
    CProTimer tim;
    CImageBuffer* pImageBuffer=new CImageBuffer;

    // 先处理缩略图
    pImageBuffer->SetImageThumb(a_sImageFile);
    AddImage(pImageBuffer);
    m_pDealImageInterface->OnDraw();

    // 加载正常图像
    pImageBuffer->AddImage(a_sImageFile);
    //AddImage(pImageBuffer);
    m_pDealImageInterface->OnDraw();
    Lock();
    if(m_aImageList.GetItemCount()>m_iMaxImageCount){
        // 需要删除一张
        for(int i=0;i<m_aImageList.GetItemCount();i++){
            CImageBuffer* pBuffer=(CImageBuffer*)m_aImageList.GetValue(i);
            if(pBuffer){
                if(pBuffer->GetName()!=m_sCurrImageName){
                    m_aImageList.DelItem(i);
                    break;
                }
            }
        }
    }
    Unlock();
    if(m_bDebugMode) OutputDebugString(Format(_T("处理 %s 花费 %.2fms"),a_sImageFile,tim.GetTime(true)*1000));
}


//////////////////////////////////////////////////////////////////////////
// CCustomViewEx 2016.6.22
//////////////////////////////////////////////////////////////////////////

CCustomViewEx::CCustomViewEx(void)
{
    m_bDebugMode    =false;
    m_fScale        =1;
    m_pOwner        =NULL;
    m_rRect         =CRect(0,0,0,0);
    m_pHScrollBar   =NULL;
    m_pVScrollBar   =NULL;
    m_bMouseDown    =false;
    m_pntMouseOld   =CPoint(0,0);
    m_pntScrollPos  =CPoint(0,0);
    m_rRealRect     =CRect(0,0,0,0);
    m_pntCenterPos  =CPoint(0,0);
    m_pntOldCenterPos=CPoint(0,0);
    m_pCustomViewRefreshCallback    =NULL;
    m_pRefreshCallbackOwner         =NULL;
    m_bCenteredShow =false;
    m_pntScroll     =CPoint(0,0);
    m_bUseBackground=false;
    m_dwBackColor1  =RGB(0,0,0);
    m_dwBackColor2  =RGB(0,0,0);
    m_iGridWidth    =30;
    m_iGridHeight   =30;
    m_szPriorImage  =CSize(0,0);

    m_bShowRGBRealValue         =false;
    m_fShowRGBRealValueScale    =40;
    m_iShowRGBRealValueFontSize =14;
    m_bExit             =false;
    m_hLoadImageThread  =NULL;
    m_dwLoadImageThreadID=0;
    m_hShowImageThread  =NULL;
    m_dwShowImageThreadID=0;
    m_pDealImageInterface=NULL;

    m_hdd=DrawDibOpen();
    m_evtLoadImage.ResetEvent();
    m_evtShowImage.ResetEvent();
    m_imImageManage.SetImageDealInterface(this);
    BeginLoadImageThread();
    BeginShowImageThread();
}


CCustomViewEx::~CCustomViewEx(void)
{
    m_bExit=true;
    m_evtLoadImage.SetEvent();
    if(m_hLoadImageThread) WaitForSingleObject(m_hLoadImageThread,2000);
    DrawDibClose(m_hdd);
}

// 获得当前显示的图像
CBMP* CCustomViewEx::GetCurrImage(){
    if(m_imImageManage.IsEmpty()) return NULL;
    CCustomViewImage* pImage=m_imImageManage.GetCurrImage();
    if(!pImage) return NULL;
    return pImage->GetImage();
}

// 返回当前图像缓冲
CImageBuffer* CCustomViewEx::GetCurrImageBuffer(){
    return m_imImageManage.GetCurrImageBuffer();
}

// 获得图像宽度
int CCustomViewEx::GetWidth(){
    return m_imImageManage.GetCurrImageSize().cx;
//     CBMP* pImage=GetCurrImage();
//     if(!pImage) return 0;
//     return pImage->GetWidth();
}

// 获得图像高度
int CCustomViewEx::GetHeight(){
    return m_imImageManage.GetCurrImageSize().cy;
//     CBMP* pImage=GetCurrImage();
//     if(!pImage) return 0;
//     return pImage->GetHeight();
}

// 将屏幕坐标转换为图像坐标
// 输入的原点必须保持和显示区域的原点一致
// 超出图像范围则返回 -1,-1
CPoint CCustomViewEx::TranScreenCoorToImage(const CPoint a_pntPoint){
    CPoint pntMouse=a_pntPoint-m_rRect.TopLeft();
    CPoint pntPoint(-1,-1);
    CBMP* pCurrImage=GetScaleImage(1.0f);
    if(!pCurrImage) return pntPoint;
    CRect rShowRect=SpeciRectLimit(m_rRealRect,pCurrImage->GetRect());

    CSize szViewPos;
    szViewPos.cx    =(int)(m_rRealRect.Width()*m_fScale);
    szViewPos.cy    =(int)(m_rRealRect.Height()*m_fScale);
    CRect rRealRect(m_rRealRect);
    CRect rRefShowRect=CRect(0,0,szViewPos.cx,szViewPos.cy);
    // 处理绘制的区域超界问题 2016.7.19
    if(rRealRect.left<0){
        rRefShowRect.left=-m_pntScroll.x;
    }
    if(rRealRect.top<0){
        rRefShowRect.top=-m_pntScroll.y;
    }

    // 绘制鼠标的当前位置框
    CPoint pntOffset;
    pntOffset.x=(int)((pntMouse.x-rRefShowRect.left)/m_fScale);
    pntOffset.y=(int)((pntMouse.y-rRefShowRect.top)/m_fScale);
    CPoint pntReal;
    pntReal.x=pntOffset.x+rShowRect.left;
    pntReal.y=pntOffset.y+rShowRect.top;

    if(pntReal.x<0) return pntPoint;
    if(pntReal.y<0) return pntPoint;
    if(pntReal.x>=pCurrImage->GetWidth()) return pntPoint;
    if(pntReal.y>=pCurrImage->GetHeight()) return pntPoint;
    return pntReal;
}

// 获得指定位置的颜色 2018.2.5
COLORREF CCustomViewEx::GetColor(const CPoint a_pntPoint){
    CBMP* pCurrImage=GetScaleImage(1.0f);
    if(!pCurrImage) return -1;
    CPoint pntReal=TranScreenCoorToImage(a_pntPoint);
    if(pntReal.x<0) return -1;
    byte* pAddr=pCurrImage->GetBits(pntReal.x,pntReal.y);
    int iBit=pCurrImage->GetBitCount();
    COLORREF dwColor;
    if(iBit==8) dwColor=(*pAddr);
    if(iBit==32 || iBit==24){
        byte R,G,B;
        B=*pAddr;
        G=*(pAddr+1);
        R=*(pAddr+2);
        dwColor=RGB(R,G,B);
    }
    return dwColor;
}

void CCustomViewEx::SetScrollBarInfo(CScrollBar* a_pScrollBar,const int a_iMax,bool a_bHor)
{
    if(!a_pScrollBar) return;
    int iPagePoint=100;
    tagSCROLLINFO siInfo;
    a_pScrollBar->GetScrollInfo(&siInfo);
    siInfo.cbSize   =sizeof(SCROLLINFO);
    siInfo.nMin     =0;
    siInfo.nMax     =a_iMax-(a_bHor?m_rRect.Width():m_rRect.Height())+(int)(iPagePoint*m_fScale);
    siInfo.nPage    =(int)(iPagePoint*m_fScale);
    siInfo.nPos     =(int)(siInfo.nPos*m_fScale);
    siInfo.fMask    =SIF_ALL;
    a_pScrollBar->SetScrollInfo(&siInfo);
}

void CCustomViewEx::ReSetScrollBar()
{
    CBMP* pCurrImage=GetCurrImage();
    if(!pCurrImage) return ;
    if(m_pHScrollBar) SetScrollBarInfo(m_pHScrollBar,(int)(GetWidth()*m_fScale),true);
    if(m_pVScrollBar) SetScrollBarInfo(m_pVScrollBar,(int)(GetHeight()*m_fScale),false);
    double fWidth=m_rRect.Width()/2.0f;
    double fHeight=m_rRect.Height()/2.0f;
    if(m_pHScrollBar) m_pHScrollBar->SetScrollPos(m_pntCenterPos.x-(int)fWidth);
    if(m_pVScrollBar) m_pVScrollBar->SetScrollPos(m_pntCenterPos.y-(int)fHeight);
    m_pntScroll=CPoint(m_pntCenterPos.x-(int)fWidth,m_pntCenterPos.y-(int)fHeight);
}

// 添加图像文件，支持缓冲
void CCustomViewEx::AddImageFile(CString a_sImageFile){
    if(!FileExists(a_sImageFile)) return;
    LockImageFileList();
    m_aImageFile.Add(a_sImageFile);
    UnlockImageFileList();
    m_evtLoadImage.SetEvent();
}

// 设置当前图像
void CCustomViewEx::SetCurrImage(CString a_sImageName){
    m_imImageManage.SetCurrImage(a_sImageName);
    m_evtShowImage.SetEvent();
}

// 设置图像 2016.7.19
// 设置需要显示的图像，此模式不支持缓冲
// 输入的图像需要自行销毁
void CCustomViewEx::SetImage(CBMP* a_pViewImage){
    m_cs.Lock();
    CImageBuffer* pImageBuffer=new CImageBuffer;
    pImageBuffer->SetName(_T("1"));
    pImageBuffer->SetImage(a_pViewImage);
    m_imImageManage.Clear();
    m_imImageManage.AddImage(pImageBuffer);
    m_imImageManage.SetCurrImage(_T("1"));
    m_cs.Unlock();
//     CBMP* pOldImage=a_pViewImage;
//     m_aImageList.Clear();
//     for(int i=1;i<5;i++){
//         CCustomViewImage* pImage=new CCustomViewImage;
//         double fScale=1.0f/pow(2.0f,i);
//         pImage->SetImage(pOldImage,0.5f,fScale);
//         pOldImage=pImage->GetImage();
//         m_aImageList.AddItem(pImage);
//     }
}

// 清空图像显示 2018.4.28
void CCustomViewEx::ClearImage(){
    m_imImageManage.Clear();
}

// 初始化相关数据 2016.2.24
void CCustomViewEx::InitControl(CWnd* a_pOwner,CRect a_rRect,CScrollBar* a_pHScrollBar,CScrollBar* a_pVScrollBar)
{
    m_pOwner        =a_pOwner;
    m_rRect         =a_rRect;
    m_pHScrollBar   =a_pHScrollBar;
    m_pVScrollBar   =a_pVScrollBar;
    if(m_fScale>0.99 && m_fScale<1.01){
        m_pntCenterPos.x=a_rRect.Width()/2;
        m_pntCenterPos.y=a_rRect.Height()/2;
    }
    m_pntOldCenterPos=m_pntCenterPos;


    ChangeSize(a_rRect);
}

void DrawGrid(CDC* a_pDC,CRect a_rRect,bool a_bFlag,COLORREF a_dwColor1,COLORREF a_dwColor2,int a_iGridWidth,int a_iGridHeight){
    int iGridWidth=a_iGridWidth;
    int iGridHeight=a_iGridHeight;

    CPen pen(PS_SOLID,1,a_bFlag?a_dwColor1:a_dwColor2);
    CPen* pOldPen=a_pDC->SelectObject(&pen);
    CBrush brush(a_bFlag?a_dwColor1:a_dwColor2);
    CBrush* pOldBrush=a_pDC->SelectObject(&brush);
    for(int y=0;y<a_rRect.Height()+iGridHeight-1;y+=iGridHeight){
        for(int x=0;x<a_rRect.Width()+iGridWidth-1;x+=iGridWidth){
            int iY=y/iGridHeight;
            int iX=x/iGridWidth;
            bool bDraw=true;
            if(a_bFlag){
                if((iY % 2)==0) bDraw=((iX % 2)==0);
                else           bDraw=((iX % 2)==1);
            }
            else{
                if((iY % 2)==0) bDraw=((iX % 2)==1);
                else           bDraw=((iX % 2)==0);
            }
            if(bDraw){
                CRect r;
                r.left=x;
                r.top=y;
                r.right=x+iGridWidth;
                r.bottom=y+iGridHeight;
                a_pDC->Rectangle(r);
            }
        }
    }
    a_pDC->SelectObject(pOldPen);
    a_pDC->SelectObject(pOldBrush);
}

// 是否自定义背景，注意，一定要先调用 InitControl 2016.7.23
void CCustomViewEx::UseBackground(bool a_bUseBackground,COLORREF a_dwColor1,COLORREF a_dwColor2,int a_iGridWidth,int a_iGridHeight)
{
    m_bUseBackground    =a_bUseBackground;
    m_dwBackColor1      =a_dwColor1;
    m_dwBackColor2      =a_dwColor2;
    m_iGridWidth        =a_iGridWidth;
    m_iGridHeight       =a_iGridHeight;
    // 准备背景 2016.7.23
//     CDC dc;
//     HDC hDC=GetDC(NULL);
//     dc.Attach(hDC);
//     m_cnvBack.Destroy();
//     m_cnvBack.Create(&dc,m_rRect.Width(),m_rRect.Height());
//     dc.Detach();
//     DeleteDC(hDC);
    m_bmpBack.ReSize(m_bmpBack.GetHDIBAdd(),m_rRect.Width(),m_rRect.Height(),24);
    m_bmpBack.BeginDraw();

    CRect r;
    r.left  =m_rRect.left+1;
    r.top   =m_rRect.top+1;
    r.right =m_rRect.right-1;
    r.bottom=m_rRect.bottom-1;
    CDC dc;
    dc.Attach(m_bmpBack.GetMemDC());
    DrawGrid(&dc,r,true,a_dwColor1,a_dwColor2,m_iGridWidth,m_iGridHeight);
    DrawGrid(&dc,r,false,a_dwColor1,a_dwColor2,m_iGridWidth,m_iGridHeight);
    dc.Detach();
    m_bmpBack.EndDraw();
//     DrawGrid(m_cnvBack.GetDC(),r,true,a_dwColor1,a_dwColor2,m_iGridWidth,m_iGridHeight);
//     DrawGrid(m_cnvBack.GetDC(),r,false,a_dwColor1,a_dwColor2,m_iGridWidth,m_iGridHeight);
}

// 获得指定放大率的图像
CBMP* CCustomViewEx::GetScaleImage(const double a_fScale){
    CImageBuffer* pImageBuffer=m_imImageManage.GetCurrImageBuffer();
    if(!pImageBuffer) return NULL;
    CCustomViewImage* pViewImage=pImageBuffer->GetScaleImage(a_fScale);
    if(!pViewImage) return NULL;
    return pViewImage->GetImage();
}

// 获得当前放大率下的图像指针和放大率
CBMP* CCustomViewEx::GetCurrScaleImage(OUT double* a_fScale)
{
    CImageBuffer* pImageBuffer=m_imImageManage.GetCurrImageBuffer();
    if(!pImageBuffer) return NULL;
    CCustomViewImage* pViewImage=pImageBuffer->GetScaleImage(m_fScale);
    *a_fScale=pViewImage->m_fScale;
    if(!pViewImage) return NULL;
    return pViewImage->GetImage();
//     if(m_fScale>=1){
//         *a_fScale=m_fScale;
//         return pCurrImage;
//     }
//     else{
//         CCustomViewImage* pImage=NULL;
//         for(int i=0;i<m_aImageList.GetItemCount();i++){
//             pImage=(CCustomViewImage*)m_aImageList.GetValue(i);
//             if(m_fScale>pImage->m_fScale){
//                 break;
//             }
//         }
//         if(!pImage){
//             if(m_aImageList.GetItemCount()>0){
//                 pImage=(CCustomViewImage*)m_aImageList.GetValue(m_aImageList.GetItemCount()-1);
//             }
//         }
//         if(pImage){
//             *a_fScale=pImage->m_fScale;
//             return &pImage->m_bmpImage;
//         }
//     }
//     return NULL;
}

// 实际绘制 2016.2.24
void CCustomViewEx::RealDraw(CDC* a_pDC,CRect a_rOutRect){
    if(m_rRealRect.Width()<=0) SetAutoFit();
    
    CRect rClient(m_rRect);
    CCanvas canvasClient;
    canvasClient.Create(a_pDC,rClient.Width(),rClient.Height());
    SetStretchBltMode(canvasClient.GetDC()->m_hDC,STRETCH_HALFTONE);
//    SetStretchBltMode(canvasClient.GetDC()->m_hDC,COLORONCOLOR);
    canvasClient.GetDC()->SetBkMode(TRANSPARENT);
    if(m_bUseBackground) {
        //CProTimer t;
        //BitBlt(canvasClient.GetDC()->m_hDC,1,1,m_rRect.Width()-2,m_rRect.Height()-2,m_bmpBack.GetMemDC(),0,0,SRCCOPY);
        PaintDIB(canvasClient.GetDC()->m_hDC,CRect(0,0,m_rRect.Width(),m_rRect.Height()),m_bmpBack.GetHDIB(),m_bmpBack.GetRect(),m_bmpBack.GetPalette());
        //OutputDebugString(Format("%2.2fms",t.GetTime(false)*1000));
    }

    CSize szViewPos;
    CRect rRealRect(m_rRealRect);
    CRect rShowRect;

    if(!GetCurrImage()) goto End ;
    int iImageWidth=GetWidth();
    int iImageHeight=GetHeight();
    if(iImageWidth<=0) SetAutoFit();


    // 计算需要显示的位置
    szViewPos.cx    =(int)(m_rRealRect.Width()*m_fScale);
    szViewPos.cy    =(int)(m_rRealRect.Height()*m_fScale);

    rShowRect=CRect(0,0,szViewPos.cx,szViewPos.cy);
    if(m_bCenteredShow){

    }
    // 处理绘制的区域超界问题 2016.7.19
    if(rRealRect.left<0){
        rRealRect.left=0;
        rShowRect.left=-m_pntScroll.x;
    }
    if(rRealRect.top<0){
        rRealRect.top=0;
        rShowRect.top=-m_pntScroll.y;
    }
    if(rRealRect.right>iImageWidth){
        int iOffset=rRealRect.right-iImageWidth;
        rRealRect.right=iImageWidth;
        rShowRect.right-=(int)(iOffset*m_fScale);
    }
    if(rRealRect.bottom>iImageHeight){
        int iOffset=rRealRect.bottom-iImageHeight;
        rRealRect.bottom=iImageHeight;
        rShowRect.bottom-=(int)(iOffset*m_fScale);
    }

    double fScale=1;
    m_cs.Lock();
    CBMP* pImage=GetCurrScaleImage(&fScale);
    if(fScale<1){
        rRealRect.left=(int)(rRealRect.left*fScale);
        rRealRect.top=(int)(rRealRect.top*fScale);
        rRealRect.right=(int)(rRealRect.right*fScale);
        rRealRect.bottom=(int)(rRealRect.bottom*fScale);
    }
    //CProTimer t;
    if(pImage) PaintDIB(canvasClient.GetDC()->m_hDC,rShowRect,pImage->GetHDIB(),rRealRect,pImage->GetPalette());
    m_cs.Unlock();
//     if(pImage) QuickDrawDibDraw(canvasClient.GetDC()->m_hDC,rShowRect,pImage->GetHDIB(),rRealRect,pImage->GetPalette());
    //OutputDebugString(Format("PaintDIB: %2.2fms",t.GetTime(true)*1000));
    // 绘制颜色值 2016.7.28
    if(m_bShowRGBRealValue && m_fScale>=m_fShowRGBRealValueScale){
        DrawRealValue(canvasClient.GetDC(),rShowRect,m_rRealRect);
    }
End:
    if(a_rOutRect.Width()>0) canvasClient.OutToDC(a_pDC,a_rOutRect);
    else canvasClient.OutToDC(a_pDC,CRect(0,0,rClient.Width(),rClient.Height()));
    //OutputDebugString(Format("OutToDC: %2.2fms",t.GetTime(true)*1000));
}
// void CCustomViewEx::RealDraw(CDC* a_pDC,CRect a_rOutRect){
//     if(!m_pViewImage) return;
// //     CRect rClient(m_rRect);
// //     canvasClient.Create(a_pDC,rClient.Width(),rClient.Height());
// //     SetStretchBltMode(canvasClient.GetDC()->m_hDC,STRETCH_HALFTONE);
// //     canvasClient.GetDC()->SetBkMode(TRANSPARENT);
//     if(m_bUseBackground) BitBlt(a_pDC->m_hDC,1+a_rOutRect.left,1+a_rOutRect.top,m_rRect.Width()-2,m_rRect.Height()-2,m_cnvBack.GetDC()->m_hDC,0,0,SRCCOPY);
// 
//     // 计算需要显示的位置
//     CSize szViewPos;
//     szViewPos.cx=(int)(m_rRealRect.Width()*m_fScale);
//     szViewPos.cy=(int)(m_rRealRect.Height()*m_fScale);
// 
//     CRect rRealRect(m_rRealRect);
// 
//     CRect rShowRect=CRect(0,0,szViewPos.cx,szViewPos.cy);
//     if(m_bCenteredShow){
// 
//     }
//     // 处理绘制的区域超界问题 2016.7.19
//     if(rRealRect.left<0){
//         rRealRect.left=0;
//         rShowRect.left=-m_pntScroll.x;
//     }
//     if(rRealRect.top<0){
//         rRealRect.top=0;
//         rShowRect.top=-m_pntScroll.y;
//     }
//     if(rRealRect.right>m_pViewImage->GetWidth()){
//         int iOffset=rRealRect.right-m_pViewImage->GetWidth();
//         rRealRect.right=m_pViewImage->GetWidth();
//         rShowRect.right-=(int)(iOffset*m_fScale);
//     }
//     if(rRealRect.bottom>m_pViewImage->GetHeight()){
//         int iOffset=rRealRect.bottom-m_pViewImage->GetHeight();
//         rRealRect.bottom=m_pViewImage->GetHeight();
//         rShowRect.bottom-=(int)(iOffset*m_fScale);
//     }
// 
//     double fScale=1;
//     CBMP* pImage=GetCurrScaleImage(&fScale);
//     if(fScale<1){
//         rRealRect.left=(int)(rRealRect.left*fScale);
//         rRealRect.top=(int)(rRealRect.top*fScale);
//         rRealRect.right=(int)(rRealRect.right*fScale);
//         rRealRect.bottom=(int)(rRealRect.bottom*fScale);
//     }
// 
//     if(pImage) PaintDIB(a_pDC->m_hDC,rShowRect+a_rOutRect.TopLeft(),pImage->GetHDIB(),rRealRect,pImage->GetPalette());
//     // 绘制颜色值 2016.7.28
//     if(m_bShowRGBRealValue && m_fScale>=m_fShowRGBRealValueScale){
//         DrawRealValue(a_pDC,rShowRect+a_rOutRect.TopLeft(),m_rRealRect);
//     }
//     //if(a_rOutRect.Width()>0) canvasClient.OutToDC(a_pDC,a_rOutRect);
//     //else canvasClient.OutToDC(a_pDC,CRect(0,0,rClient.Width(),rClient.Height()));
// }

// 绘制真值到屏幕 2016.7.28
void CCustomViewEx::DrawRealValue(CDC* a_pDC,CRect a_rShowRect,CRect a_rRealRect){
    if(a_rRealRect.Width()<=0) return;
    //CBMP* pCurrImage=GetCurrImage();
    CBMP* pCurrImage=GetScaleImage(1.0f);
    if(!pCurrImage) return ;
    CBrush brush;
    LOGBRUSH logBrush;
    logBrush.lbStyle = BS_NULL;
    brush.CreateBrushIndirect(&logBrush);
    a_pDC->SelectObject(brush);

    CRect rShowRect=SpeciRectLimit(m_rRealRect,pCurrImage->GetRect());

//     CPoint pntReal=TranScreenCoorToImage(m_pntCurrMouse);        // 绘制鼠标当前的格子

    int iBit=pCurrImage->GetBitCount();
    for(int y=rShowRect.top;y<rShowRect.bottom;y++){
        for(int x=rShowRect.left;x<rShowRect.right;x++){
            int iX=x-rShowRect.left;                        // 绘制图像的位置
            int iY=y-rShowRect.top;                         //
            CRect rText;
            rText.left  =a_rShowRect.left+(int)(iX*m_fScale);
            rText.top   =a_rShowRect.top+(int)(iY*m_fScale);
            rText.right =rText.left+(int)m_fScale;
            rText.bottom=rText.top+(int)m_fScale;
            rText.InflateRect(-1,-1);
            byte* p=pCurrImage->GetBits(x,y);
            CString s;
            COLORREF dwColor=RGB(0,0,0);
            if(iBit==8){
                s.Format(_T("%d"),*p);
                if(*p>128) dwColor=RGB(0,0,0);
                else       dwColor=RGB(255,255,255);
            }
            else if(iBit==24 || iBit==32){
                byte R=*(p+2);
                byte G=*(p+1);
                byte B=*(p+0);
                s.Format(_T("%d\n%d\n%d"),R,G,B);
                if(R+G+B>400) dwColor=RGB(0,0,0);
                else          dwColor=RGB(255,255,255);
            }
//             if(pntReal.x==x && pntReal.y==y){
//                 dwColor=RGB(255,0,0);
//             }
            CPen pen(PS_SOLID,1,dwColor);
            a_pDC->SelectObject(pen);
            a_pDC->Rectangle(rText);
            rText.left+=2;
            ShowTextToDC(*a_pDC,s,rText,dwColor,m_iShowRGBRealValueFontSize,DT_LEFT,300,_T("Arial Black"));
        }        
    }
    brush.DeleteObject();
}

// 重算中心点位置 2016.4.22
// a_pntPoint:鼠标当前位置
// a_bZoomIn:是否放大，否则缩小
void CCustomViewEx::ReCalcCenterPoint(CPoint a_pntPoint,bool a_bZoomIn,double a_fOldScale)
{
    CPoint pntScrollPos=m_pntScroll;
    CPoint pntRelPoint=a_pntPoint-m_rRect.TopLeft();
    pntRelPoint.x   +=pntScrollPos.x;
    pntRelPoint.y   +=pntScrollPos.y;

    CPoint pntOldCenter;        // 重算中心点位置
    pntOldCenter.x=(m_rRect.Width()/2+pntScrollPos.x);
    pntOldCenter.y=(m_rRect.Height()/2+pntScrollPos.y);

    m_pntCenterPos.x=(int)(pntRelPoint.x*(m_fScale/a_fOldScale))+(pntOldCenter.x-pntRelPoint.x);
    m_pntCenterPos.y=(int)(pntRelPoint.y*(m_fScale/a_fOldScale))+(pntOldCenter.y-pntRelPoint.y);
        

    RegurlalyCenterPoint();

    m_pntOldCenterPos=m_pntCenterPos;

}

void CCustomViewEx::ReCalcRealRect()
{
    // 根据缩放比例计算需要显示的图像大小
    m_rRealRect.left=(int)(m_pntCenterPos.x/m_fScale-((double)m_rRect.Width()/2.0f/m_fScale));
    m_rRealRect.top =(int)(m_pntCenterPos.y/m_fScale-((double)m_rRect.Height()/2.0f/m_fScale));
    m_rRealRect.right=(int)(m_rRealRect.left+(double)m_rRect.Width()/m_fScale);
    m_rRealRect.bottom=(int)(m_rRealRect.top+(double)m_rRect.Height()/m_fScale);
    if(m_bCenteredShow){

    }
    else{
        if(m_rRealRect.left<0) {
            m_rRealRect.right-=m_rRealRect.left;
            m_rRealRect.left=0;
        }
        if(m_rRealRect.top<0){
            m_rRealRect.bottom-=m_rRealRect.top;
            m_rRealRect.top=0;
        }
        CBMP* pImage=GetScaleImage(1.0f);
        if(pImage){
            if(m_rRealRect.right>pImage->GetWidth()){
                m_rRealRect.right=pImage->GetWidth();
            }
            if(m_rRealRect.bottom>pImage->GetHeight()){
                m_rRealRect.bottom=pImage->GetHeight();
            }
        }

        CRect rImage(0,0,GetWidth(),GetHeight());
        m_rRealRect.IntersectRect(m_rRealRect,rImage);
    }
}

// 设置图像缩放比例
void CCustomViewEx::SetImageScale(double a_fScale,CPoint a_pntPoint)
{
    CBMP* pCurrImage=GetCurrImage();
    if(!pCurrImage) return ;

    double fOldScale=m_fScale;
    m_fScale=1.0f;

    ReCalcCenterPoint(a_pntPoint,true,fOldScale);
    ReCalcRealRect();

    ReSetScrollBar();
}

// 设置自适应
void CCustomViewEx::SetAutoFit()
{
    int iImageWidth=GetWidth();
    int iImageHeight=GetHeight();
    if(iImageWidth==0 || iImageHeight==0) return;
    double fOldScale=m_fScale;
    double fHScale=(double)m_rRect.Width()/(double)iImageWidth;
    double fVScale=(double)m_rRect.Height()/(double)iImageHeight;
    m_fScale=(fHScale>fVScale?fVScale:fHScale);

    if(m_bCenteredShow){
        m_pntScroll.x=-(int)(((double)m_rRect.Width()-m_fScale*iImageWidth)/2.0f);
        m_pntScroll.y=-(int)(((double)m_rRect.Height()-m_fScale*iImageHeight)/2.0f);
    }

    CPoint pntCenter;
    pntCenter.x=(long)(iImageWidth*m_fScale/2.0f+m_rRect.left);
    pntCenter.y=(long)(iImageHeight*m_fScale/2.0f+m_rRect.top);
    fOldScale=m_fScale;
    ReCalcCenterPoint(pntCenter,true,fOldScale);
    ReCalcRealRect();

    ReSetScrollBar();
}

// 保持位置
void CCustomViewEx::HoldLocale()
{
    int iImageWidth=GetWidth();
    int iImageHeight=GetHeight();
    if(iImageWidth==0 || iImageHeight==0) return;
    if(iImageWidth==m_szPriorImage.cx && iImageHeight==m_szPriorImage.cy) return;
    if(m_szPriorImage.cx<=0 || m_szPriorImage.cy<=0){
        m_szPriorImage=CSize(iImageWidth,iImageHeight);
        return;
    }
    double fOldScale=m_fScale;
    double fHScale=(double)m_rRect.Width()/(double)iImageWidth;
    double fVScale=(double)m_rRect.Height()/(double)iImageHeight;

    if(m_bCenteredShow){
        m_pntScroll.x=-(int)(((double)m_rRect.Width()-m_fScale*iImageWidth)/2.0f);
        m_pntScroll.y=-(int)(((double)m_rRect.Height()-m_fScale*iImageHeight)/2.0f);
    }

    CPoint pntCenter;
    pntCenter.x=(long)(iImageWidth*m_fScale/2.0f+m_rRect.left);
    pntCenter.y=(long)(iImageHeight*m_fScale/2.0f+m_rRect.top);
    fOldScale=m_fScale;

    //ReCalcCenterPoint(pntCenter,true,fOldScale);
    m_pntCenterPos.x=(int)((double)iImageWidth/(double)m_szPriorImage.cx*(double)m_pntCenterPos.x);
    m_pntCenterPos.y=(int)((double)iImageHeight/(double)m_szPriorImage.cy*(double)m_pntCenterPos.y);
//     m_pntScroll.x=(int)((double)iImageWidth/(double)m_szPriorImage.cx*(double)m_pntScroll.x);
//     m_pntScroll.y=(int)((double)iImageHeight/(double)m_szPriorImage.cy*(double)m_pntScroll.y);

    ReCalcRealRect();

    ReSetScrollBar();
    if(iImageWidth!=m_szPriorImage.cx && iImageHeight!=m_szPriorImage.cy){
        m_szPriorImage=CSize(iImageWidth,iImageHeight);
    }
}

bool CCustomViewEx::ZoomIn(CPoint a_pntPoint)
{
    if(m_fScale>60) return false;
    double fScale;

    double fOldScale=m_fScale;
    fScale=m_fScale*1.25;
    m_fScale=fScale;

    ReCalcCenterPoint(a_pntPoint,true,fOldScale);
    ReCalcRealRect();

    ReSetScrollBar();
    return true;
}

bool CCustomViewEx::ZoomOut(CPoint a_pntPoint)
{
    if(m_fScale<0.05) return false;
    double fScale;
    fScale=m_fScale*0.8;
    double fOldScale=m_fScale;
    m_fScale=fScale;
    ReCalcCenterPoint(a_pntPoint,false,fOldScale);
    ReCalcRealRect();

    ReSetScrollBar();

    return true;
}

void CCustomViewEx::ChangeSize(CRect a_rRect){
    CBMP* pCurrImage=GetCurrImage();
    if(!pCurrImage) return ;

    m_rRect=a_rRect;
    if(m_bUseBackground) UseBackground(true,m_dwBackColor1,m_dwBackColor2,m_iGridWidth,m_iGridHeight);

    ReCalcRealRect();
    ReSetScrollBar();
}

// 将中心点规则化 2016.4.20
void CCustomViewEx::RegurlalyCenterPoint(){
    if(m_bCenteredShow) return;
    double fWidth   =m_rRect.Width()/2;
    double fHeight  =m_rRect.Height()/2;
    if(m_pntCenterPos.x<fWidth) m_pntCenterPos.x=(int)fWidth;
    if(m_pntCenterPos.x>GetWidth()*m_fScale-fWidth) m_pntCenterPos.x=(int)(GetWidth()*m_fScale-fWidth);
    if(m_pntCenterPos.y<fHeight) m_pntCenterPos.y=(int)fHeight;
    if(m_pntCenterPos.y>GetHeight()*m_fScale-fHeight) m_pntCenterPos.y=(int)(GetHeight()*m_fScale-fHeight);
}

void CCustomViewEx::OnMouseMove(UINT nFlags, CPoint point) 
{
    m_pntCurrMouse=point;
    if(m_bMouseDown){
        int iOffsetX=m_pntMouseOld.x-point.x;
        int iOffsetY=m_pntMouseOld.y-point.y;

        m_pntCenterPos.x=(int)(m_pntOldCenterPos.x+(double)iOffsetX);
        m_pntCenterPos.y=(int)(m_pntOldCenterPos.y+(double)iOffsetY);

        RegurlalyCenterPoint();

        double fWidth=m_rRect.Width()/2;
        double fHeight=m_rRect.Height()/2;
        if(m_pHScrollBar) m_pHScrollBar->SetScrollPos(m_pntCenterPos.x-(int)fWidth);
        if(m_pVScrollBar) m_pVScrollBar->SetScrollPos(m_pntCenterPos.y-(int)fHeight);
        m_pntScroll=CPoint(m_pntCenterPos.x-(int)fWidth,m_pntCenterPos.y-(int)fHeight);

        ReCalcRealRect();
    }
}

void CCustomViewEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if(m_pOwner) m_pOwner->SetCapture();
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if(nFlags==MK_LBUTTON){
        if(m_rRect.PtInRect(point)){
            m_bMouseDown=true;
            m_pntMouseOld=point;
            m_pntOldCenterPos=m_pntCenterPos;
            SetCursor(LoadCursor(NULL,IDC_SIZEALL));
        }
    }       
}

void CCustomViewEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
    ReleaseCapture();
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CRect rImagePos(m_rRect);

    if(m_bMouseDown){
        m_pntOldCenterPos=m_pntCenterPos;
        m_bMouseDown=false;
    }
    SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void CCustomViewEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}

BOOL CCustomViewEx::PreTranslateMessage(MSG* pMsg) 
{

    return TRUE;
}

void CCustomViewEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    double fWidth=m_rRect.Width()/2;
    m_pntCenterPos.x=pScrollBar->GetScrollPos()+(int)fWidth;

    ReCalcRealRect();

    m_pntOldCenterPos=m_pntCenterPos;
}

void CCustomViewEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    double fHeight=m_rRect.Height()/2;
    m_pntCenterPos.y=pScrollBar->GetScrollPos()+(int)fHeight;
    
    ReCalcRealRect();

    m_pntOldCenterPos=m_pntCenterPos;
}

BOOL CCustomViewEx::QuickDrawDibDraw(HDC     hDC,LPRECT  lpDCRect,HDIB    hDIB,LPRECT  lpDIBRect,CPalette* pPal)
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

        DrawDibDraw(m_hdd,hDC,
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   (BITMAPINFOHEADER*)hDIB,        // LPBITMAPINFOHEADER 
							   (LPBITMAPINFO)lpDIBBits,         // lpBits
            rTemp.left,                     // SrcX
            rTemp.top,                      // SrcY
            rTemp.Width(),                  // wSrcWidth
            rTemp.Height(),                 // wSrcHeight
            DDF_BACKGROUNDPAL
        );
// 	  bSuccess = ::StretchDIBits(hDC,                          // hDC
// 							   lpDCRect->left,                 // DestX
// 							   lpDCRect->top,                  // DestY
// 							   RECTWIDTH(lpDCRect),            // nDestWidth
// 							   RECTHEIGHT(lpDCRect),           // nDestHeight
//             rTemp.left,                     // SrcX
//             rTemp.top,                      // SrcY
//             rTemp.Width(),                  // wSrcWidth
//             rTemp.Height(),                 // wSrcHeight
// 							   lpDIBBits,                      // lpBits
// 							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
// 							   DIB_RGB_COLORS,                 // wUsage
// 							   SRCCOPY);                       // dwROP
    }

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

// 加载图像的线程 2018.1.21
UINT CCustomViewEx::_LoadImageThread(){
    while(!m_bExit){
        WaitForSingleObject(m_evtLoadImage,INFINITE);
        m_evtLoadImage.ResetEvent();
        while(m_aImageFile.GetCount()>0){
            if(m_bExit) break;
            CString sImageFile;
            LockImageFileList();
            if(m_aImageFile.GetCount()>0){
                sImageFile=m_aImageFile.GetAt(0);
                m_aImageFile.RemoveAt(0);
            }
            UnlockImageFileList();
            m_imImageManage.AddImage(sImageFile);
        }
    }
    return 0;
}
UINT _stdcall CCustomViewEx::LoadImageThread(LPVOID AParam){
    CCustomViewEx* me=(CCustomViewEx*)AParam;
    if(me){
        me->_LoadImageThread();
    }
    return 0;
}

// 显示图像的线程 2018.1.21
UINT CCustomViewEx::_ShowImageThread(){
    while(!m_bExit){
        WaitForSingleObject(m_evtShowImage,INFINITE);
        if(m_bExit) break;
        m_evtShowImage.ResetEvent();
        //m_pDealImageInterface->OnDraw();
    }
    return 0;
}
UINT _stdcall CCustomViewEx::ShowImageThread(LPVOID AParam){
    CCustomViewEx* me=(CCustomViewEx*)AParam;
    if(me){
        me->_ShowImageThread();
    }
    return 0;
}

}