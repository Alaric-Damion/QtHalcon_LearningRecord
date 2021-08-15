//////////////////////////////////////////////////////////////////////
// 
// 画布类
// 功能：用来替代窗口的设备句柄进行图像的绘制
// 消除绘制时产生的屏幕闪烁
// 使用说明：
// 先使用 Create 创建画布所需的对象，并且传入画布的大小
// 接下来可以直接将数据绘制到画布上，最后把图像输出到设备上，如下：
// CCanvas canvas;
// canvas.Create(&dc,332,153);
// canvas.GetDC()->Rectange(1,1,100,50);
// OutToDC(&dc,CRect(100,100,100+332,100+153));
// 
// 作者：梅文海
// 日期：2011.3.31
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
    // 创建画布
    void Create(CDC* a_pDC,const int a_iWidth,const int a_iHeight){
        m_dcMem.CreateCompatibleDC(a_pDC);
        m_bit.CreateCompatibleBitmap(a_pDC,a_iWidth,a_iHeight);
        m_dcMem.SelectObject(&m_bit);
    }
    void Destroy(){
        m_dcMem.DeleteDC();
        m_bit.DeleteObject();
    }
    // 绘制图像到画布
    void Draw(CBMP* a_pBMP,LPRECT a_rRect){
        PaintDIB(m_dcMem,a_rRect,a_pBMP->GetHDIB(),a_pBMP->GetRect(),a_pBMP->GetPalette());
    }
    // 输出到设备
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
