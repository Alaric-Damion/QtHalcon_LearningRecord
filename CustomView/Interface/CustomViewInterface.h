/*

功能：图像显示模块
作者：梅文海
日期：2019.10.24

用法：
1.实例化 CCustomViewManage 对象
2.用 CCustomViewManage 对象的 CreateCustomViewObject 方法构造 CCustomViewInterface 对象
3.用 CCustomViewInterface 对象的 
    LoadImage 方法加载图像
    Init 设置窗口句柄以及绘制的位置
    Draw 绘制
    ZoomIn 和 ZoomOut 用来放大缩小
    SetImageCenter 用来自动定位到指定位置
    AddRectangle 用来绘制矩形
    ClearGraph 用来清除所有绘制的图形

    如果需要用鼠标拖动图像，需要在相应的事件中调用 OnLButtonDown、OnLButtonUp、OnMouseMove
*/

class CDrawCallbackInterface{
public:
    CDrawCallbackInterface(){};
    virtual ~CDrawCallbackInterface(){};
protected:
    virtual void OnDraw(HDC a_hDC)=NULL;        // 绘制回调
};

class CCustomViewInterface{
protected:
    CCustomViewInterface(){};
    virtual ~CCustomViewInterface(){};

public:
    virtual bool LoadImage(LPCTSTR a_sImageFile,int a_iRotate)=NULL;    // 加载图像文件
    virtual void Init(HWND a_hWnd,RECT a_rRect)=NULL;                   // 初始化
    virtual void Draw(HDC a_hDC)=NULL;                                  // 绘制
    virtual bool ZoomIn(POINT a_pntPoint)=NULL;                         // 放大
    virtual bool ZoomOut(POINT a_pntPoint)=NULL;                        // 缩小
    virtual void OnLButtonDown(UINT nFlags,POINT point)=NULL;           // 鼠标左键点击事件
    virtual void OnLButtonUp(UINT nFlags,POINT point)=NULL;             // 鼠标左键弹起事件
    virtual void OnMouseMove(UINT nFlags,POINT point)=NULL;             // 鼠标移动事件
    virtual POINT GetScrollPoint()=NULL;                                // 获得当前滚动条位置
    virtual void SetImageCenter(POINT a_pntPoint)=NULL;                 // 设置图像的中心位置
    virtual double GetScale()=NULL;                                     // 获得缩放比
    virtual bool SetScale(POINT a_pntPoint,double a_fScale)=NULL;       // 设置当前缩放比例
    virtual SIZE GetImageSize()=NULL;                                   // 获得图像的大小
    virtual int GetImageBit()=NULL;                                     // 获得图像的位数
    virtual void AddRectangle(RECT a_rRect,DWORD a_dwColor,int a_iPenWidth)=NULL;   // 添加矩形框
    virtual void ClearGraph()=NULL;                                                 // 清除图形
};

// 创建通用数据结构对象
// 2011.1.12
class CCustomViewManage{
public:
    typedef CCustomViewInterface* (__stdcall* PCreateCustomViewObject)(void);
    typedef void(_stdcall* PDestroyCustomViewObject)(HANDLE a_hHnd);
    CCustomViewManage(){
        m_pCreateCustomView=NULL;
        m_pDestroyCustomView=NULL;
        m_bLoad=false;
        m_hDLL=GetModuleHandle(_T("CustomViewDLL.dll"));
        if(!m_hDLL){
            CString sDLLFile=GetAppExePath()+_T("CustomViewDLL.dll");
            m_hDLL=LoadLibrary(sDLLFile);
            if(!m_hDLL){
                return;
            }
            m_bLoad=true;
        }
        m_pCreateCustomView=(PCreateCustomViewObject)GetProcAddress(m_hDLL,"ExpCreateCustomView");
        m_pDestroyCustomView=(PDestroyCustomViewObject)GetProcAddress(m_hDLL,"ExpDestroyCustomView");
        ASSERT(m_pCreateCustomView && m_pDestroyCustomView);
    };
    virtual ~CCustomViewManage(){
        Destroy();
    };
    CCustomViewInterface* CreateCustomViewObject(){
        if(m_pCreateCustomView){
            return m_pCreateCustomView();
        }
        return NULL;
    }
    BOOL DestroyCustomViewObject(CCustomViewInterface* a_pPack){
        if(m_pDestroyCustomView && a_pPack){
            m_pDestroyCustomView((HANDLE)a_pPack);
            return TRUE;
        }
        return FALSE;
    }
    void Destroy(){
        if(m_hDLL && m_bLoad){
            FreeLibrary(m_hDLL);
            m_hDLL=NULL;
            m_bLoad=false;
        }
    }
protected:
    HMODULE m_hDLL;
    bool    m_bLoad;
    PCreateCustomViewObject   m_pCreateCustomView;
    PDestroyCustomViewObject  m_pDestroyCustomView;
};




#ifndef _custom_view_interface_h
#define _custom_view_interface_h








#endif // _custom_view_interface_h
