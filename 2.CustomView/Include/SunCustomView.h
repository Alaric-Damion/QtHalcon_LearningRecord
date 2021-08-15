//////////////////////////////////////////////////////////////////////////
// 图像显示类
// 作者：梅文海
// 日期：2016.7
// 要求：
//   1.必须是基于 CWnd 开发的窗口
//   2.必须包含三化库
// 用法：
//   1.在窗口改变的事件中调用 ChangeSize 方法
//   2.如果要保持鼠标的操作一定要在调用鼠标相关的函数：OnLButtonDown、OnLButtonUp 和 OnMouseMove
//   3.如果要让滚动条启用，那么一定要在滚动条事件中调用：OnHScroll 和 OnVScroll
//   4.如果要激活鼠标滚轮的放大缩小功能，那么可以编写如下的代码：
// BOOL XXXX::PreTranslateMessage(MSG* pMsg)
// {
//     m_cvImageView.PreTranslateMessage(pMsg);   
//     if(pMsg->message==WM_MOUSEWHEEL){
//         if(m_ptCancelMulClickTime.GetTime(false)>0.01){
//             if(GetDlgItem(IDC_LVW_KEY)==GetFocus()){
//                 CPoint pntPoint=pMsg->pt;
//                 int iParam=pMsg->wParam & 0xffffffff;
//                 POINT pntNew;
//                 pntNew.x=pntPoint.x;
//                 pntNew.y=pntPoint.y;
//                 ScreenToClient(&pntNew);
//                 pntPoint=pntNew;
//                 if(iParam>0) ZoomIn(pntNew);
//                 else         ZoomOut(pntNew);
//                 RefreshImage();        
//             }
//             m_ptCancelMulClickTime.Reset();
//         }
//     }
//     return CFormView::PreTranslateMessage(pMsg);
// }
// 注：m_cvImageView 的声明为：CCustomViewEx m_cvImageView; 
// 其中 ZoomIn 和 ZoomOut 的代码如下：
// bool XXXX::ZoomIn(CPoint a_pntPoint)
// {
//     double fOldScale=m_cvImageView.GetScale();
// 
//     bool bResult=m_cvImageView.ZoomIn(a_pntPoint);
// 
//     return bResult;
// }
// 
// bool XXXX::ZoomOut(CPoint a_pntPoint)
// {
//     double fOldScale=m_cvImageView.GetScale();
//     bool bResult=m_cvImageView.ZoomOut(a_pntPoint);
// 
//     return bResult;
// }
//   5.最后在 OnPaint 中调用 RealDraw 把图像绘制出来
//   6.需要注意的是每次操作需要刷新显示的地方都需要调用 InvalidateRect ，如：InvalidateRect(r,FALSE);
//
// 说明：
//   1.滚动条的最大值等于：图像尺寸（宽高）X 放大率 - 当前显示窗口尺寸（宽高）
//   2.SetAutoFit       : 让图像自动适应当前界面(需要先调用 SetImage 函数)
//   3.SetImageScale    : 设置图像的缩放比
//   4.UseBackground    : 是否使用背景，该背景为国际象棋盘
//   5.ShowRGBRealValue : 当图像被放大时可以查看图像的 RGB 数值
//   6.如果图像发生改变一定要调用 SetImage 函数
//   7.InitControl 的第一个参数 a_pOwner 可以设置为 NULL ,这样就不会自动调用 SetCapture 和 ReleaseCapture 函数
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "SunCanvas.h"
#include "vfw.h"
#pragma comment(lib,"vfw32.lib")

#pragma comment( lib, "gdiplus.lib" )
#include "SunItemList.h"
#include "gdiplus.h"
using namespace Gdiplus;


namespace sunLib{


// 刷新界面的回调函数
typedef void (_stdcall* PCustomViewRefresh)(void*);

class SUN_DLL_DEFINE CCustomViewImage:public CIDItem{
public:
    double  m_fScale;           // 图像比例
    CBMP    m_bmpImage;         // 图像

    CCustomViewImage(){
        m_bmpImage.ReSize(m_bmpImage.GetHDIBAdd(),10,10,24);
    }
    virtual ~CCustomViewImage(){};
    void SetImage(CBMP* a_pImage,double a_fZoom,double a_fScale);
    CBMP* GetImage(){return &m_bmpImage;}
};

// 图像 2018.1.20
// 存储了每种比例的图像，包括 1:1 的图像
class SUN_DLL_DEFINE CImageBuffer:public CIDItem{
public:
    CImageBuffer(){};
    virtual ~CImageBuffer(){};

