
// CustomViewTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "CustomViewInterface.h"
#include "SunMyBmp.h"


// CCustomViewTestDlg �Ի���
class CCustomViewTestDlg : public CDialogEx
{
    void RefreshImage();
// ����
public:
	CCustomViewTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
    virtual ~CCustomViewTestDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMVIEWTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
    CGDIPlusInit            m_gdiplus;
    CCustomViewManage       m_cvmManage;
    CCustomViewInterface*   m_pCustomView;

	// ���ɵ���Ϣӳ�亯��
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
