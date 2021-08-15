//////////////////////////////////////////////////////////////////////
// 
// ������
// ���ܣ�����������ڵ��豸�������ͼ��Ļ���
// ��������ʱ��������Ļ��˸
// ʹ��˵����
// ��ʹ�� Create ������������Ķ��󣬲��Ҵ��뻭���Ĵ�С
// ����������ֱ�ӽ����ݻ��Ƶ������ϣ�����ͼ��������豸�ϣ����£�
// CCanvas canvas;
// canvas.Create(&dc,332,153);
// canvas.GetDC()->Rectange(1,1,100,50);
// OutToDC(&dc,CRect(100,100,100+332,100+153));
// 
// ���ߣ�÷�ĺ�
// ���ڣ�2011.3.31
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANVAS_H__9066CECA_82F0_4A75_950D_26D34D23F5E0__INCLUDED_)
#define AFX_CANVAS_H__9066CECA_82F0_4A75_950D_26D34D23F5E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace sunLib{

class SUN_DLL_DEFINE CCanvas  
{
public:
    CCanvas();
    virtual ~CCanvas();

    void Create(CDC* a_pDC,CSize a_szSize){
        Create(a_pDC,a_szSize.cx,a_szSize.cy);
    }
    // ��������
    void Create(CDC* a_pDC,const int a_iWidth,const int a_iHeight){
        m_dcMem.CreateCompatibleDC(a_pDC);
        m_bit.CreateCompatibleBitmap(a_pDC,a_iWidth,a_iHeight);
        m_dcMem.SelectObject(&m_bit);
    }
    void Destroy(){
        m_dcMem.DeleteDC();
        m_bit.DeleteObject();
    }
    // ����ͼ�񵽻���
    void Draw(CBMP* a_pBMP,LPRECT a_rRect){
        PaintDIB(m_dcMem,a_rRect,a_pBMP->GetHDIB(),a_pBMP->GetRect(),a_pBMP->GetPalette());
    }
    // ������豸
    void OutToDC(CDC* a_pDC,LPRECT a_rRect,DWORD dwRop=SRCCOPY){
        a_pDC->BitBlt(a_rRect->left,a_rRect->top,a_rRect->right-a_rRect->left,a_rRect->bottom-a_rRect->top,&m_dcMem,0,0,SRCCOPY);

    }
    CDC*    GetDC(){return &m_dcMem;}
    CBitmap* GetBitmap(){return &m_bit;}
protected:
    CDC         m_dcMem;
    CBitmap     m_bit;

};

}

#endif // !defined(AFX_CANVAS_H__9066CECA_82F0_4A75_950D_26D34D23F5E0__INCLUDED_)