    virtual bool Assign(const CIDItemBase& AOther){
        CIDItem::Assign(AOther);
        return true;
    }
    virtual CIDItemBase* CreateInstance(){return new CImageBuffer;};            // 创建该类的实例
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CImageBuffer);};     // 获得类名
    virtual LPCTSTR GetName(){return m_sName;};      // 获得名称
    virtual void SetName(CString a_sName){m_sName=a_sName;};
    // 获得指定放大率下的图像指针和放大率
    virtual CCustomViewImage* GetScaleImage(double a_fScale);
    virtual void AddImage(CBMP* a_pImage);
    virtual void AddImage(CString a_sImageFile);
    virtual void AddImageThumb(CString a_sImageFile);
    virtual void SetImage(CBMP* a_pImage);
    virtual void SetImage(CString a_sImageFile);
    virtual void SetImageThumb(CString a_sImageFile);
    // 获得 1:1 比例的原始图像
    virtual CCustomViewImage* GetImage(){if(m_aImageList.GetItemCount()>0) return (CCustomViewImage*)m_aImageList.GetValue(0);return NULL;};
    // 获得正常图像尺寸
    virtual CSize GetImageSize(){return m_szImage;}
protected:
    CString     m_sName;        // 文件名称（带绝对路径）
    CItemList   m_aImageList;   // CCustomViewImage* 列表
    CSize       m_szImage;      // 图像正常大小
};

// 处理图像回调接口 2018.1.21
class SUN_DLL_DEFINE CDealImageInterface{
public:
    virtual void OnDraw()=NULL;
};

// 图像管理 2018.1.20
class SUN_DLL_DEFINE CImageManage{
public:
    CImageManage(){m_bDebugMode=false;m_iMaxImageCount=1;m_pDealImageInterface=NULL;};
    virtual ~CImageManage(){};
    // 设置图像处理回调
    virtual void SetImageDealInterface(CDealImageInterface* a_pInterface){m_pDealImageInterface=a_pInterface;};
    virtual void SetMaxImageCount(const int a_iCount){m_iMaxImageCount=a_iCount;};  // 设置最大图像数量
    virtual void AddImage(CImageBuffer* a_pImageBuffer);                            // 添加图像
    virtual void AddImage(CString a_sImageFile);
    virtual void Clear(){Lock();m_aImageList.Clear();Unlock();};                    // 清除所有图像
    // 根据名称获得图像
    virtual CImageBuffer* GetImageOfName(CString a_sName){
        CImageBuffer* pBuffer=NULL;
        Lock();
        pBuffer=(CImageBuffer*)m_aImageList.GetValueOfName(a_sName);
        Unlock();
        return pBuffer;
    };
    // 设置当前图像
    virtual void SetCurrImage(CString a_sImageName);
    // 获得当前图像
    virtual CImageBuffer* GetCurrImageBuffer();                                     // 返回当前图像缓冲
    // 获得正常图像尺寸
    virtual CSize GetCurrImageSize();
    virtual CCustomViewImage* GetCurrImage();                                       // 返回当前图像缓冲中的原始图像
    virtual bool IsEmpty(){return (m_aImageList.GetItemCount()==0);};               // 返回是否为空
    virtual void SetDebugMode(const bool a_bValue){m_bDebugMode=a_bValue;}          // 设置调试模式，调试模式下会输出日志到 DebugView
protected:
    virtual void Lock(){m_cs.Lock();}
    virtual void Unlock(){m_cs.Unlock();}
protected:
    bool                    m_bDebugMode;                   // 调试模式
    int                     m_iMaxImageCount;               // 允许的最大图像数量
    CString                 m_sPriorImageName;              // 前一个当前图像名称
    CString                 m_sCurrImageName;               // 当前图像的名称    (先找当前图像，如果当前图像不存在则找前一个图像)
    CItemList               m_aImageList;                   // CImageBuffer*
    CDealImageInterface*    m_pDealImageInterface;          // 处理图像的回调接口
    CCriticalSection        m_cs;                           // 同步对象
};

class SUN_DLL_DEFINE CCustomViewEx:public CDealImageInterface
{
public:
    CCustomViewEx(void);
    ~CCustomViewEx(void);

