//////////////////////////////////////////////////////////////////////////
// ͼ����ʾ��
// ���ߣ�÷�ĺ�
// ���ڣ�2016.7
// Ҫ��
//   1.�����ǻ��� CWnd �����Ĵ���
//   2.�������������
// �÷���
//   1.�ڴ��ڸı���¼��е��� ChangeSize ����
//   2.���Ҫ�������Ĳ���һ��Ҫ�ڵ��������صĺ�����OnLButtonDown��OnLButtonUp �� OnMouseMove
//   3.���Ҫ�ù��������ã���ôһ��Ҫ�ڹ������¼��е��ã�OnHScroll �� OnVScroll
//   4.���Ҫ���������ֵķŴ���С���ܣ���ô���Ա�д���µĴ��룺
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
// ע��m_cvImageView ������Ϊ��CCustomViewEx m_cvImageView; 
// ���� ZoomIn �� ZoomOut �Ĵ������£�
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
//   5.����� OnPaint �е��� RealDraw ��ͼ����Ƴ���
//   6.��Ҫע�����ÿ�β�����Ҫˢ����ʾ�ĵط�����Ҫ���� InvalidateRect ���磺InvalidateRect(r,FALSE);
//
// ˵����
//   1.�����������ֵ���ڣ�ͼ��ߴ磨��ߣ�X �Ŵ��� - ��ǰ��ʾ���ڳߴ磨��ߣ�
//   2.SetAutoFit       : ��ͼ���Զ���Ӧ��ǰ����(��Ҫ�ȵ��� SetImage ����)
//   3.SetImageScale    : ����ͼ������ű�
//   4.UseBackground    : �Ƿ�ʹ�ñ������ñ���Ϊ����������
//   5.ShowRGBRealValue : ��ͼ�񱻷Ŵ�ʱ���Բ鿴ͼ��� RGB ��ֵ
//   6.���ͼ�����ı�һ��Ҫ���� SetImage ����
//   7.InitControl �ĵ�һ������ a_pOwner ��������Ϊ NULL ,�����Ͳ����Զ����� SetCapture �� ReleaseCapture ����
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


// ˢ�½���Ļص�����
typedef void (_stdcall* PCustomViewRefresh)(void*);

class SUN_DLL_DEFINE CCustomViewImage:public CIDItem{
public:
    double  m_fScale;           // ͼ�����
    CBMP    m_bmpImage;         // ͼ��

    CCustomViewImage(){
        m_bmpImage.ReSize(m_bmpImage.GetHDIBAdd(),10,10,24);
    }
    virtual ~CCustomViewImage(){};
    void SetImage(CBMP* a_pImage,double a_fZoom,double a_fScale);
    CBMP* GetImage(){return &m_bmpImage;}
};

// ͼ�� 2018.1.20
// �洢��ÿ�ֱ�����ͼ�񣬰��� 1:1 ��ͼ��
class SUN_DLL_DEFINE CImageBuffer:public CIDItem{
public:
    CImageBuffer(){};
    virtual ~CImageBuffer(){};

    virtual bool Assign(const CIDItemBase& AOther){
        CIDItem::Assign(AOther);
        return true;
    }
    virtual CIDItemBase* CreateInstance(){return new CImageBuffer;};            // ���������ʵ��
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CImageBuffer);};     // �������
    virtual LPCTSTR GetName(){return m_sName;};      // �������
    virtual void SetName(CString a_sName){m_sName=a_sName;};
    // ���ָ���Ŵ����µ�ͼ��ָ��ͷŴ���
    virtual CCustomViewImage* GetScaleImage(double a_fScale);
    virtual void AddImage(CBMP* a_pImage);
    virtual void AddImage(CString a_sImageFile);
    virtual void AddImageThumb(CString a_sImageFile);
    virtual void SetImage(CBMP* a_pImage);
    virtual void SetImage(CString a_sImageFile);
    virtual void SetImageThumb(CString a_sImageFile);
    // ��� 1:1 ������ԭʼͼ��
    virtual CCustomViewImage* GetImage(){if(m_aImageList.GetItemCount()>0) return (CCustomViewImage*)m_aImageList.GetValue(0);return NULL;};
    // �������ͼ��ߴ�
    virtual CSize GetImageSize(){return m_szImage;}
