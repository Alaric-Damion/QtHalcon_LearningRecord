
// CustomViewTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CustomViewTest.h"
#include "CustomViewTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCustomViewTestDlg 对话框



CCustomViewTestDlg::CCustomViewTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUSTOMVIEWTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pCustomView=NULL;
}

CCustomViewTestDlg::~CCustomViewTestDlg(){
    m_cvmManage.DestroyCustomViewObject(m_pCustomView);
}

void CCustomViewTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX,IDC_IMG_MAIN,m_imgMain);
}

BEGIN_MESSAGE_MAP(CCustomViewTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE,&CCustomViewTestDlg::OnBnClickedBtnLoadImage)
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BUTTON1,&CCustomViewTestDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2,&CCustomViewTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCustomViewTestDlg 消息处理程序

BOOL CCustomViewTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    m_pCustomView   =m_cvmManage.CreateCustomViewObject();
    CRect r;
    //GetClientRect(r);
    m_imgMain.GetWindowRect(r);
    ScreenToClient(r);
    m_pCustomView->Init(GetSafeHwnd(),r);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCustomViewTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCustomViewTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
        m_pCustomView->Draw(dc.m_hDC);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCustomViewTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCustomViewTestDlg::OnBnClickedBtnLoadImage(){
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,_T("图像文件(*.jpg;*.bmp;*.png;*.tif;*.gif)|*.jpg;*.bmp;*.png;*.tif;*.gif|位图文件 (*.bmp)|*.bmp|JPEG图像文件(*.jpg)|*.jpg|PNG图像文件(*.png)|*.png|GIF图像文件(*.gif)|*.gif|所有文件|*.*||"),this);
    if(dlg.DoModal()==IDOK){
        m_pCustomView->LoadImage(dlg.GetPathName(),0);
        RefreshImage();
    }
}


void CCustomViewTestDlg::OnSize(UINT nType,int cx,int cy){
    CDialogEx::OnSize(nType,cx,cy);
    if(!m_pCustomView) return;
    CRect r;
    m_imgMain.GetWindowRect(r);
    ScreenToClient(r);
    m_pCustomView->Init(GetSafeHwnd(),r);
    RefreshImage();
}


void CCustomViewTestDlg::OnLButtonDown(UINT nFlags,CPoint point){
    SetCapture();
    CRect rMain;
    m_imgMain.GetWindowRect(rMain);
    ScreenToClient(rMain);
    if(rMain.PtInRect(point)){
        m_pCustomView->OnLButtonDown(nFlags,point);
        RefreshImage();
    }
    CDialogEx::OnLButtonDown(nFlags,point);
}


void CCustomViewTestDlg::OnLButtonUp(UINT nFlags,CPoint point){
    ReleaseCapture();
    CRect rMain;
    m_imgMain.GetWindowRect(rMain);
    ScreenToClient(rMain);
    if(rMain.PtInRect(point)){
        m_pCustomView->OnLButtonUp(nFlags,point);
        RefreshImage();
    }

    CDialogEx::OnLButtonUp(nFlags,point);
}


void CCustomViewTestDlg::OnMouseMove(UINT nFlags,CPoint point){
    CRect rMain;
    m_imgMain.GetWindowRect(rMain);
    ScreenToClient(rMain);
    if(rMain.PtInRect(point)){
        m_pCustomView->OnMouseMove(nFlags,point);
        RefreshImage();
    }

    CDialogEx::OnMouseMove(nFlags,point);
}


BOOL CCustomViewTestDlg::PreTranslateMessage(MSG* pMsg){
    if(pMsg->message==WM_MOUSEWHEEL){
        CPoint pntPoint=pMsg->pt;
        int iParam=pMsg->wParam&0xffffffff;
        //SetWindowText(Format("message=%d wParam=%d %d,%d",pMsg->message,iParam,pntPoint.x,pntPoint.y));
        POINT pntNew;
        pntNew.x=pntPoint.x;
        pntNew.y=pntPoint.y;
        ScreenToClient(&pntNew);
        pntPoint=pntNew;
        if(iParam>0) m_pCustomView->ZoomIn(pntNew);
        else         m_pCustomView->ZoomOut(pntNew);
        RefreshImage();
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CCustomViewTestDlg::RefreshImage(){
    CRect rMain;
    m_imgMain.GetWindowRect(rMain);
    ScreenToClient(rMain);
    InvalidateRect(rMain,FALSE);
}

void CCustomViewTestDlg::OnBnClickedButton1(){
    CSize szImage=m_pCustomView->GetImageSize();
    CPoint pntPoint;
    pntPoint.x=szImage.cx/4*3;
    pntPoint.y=szImage.cy/4*3;
    m_pCustomView->SetImageCenter(pntPoint);
    RefreshImage();
}


void CCustomViewTestDlg::OnBnClickedButton2(){
    CRect r(100,100,150,150);
    m_pCustomView->AddRectangle(r,RGB(255,255,0),2);
    RefreshImage();
}