    // 初始化相关数据
    virtual void InitControl(CWnd* a_pOwner,CRect a_rRect,CScrollBar* a_pHScrollBar,CScrollBar* a_pVScrollBar);
    virtual void AddImageFile(CString a_sImageFile);        // 添加图像文件，支持缓冲
    virtual void SetImageBufferCount(const int a_iCount){m_imImageManage.SetMaxImageCount(a_iCount);};  // 设置图像缓冲的大小
    virtual void SetCurrImage(CString a_sImageName);        // 设置当前图像
    virtual void SetImage(CBMP* a_pViewImage);              // 设置需要显示的图像，此模式不支持缓冲
    virtual void ClearImage();                              // 清除所有图像
    virtual void OnDraw(){
        //ReCalcCenterPoint(m_pntCenterPos,true,GetScale());
        HoldLocale();
        if(m_pDealImageInterface) m_pDealImageInterface->OnDraw();
    };
    // 获得当前显示的图像
    virtual CBMP* GetCurrImage();
    // 获得当前图像
    virtual CImageBuffer* GetCurrImageBuffer();                                     // 返回当前图像缓冲
    void ReSetScrollBar();

    virtual bool ZoomIn(CPoint a_pntPoint);
    virtual bool ZoomOut(CPoint a_pntPoint);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void OnMouseMove(UINT nFlags, CPoint point);
    virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual void OnRButtonDown(UINT nFlags, CPoint point){};
    virtual void OnRButtonUp(UINT nFlags, CPoint point){};
    virtual void OnRButtonDblClk(UINT nFlags, CPoint point){};
    virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
    virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
    virtual void RefreshView(){};                           // 刷新显示
    virtual double GetScale(){return m_fScale;}
    virtual void RegisterRefreshCallback(PCustomViewRefresh a_pCallback,void* a_pOwner){
        m_pCustomViewRefreshCallback=a_pCallback;m_pRefreshCallbackOwner=a_pOwner;}
    virtual CPoint TranScreenCoorToImage(const CPoint a_pntPoint);  // 将屏幕坐标转换为图像坐标
    virtual COLORREF GetColor(const CPoint a_pntPoint);             // 获得指定位置的颜色