protected:
    CString     m_sName;        // �ļ����ƣ�������·����
    CItemList   m_aImageList;   // CCustomViewImage* �б�
    CSize       m_szImage;      // ͼ��������С
};

// ����ͼ��ص��ӿ� 2018.1.21
class SUN_DLL_DEFINE CDealImageInterface{
public:
    virtual void OnDraw()=NULL;
};

// ͼ����� 2018.1.20
class SUN_DLL_DEFINE CImageManage{
public:
    CImageManage(){m_bDebugMode=false;m_iMaxImageCount=1;m_pDealImageInterface=NULL;};
    virtual ~CImageManage(){};
    // ����ͼ����ص�
    virtual void SetImageDealInterface(CDealImageInterface* a_pInterface){m_pDealImageInterface=a_pInterface;};
    virtual void SetMaxImageCount(const int a_iCount){m_iMaxImageCount=a_iCount;};  // �������ͼ������
    virtual void AddImage(CImageBuffer* a_pImageBuffer);                            // ���ͼ��
    virtual void AddImage(CString a_sImageFile);
    virtual void Clear(){Lock();m_aImageList.Clear();Unlock();};                    // �������ͼ��
    // �������ƻ��ͼ��
    virtual CImageBuffer* GetImageOfName(CString a_sName){
        CImageBuffer* pBuffer=NULL;
        Lock();
        pBuffer=(CImageBuffer*)m_aImageList.GetValueOfName(a_sName);
        Unlock();
        return pBuffer;
    };
    // ���õ�ǰͼ��
    virtual void SetCurrImage(CString a_sImageName);
    // ��õ�ǰͼ��
    virtual CImageBuffer* GetCurrImageBuffer();                                     // ���ص�ǰͼ�񻺳�
    // �������ͼ��ߴ�
    virtual CSize GetCurrImageSize();
    virtual CCustomViewImage* GetCurrImage();                                       // ���ص�ǰͼ�񻺳��е�ԭʼͼ��
    virtual bool IsEmpty(){return (m_aImageList.GetItemCount()==0);};               // �����Ƿ�Ϊ��
    virtual void SetDebugMode(const bool a_bValue){m_bDebugMode=a_bValue;}          // ���õ���ģʽ������ģʽ�»������־�� DebugView
protected:
    virtual void Lock(){m_cs.Lock();}
    virtual void Unlock(){m_cs.Unlock();}
protected:
    bool                    m_bDebugMode;                   // ����ģʽ
    int                     m_iMaxImageCount;               // ��������ͼ������
    CString                 m_sPriorImageName;              // ǰһ����ǰͼ������
    CString                 m_sCurrImageName;               // ��ǰͼ�������    (���ҵ�ǰͼ�������ǰͼ�񲻴�������ǰһ��ͼ��)
    CItemList               m_aImageList;                   // CImageBuffer*
    CDealImageInterface*    m_pDealImageInterface;          // ����ͼ��Ļص��ӿ�
    CCriticalSection        m_cs;                           // ͬ������
};

class SUN_DLL_DEFINE CCustomViewEx:public CDealImageInterface
{
public:
    CCustomViewEx(void);
    ~CCustomViewEx(void);

