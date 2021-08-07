/*

���ܣ�ͼ����ʾģ��
���ߣ�÷�ĺ�
���ڣ�2019.10.24

�÷���
1.ʵ���� CCustomViewManage ����
2.�� CCustomViewManage ����� CreateCustomViewObject �������� CCustomViewInterface ����
3.�� CCustomViewInterface ����� 
    LoadImage ��������ͼ��
    Init ���ô��ھ���Լ����Ƶ�λ��
    Draw ����
    ZoomIn �� ZoomOut �����Ŵ���С
    SetImageCenter �����Զ���λ��ָ��λ��
    AddRectangle �������ƾ���
    ClearGraph ����������л��Ƶ�ͼ��

    �����Ҫ������϶�ͼ����Ҫ����Ӧ���¼��е��� OnLButtonDown��OnLButtonUp��OnMouseMove
*/

class CDrawCallbackInterface{
public:
    CDrawCallbackInterface(){};
    virtual ~CDrawCallbackInterface(){};
protected:
    virtual void OnDraw(HDC a_hDC)=NULL;        // ���ƻص�
};

class CCustomViewInterface{
protected:
    CCustomViewInterface(){};
    virtual ~CCustomViewInterface(){};

public:
    virtual bool LoadImage(LPCTSTR a_sImageFile,int a_iRotate)=NULL;    // ����ͼ���ļ�
    virtual void Init(HWND a_hWnd,RECT a_rRect)=NULL;                   // ��ʼ��
    virtual void Draw(HDC a_hDC)=NULL;                                  // ����
    virtual bool ZoomIn(POINT a_pntPoint)=NULL;                         // �Ŵ�
    virtual bool ZoomOut(POINT a_pntPoint)=NULL;                        // ��С
    virtual void OnLButtonDown(UINT nFlags,POINT point)=NULL;           // ����������¼�
    virtual void OnLButtonUp(UINT nFlags,POINT point)=NULL;             // �����������¼�
    virtual void OnMouseMove(UINT nFlags,POINT point)=NULL;             // ����ƶ��¼�
    virtual POINT GetScrollPoint()=NULL;                                // ��õ�ǰ������λ��
    virtual void SetImageCenter(POINT a_pntPoint)=NULL;                 // ����ͼ�������λ��
    virtual double GetScale()=NULL;                                     // ������ű�
    virtual bool SetScale(POINT a_pntPoint,double a_fScale)=NULL;       // ���õ�ǰ���ű���
    virtual SIZE GetImageSize()=NULL;                                   // ���ͼ��Ĵ�С
    virtual int GetImageBit()=NULL;                                     // ���ͼ���λ��
    virtual void AddRectangle(RECT a_rRect,DWORD a_dwColor,int a_iPenWidth)=NULL;   // ��Ӿ��ο�
    virtual void ClearGraph()=NULL;                                                 // ���ͼ��
};

// ����ͨ�����ݽṹ����
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