    // 实际绘制
    virtual void RealDraw(CDC* a_pDC,CRect a_rOutRect=CRect(0,0,0,0));
    virtual void ChangeSize(CRect a_rRect);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    // 设置是否居中显示(默认不居中显示)
    virtual void SetCenteredShow(bool a_bCentered){m_bCenteredShow=a_bCentered;}
    // 设置自适应
    virtual void SetAutoFit();
    // 保持位置
    virtual void HoldLocale();
    // 设置图像缩放比例
    virtual void SetImageScale(double a_fScale,CPoint a_pntPoint);
    // 获得当前滚动条位置(注意：如果使用“居中模式”，则原有的滚动条位置是错误的)
    virtual CPoint GetScrollPoint(){return m_pntScroll;}
    // 是否自定义背景，注意，一定要先调用 InitControl 2016.7.23
    virtual void UseBackground(bool a_bUseBackground,COLORREF a_dwColor1=RGB(255,255,255),COLORREF a_dwColor2=RGB(206,206,206),int a_iGridWidth=30,int a_iGridHeight=30);
    // 是否显示 RGB 数据，要求必须大于一定的放大率
    virtual void ShowRGBRealValue(bool a_bShow,double a_fScale,int a_iFontSize){
        m_bShowRGBRealValue         =a_bShow;
        m_fShowRGBRealValueScale    =a_fScale;
        m_iShowRGBRealValueFontSize =a_iFontSize;
    }
    // 设置图像处理回调
    virtual void SetImageDealInterface(CDealImageInterface* a_pInterface){m_pDealImageInterface=a_pInterface;};
    virtual void SetDebugMode(const bool a_bValue){m_bDebugMode=a_bValue;m_imImageManage.SetDebugMode(a_bValue);}          // 设置调试模式，调试模式下会输出日志到 DebugView
protected:
    // 将中性点规则化 2016.4.20
    void RegurlalyCenterPoint();
    // 重算中心点位置 2016.4.22
    void ReCalcCenterPoint(CPoint a_pntPoint,bool a_bZoomIn,double a_fScale);
    // 重算真实位置
    void ReCalcRealRect();
    // 设置滚动条信息
    void SetScrollBarInfo(CScrollBar* a_pScrollBar,const int a_iMax,bool a_bHor);
    // 获得当前放大率下的图像指针和放大率
    CBMP* GetCurrScaleImage(OUT double* a_fScale);
    // 获得指定放大率的图像
    CBMP* GetScaleImage(const double a_fScale);
    // 绘制真值到屏幕 2016.7.28
    void DrawRealValue(CDC* a_pDC,CRect a_rShowRect,CRect a_rRealRect);
    // 高速绘制代码
    BOOL QuickDrawDibDraw(HDC     hDC,LPRECT  lpDCRect,HDIB    hDIB,LPRECT  lpDIBRect,CPalette* pPal);
    // 获得图像宽度
    int GetWidth();
    // 获得图像高度
    int GetHeight();
    virtual void BeginLoadImageThread(){
        m_bExit=false;
        m_hLoadImageThread=(HANDLE)_beginthreadex(NULL,0,LoadImageThread,this,CREATE_SUSPENDED,&m_dwLoadImageThreadID);
        ResumeThread(m_hLoadImageThread);
    };
    virtual void BeginShowImageThread(){
        m_bExit=false;
        m_hShowImageThread=(HANDLE)_beginthreadex(NULL,0,ShowImageThread,this,CREATE_SUSPENDED,&m_dwShowImageThreadID);
        ResumeThread(m_hShowImageThread);
    };
    static UINT _stdcall LoadImageThread(LPVOID AParam);   // 开始线程
    UINT _LoadImageThread();        // 开始线程
    virtual void LockImageFileList(){m_evtLoadImageLock.Lock();}        // 锁定图像文件列表
    virtual void UnlockImageFileList(){m_evtLoadImageLock.Unlock();}    // 解锁图像文件列表
    static UINT _stdcall ShowImageThread(LPVOID AParam);   // 开始线程
    UINT _ShowImageThread();        // 开始线程
protected:
    bool            m_bDebugMode;           // 是否调试模式
    double          m_fScale;               // 图像缩放比
    CWnd*           m_pOwner;               // 主窗口
    CRect           m_rRect;                // 本模块需要显示的位置，相对于主窗口
    CScrollBar*	    m_pHScrollBar;          // 横向滚动条
    CScrollBar*     m_pVScrollBar;          // 纵向滚动条
    bool            m_bMouseDown;           // 鼠标按下状态
    CPoint          m_pntMouseOld;          // 鼠标原位置
    CPoint          m_pntScrollPos;         // 滚动条位置
    CRect           m_rRealRect;            // 实际显示图像的位置
    CPoint          m_pntCenterPos;         // 图像中心点坐标(当前分辨率)
    CPoint          m_pntOldCenterPos;      // 原图像中心点坐标
    CProTimer       m_ptCancelMulClickTime; // 解决多次滚轮触发
    PCustomViewRefresh  m_pCustomViewRefreshCallback;   // 刷新回调
    void*           m_pRefreshCallbackOwner;// 刷新回调参数
    bool            m_bCenteredShow;        // 是否居中显示
    CPoint          m_pntScroll;            // 用来替换滚动条的位置
    CSize           m_szPriorImage;         // 前一张图像的尺寸

    CProTimer       m_ptSleep;              // 延时用

    CItemList       m_aImageList;           // CCustomViewImage* 
    
    //CCanvas         m_cnvBack;              // 背景
    CBMP            m_bmpBack;              // 背景
    bool            m_bUseBackground;       // 是否自定义背景
    COLORREF        m_dwBackColor1;         // 背景颜色1
    COLORREF        m_dwBackColor2;         // 背景颜色2
    int             m_iGridWidth;           // 背景格子的宽度
    int             m_iGridHeight;          // 背景格子的高度

    CPoint          m_pntCurrMouse;         // 鼠标的位置

    // 显示真值 2016.7.28
    bool            m_bShowRGBRealValue;   // 是否显示 RGB 真值
    double          m_fShowRGBRealValueScale;   // 显示真值的条件(必须要大于该放大率)
    int             m_iShowRGBRealValueFontSize;// 显示真值的字体大小

    HDRAWDIB        m_hdd;                  // VFW 设备句柄
    CImageManage    m_imImageManage;        // 图像管理
    bool                m_bExit;                        // 退出程序，停止所有线程
    HANDLE              m_hLoadImageThread;             // 加载图像线程
    UINT                m_dwLoadImageThreadID;          // 线程编号
    CEvent              m_evtLoadImage;                 // 加载图像事件
    CCriticalSection    m_evtLoadImageLock;             // 图像文件名缓存锁
    HANDLE              m_hShowImageThread;             // 显示图像线程
    UINT                m_dwShowImageThreadID;          // 线程编号
    CEvent              m_evtShowImage;                 // 显示图像事件
    CStringArray        m_aImageFile;                   // 所有需要加载的图像文件
    CDealImageInterface*    m_pDealImageInterface;      // 图像处理回调接口

    CCriticalSection    m_cs;                           // 同步对象
};

}