    // ��ʼ���������
    virtual void InitControl(CWnd* a_pOwner,CRect a_rRect,CScrollBar* a_pHScrollBar,CScrollBar* a_pVScrollBar);
    virtual void AddImageFile(CString a_sImageFile);        // ���ͼ���ļ���֧�ֻ���
    virtual void SetImageBufferCount(const int a_iCount){m_imImageManage.SetMaxImageCount(a_iCount);};  // ����ͼ�񻺳�Ĵ�С
    virtual void SetCurrImage(CString a_sImageName);        // ���õ�ǰͼ��
    virtual void SetImage(CBMP* a_pViewImage);              // ������Ҫ��ʾ��ͼ�񣬴�ģʽ��֧�ֻ���
    virtual void ClearImage();                              // �������ͼ��
    virtual void OnDraw(){
        //ReCalcCenterPoint(m_pntCenterPos,true,GetScale());
        HoldLocale();
        if(m_pDealImageInterface) m_pDealImageInterface->OnDraw();
    };
    // ��õ�ǰ��ʾ��ͼ��
    virtual CBMP* GetCurrImage();
    // ��õ�ǰͼ��
    virtual CImageBuffer* GetCurrImageBuffer();                                     // ���ص�ǰͼ�񻺳�
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
    virtual void RefreshView(){};                           // ˢ����ʾ
    virtual double GetScale(){return m_fScale;}
    virtual void RegisterRefreshCallback(PCustomViewRefresh a_pCallback,void* a_pOwner){
        m_pCustomViewRefreshCallback=a_pCallback;m_pRefreshCallbackOwner=a_pOwner;}
    virtual CPoint TranScreenCoorToImage(const CPoint a_pntPoint);  // ����Ļ����ת��Ϊͼ������
    virtual COLORREF GetColor(const CPoint a_pntPoint);             // ���ָ��λ�õ���ɫ

    // ʵ�ʻ���
    virtual void RealDraw(CDC* a_pDC,CRect a_rOutRect=CRect(0,0,0,0));
    virtual void ChangeSize(CRect a_rRect);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    // �����Ƿ������ʾ(Ĭ�ϲ�������ʾ)
    virtual void SetCenteredShow(bool a_bCentered){m_bCenteredShow=a_bCentered;}
    // ��������Ӧ
    virtual void SetAutoFit();
    // ����λ��
    virtual void HoldLocale();
    // ����ͼ�����ű���
    virtual void SetImageScale(double a_fScale,CPoint a_pntPoint);
    // ��õ�ǰ������λ��(ע�⣺���ʹ�á�����ģʽ������ԭ�еĹ�����λ���Ǵ����)
    virtual CPoint GetScrollPoint(){return m_pntScroll;}
    // �Ƿ��Զ��屳����ע�⣬һ��Ҫ�ȵ��� InitControl 2016.7.23
    virtual void UseBackground(bool a_bUseBackground,COLORREF a_dwColor1=RGB(255,255,255),COLORREF a_dwColor2=RGB(206,206,206),int a_iGridWidth=30,int a_iGridHeight=30);
    // �Ƿ���ʾ RGB ���ݣ�Ҫ��������һ���ķŴ���
    virtual void ShowRGBRealValue(bool a_bShow,double a_fScale,int a_iFontSize){
        m_bShowRGBRealValue         =a_bShow;
        m_fShowRGBRealValueScale    =a_fScale;
        m_iShowRGBRealValueFontSize =a_iFontSize;
    }
    // ����ͼ����ص�
    virtual void SetImageDealInterface(CDealImageInterface* a_pInterface){m_pDealImageInterface=a_pInterface;};
    virtual void SetDebugMode(const bool a_bValue){m_bDebugMode=a_bValue;m_imImageManage.SetDebugMode(a_bValue);}          // ���õ���ģʽ������ģʽ�»������־�� DebugView
protected:
    // �����Ե���� 2016.4.20
    void RegurlalyCenterPoint();
    // �������ĵ�λ�� 2016.4.22
    void ReCalcCenterPoint(CPoint a_pntPoint,bool a_bZoomIn,double a_fScale);
    // ������ʵλ��
    void ReCalcRealRect();
    // ���ù�������Ϣ
    void SetScrollBarInfo(CScrollBar* a_pScrollBar,const int a_iMax,bool a_bHor);
    // ��õ�ǰ�Ŵ����µ�ͼ��ָ��ͷŴ���
    CBMP* GetCurrScaleImage(OUT double* a_fScale);
    // ���ָ���Ŵ��ʵ�ͼ��
    CBMP* GetScaleImage(const double a_fScale);
    // ������ֵ����Ļ 2016.7.28
    void DrawRealValue(CDC* a_pDC,CRect a_rShowRect,CRect a_rRealRect);
    // ���ٻ��ƴ���
    BOOL QuickDrawDibDraw(HDC     hDC,LPRECT  lpDCRect,HDIB    hDIB,LPRECT  lpDIBRect,CPalette* pPal);
    // ���ͼ����
    int GetWidth();
    // ���ͼ��߶�
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
    static UINT _stdcall LoadImageThread(LPVOID AParam);   // ��ʼ�߳�
    UINT _LoadImageThread();        // ��ʼ�߳�
    virtual void LockImageFileList(){m_evtLoadImageLock.Lock();}        // ����ͼ���ļ��б�
    virtual void UnlockImageFileList(){m_evtLoadImageLock.Unlock();}    // ����ͼ���ļ��б�
    static UINT _stdcall ShowImageThread(LPVOID AParam);   // ��ʼ�߳�
    UINT _ShowImageThread();        // ��ʼ�߳�
protected:
    bool            m_bDebugMode;           // �Ƿ����ģʽ
    double          m_fScale;               // ͼ�����ű�
    CWnd*           m_pOwner;               // ������
    CRect           m_rRect;                // ��ģ����Ҫ��ʾ��λ�ã������������
    CScrollBar*	    m_pHScrollBar;          // ���������
    CScrollBar*     m_pVScrollBar;          // ���������
    bool            m_bMouseDown;           // ��갴��״̬
    CPoint          m_pntMouseOld;          // ���ԭλ��
    CPoint          m_pntScrollPos;         // ������λ��
    CRect           m_rRealRect;            // ʵ����ʾͼ���λ��
    CPoint          m_pntCenterPos;         // ͼ�����ĵ�����(��ǰ�ֱ���)
    CPoint          m_pntOldCenterPos;      // ԭͼ�����ĵ�����
    CProTimer       m_ptCancelMulClickTime; // �����ι��ִ���
    PCustomViewRefresh  m_pCustomViewRefreshCallback;   // ˢ�»ص�
    void*           m_pRefreshCallbackOwner;// ˢ�»ص�����
    bool            m_bCenteredShow;        // �Ƿ������ʾ
    CPoint          m_pntScroll;            // �����滻��������λ��
    CSize           m_szPriorImage;         // ǰһ��ͼ��ĳߴ�

