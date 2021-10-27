#include "cmylabel.h"

//定义单步放大倍率
#define ZOOMRATIO 2.0

CMyLabel::CMyLabel(QWidget *parent)
    : QLabel(parent)
{
    m_bIsMove = false;
}

CMyLabel::~CMyLabel()
{
}

//设置Halcon图像和Halcon窗口句柄，用户响应鼠标事件后实时更新图像
void CMyLabel::setHalconWnd(HObject img, HTuple hHalconID, QLabel *label)
{
    m_hHalconID = hHalconID;
    m_currentImg = img;
    m_label = label;
}

//鼠标滚轮缩放事件，用于缩放图像
void CMyLabel::wheelEvent(QWheelEvent *ev)
{
    double Zoom;			//放大或缩小倍率
    HTuple  mouseRow, mouseCol, Button;
    HTuple startRowBf, startColBf, endRowBf, endColBf, Ht, Wt, startRowAft, startColAft, endRowAft, endColAft;

    //滚轮前滑，放大
    if (ev->delta() > 0)
    {
        Zoom = ZOOMRATIO;
    }
    else
    {
        Zoom = 1 / ZOOMRATIO;
    }

    //获取光标在原图上的位置，注意是原图坐标，不是Label下的坐标
    SetCheck("~give_error");
    GetMposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
    SetCheck("give_error");

    //当光标不在Halcon窗口内时返回，否则会报错
//    if (ret != H_MSG_TRUE)
//    {
//        return;
//    }

    //获取原图显示的部分，注意也是原图坐标
    GetPart(m_hHalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);

    //缩放前显示的图像宽高
    Ht = endRowBf - startRowBf;
    Wt = endColBf - startColBf;

    //普通版halcon能处理的图像最大尺寸是32K*32K。如果无限缩小原图像，导致显示的图像超出限制，则会造成程序崩溃
    if ((Ht * Wt < 20000 * 20000 || Zoom == ZOOMRATIO))
    {
        //计算缩放后的图像区域
        startRowAft = mouseRow - ((mouseRow - startRowBf) / Zoom);
        startColAft = mouseCol - ((mouseCol - startColBf) / Zoom);

        endRowAft = startRowAft + (Ht / Zoom);
        endColAft = startColAft + (Wt / Zoom);

        //如果放大过大，则返回
        if (endRowAft - startRowAft < 2)
        {
            return;
        }

        if (m_hHalconID != NULL)
        {
            //如果有图像，则先清空图像
            ClearWindow(m_hHalconID);
        }
        SetPart(m_hHalconID, startRowAft, startColAft, endRowAft, endColAft);
        DispObj(m_currentImg, m_hHalconID);
    }
}

//鼠标按下事件
void CMyLabel::mousePressEvent(QMouseEvent *ev)
{
    HTuple mouseRow, mouseCol, Button,hv_Error;
    SetCheck("~give_error");
    try
     {
       hv_Error = 2;
       GetMposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
     }
     catch(HException e)
     {
       hv_Error = (int)e.ErrorCode();
       if (hv_Error < 0)
         throw e;
     }

    SetCheck("give_error");

    //当光标不在Halcon窗口内时返回，否则会报错
    if (hv_Error != 2)
    {
        return;
    }

    //鼠标按下时的行列坐标
    m_tMouseDownRow = mouseRow;
    m_tMouseDownCol = mouseCol;

    m_bIsMove = true;
}

//鼠标释放事件
void CMyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    m_bIsMove = false;
}

//鼠标移动事件
void CMyLabel::mouseMoveEvent(QMouseEvent *ev)
{
    HTuple startRowBf, startColBf, endRowBf, endColBf, mouseRow, mouseCol, Button;

    //获取当前鼠标在原图的位置
    SetCheck("~give_error");
    GetMposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
    SetCheck("give_error");

    //当光标不在Halcon窗口内时返回，否则会报错
//    if (ret != H_MSG_TRUE)
//    {
//        return;
//    }

    //鼠标按下并移动时，移动图像，否则只显示坐标
    if (m_bIsMove)
    {
        //计算移动值
        double RowMove = mouseRow[0].D() - m_tMouseDownRow[0].D();
        double ColMove = mouseCol[0].D() - m_tMouseDownCol[0].D();

        //得到当前的窗口坐标
        GetPart(m_hHalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);

        //移动图像
        if (m_hHalconID != NULL)
        {
            //如果有图像，则先清空图像
            ClearWindow(m_hHalconID);
        }
        SetPart(m_hHalconID, startRowBf - RowMove, startColBf - ColMove, endRowBf - RowMove, endColBf - ColMove);
        DispObj(m_currentImg, m_hHalconID);
    }

    //获取灰度值
    HTuple pointGray;
    SetCheck("~give_error");
    GetGrayval(m_currentImg, mouseRow, mouseCol, &pointGray);
    SetCheck("give_error");

    //当光标不在Halcon窗口内时返回，否则会报错
//    if (ret != H_MSG_TRUE)
//    {
//        m_label->setText(QString::fromLocal8Bit("X坐标：-    Y坐标：-    灰度值：-"));

//        return;
//    }

    //设置坐标
    m_label->setText(QString::fromLocal8Bit("X坐标：%1    Y坐标：%2    灰度值：%3").arg(mouseCol[0].D()).arg(mouseRow[0].D()).arg(pointGray[0].D()));
}
