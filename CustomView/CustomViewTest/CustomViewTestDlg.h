
// CustomViewTestDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CustomViewInterface.h"
#include "SunMyBmp.h"


// CCustomViewTestDlg 对话框
class CCustomViewTestDlg : public CDialogEx
{
    void RefreshImage();
// 构造
public:
	CCustomViewTestDlg(CWnd* pParent = NULL);	// 标准构造函数
    virtual ~CCustomViewTestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMVIEWTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
    CGDIPlusInit            m_gdiplus;
    CCustomViewManage       m_cvmManage;
    CCustomViewInterface*   m_pCustomView;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CStatic m_imgMain;
    afx_msg void OnBnClickedBtnLoadImage();
    afx_msg void OnSize(UINT nType,int cx,int cy);
    afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
    afx_msg void OnMouseMove(UINT nFlags,CPoint point);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
};