    CProTimer       m_ptSleep;              // ��ʱ��

    CItemList       m_aImageList;           // CCustomViewImage* 
    
    //CCanvas         m_cnvBack;              // ����
    CBMP            m_bmpBack;              // ����
    bool            m_bUseBackground;       // �Ƿ��Զ��屳��
    COLORREF        m_dwBackColor1;         // ������ɫ1
    COLORREF        m_dwBackColor2;         // ������ɫ2
    int             m_iGridWidth;           // �������ӵĿ��
    int             m_iGridHeight;          // �������ӵĸ߶�

    CPoint          m_pntCurrMouse;         // ����λ��

    // ��ʾ��ֵ 2016.7.28
    bool            m_bShowRGBRealValue;   // �Ƿ���ʾ RGB ��ֵ
    double          m_fShowRGBRealValueScale;   // ��ʾ��ֵ������(����Ҫ���ڸ÷Ŵ���)
    int             m_iShowRGBRealValueFontSize;// ��ʾ��ֵ�������С

    HDRAWDIB        m_hdd;                  // VFW �豸���
    CImageManage    m_imImageManage;        // ͼ�����
    bool                m_bExit;                        // �˳�����ֹͣ�����߳�
    HANDLE              m_hLoadImageThread;             // ����ͼ���߳�
    UINT                m_dwLoadImageThreadID;          // �̱߳��
    CEvent              m_evtLoadImage;                 // ����ͼ���¼�
    CCriticalSection    m_evtLoadImageLock;             // ͼ���ļ���������
    HANDLE              m_hShowImageThread;             // ��ʾͼ���߳�
    UINT                m_dwShowImageThreadID;          // �̱߳��
    CEvent              m_evtShowImage;                 // ��ʾͼ���¼�
    CStringArray        m_aImageFile;                   // ������Ҫ���ص�ͼ���ļ�
    CDealImageInterface*    m_pDealImageInterface;      // ͼ����ص��ӿ�

    CCriticalSection    m_cs;                           // ͬ������
};

}
