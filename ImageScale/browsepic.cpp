#include "BrowsePic.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
BrowsePic::BrowsePic(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //初始化
    init();
}

BrowsePic::~BrowsePic()
{

}

//初始化
void BrowsePic::init()
{
    //设置halcon的文件路径为utf8，解决中文乱码
    SetSystem("filename_encoding", "utf8");

    //生成空图像
    GenEmptyObj(&m_hCurrentImg);

    //获取图像显示控件的句柄
    m_hHalconID = NULL;
    m_hLabelID = (Hlong)ui.label_show->winId();

    m_bIsEqualScale = false;
    m_bIsShowCenter = true;

    ui.checkBox_equalScale->setChecked(m_bIsEqualScale);
    ui.checkBox_showCenter->setChecked(m_bIsShowCenter);
    ui.checkBox_showCenter->setEnabled(false);
}

//打开窗口
void BrowsePic::showImg()
{
    //判断图像是否为空
    HTuple isEqual;
    HObject emptyObj;
    GenEmptyObj(&emptyObj);
    TestEqualObj(emptyObj, m_hCurrentImg, &isEqual);

    if (isEqual.D())
    {
        return;
    }

    if (m_hHalconID != NULL)
    {
        //如果有图像，则先清空图像
        ClearWindow(m_hHalconID);
    }
    else
    {
        //打开窗口
        OpenWindow(0, 0, ui.label_show->width(), ui.label_show->height(), m_hLabelID, "visible", "", &m_hHalconID);

    }

    ui.label_show->setHalconWnd(m_hCurrentImg, m_hHalconID, ui.label_status);


    GetImageSize(m_hCurrentImg, &m_imgWidth, &m_imgHeight);

    //判断是否等比缩放
    if (m_bIsEqualScale)
    {
        //获取缩放系数
        TupleMin2(1.0 * ui.label_show->width() / m_imgWidth, 1.0 * ui.label_show->height() / m_imgHeight, &m_hvScaledRate);

        //进行图像缩放
        ZoomImageFactor(m_hCurrentImg, &m_hResizedImg, m_hvScaledRate, m_hvScaledRate, "constant");

        GetImageSize(m_hResizedImg, &m_scaledWidth, &m_scaledHeight);
        SetWindowExtents(m_hHalconID, 0, 0, m_scaledWidth, m_scaledHeight);

        if (m_bIsShowCenter)
        {
            if (1.0*ui.label_show->width() / m_imgWidth < 1.0*ui.label_show->height() / m_imgHeight)
            {
                SetWindowExtents(m_hHalconID, ui.label_show->height() / 2.0 - m_scaledHeight / 2.0, 0, ui.label_show->width(), m_scaledHeight);
            }
            else
            {
                SetWindowExtents(m_hHalconID, 0, ui.label_show->width() / 2.0 - m_scaledWidth / 2.0, m_scaledWidth, ui.label_show->height());
            }
        }

    }
    else
    {
        SetWindowExtents(m_hHalconID, 0, 0, ui.label_show->width(), ui.label_show->height());
    }

    SetPart(m_hHalconID, 0, 0, m_imgHeight - 1, m_imgWidth - 1);
    DispObj(m_hCurrentImg, m_hHalconID);
}

//窗体大小改变事件
void BrowsePic::resizeEvent(QResizeEvent *ev)
{
    if (m_hHalconID != NULL)
    {
        //防止窗口闪烁
        SetSystem("flush_graphic","false");
        showImg();
        SetSystem("flush_graphic", "true");
        ClearWindow(m_hHalconID);

        DispObj(m_hCurrentImg, m_hHalconID);

    }
}

//打开图片
void BrowsePic::on_btn_openPic_clicked()
{
    //打开选择文件对话框，选择文件
    QString qtFilePath = QFileDialog::getOpenFileName(this, "选择图片", "./", "图像文件(*.jpg *.png *.bmp *.tif)");

    if (qtFilePath.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(qtFilePath);

    qtFilePath.replace("/", "\\");
    QString hPath = fileInfo.path().replace("/", "\\");

    //由halcon算子获取选中的文件夹中的所有文件
    ListFiles(hPath.toStdString().c_str(), HTuple("files").Append("follow_links"), &m_imgFiles);

    //筛选符合图片格式的文件
    TupleRegexpSelect(m_imgFiles, HTuple("\\.(jpg|png|bmp|tif)$").Append("ignore_case"), &m_imgFiles);

    //找到选择的文件

    TupleFind(m_imgFiles, qtFilePath.toStdString().c_str(), &index);

    //读取图像
    ReadImage(&m_hCurrentImg, HTuple(m_imgFiles[index]));

    showImg();
}

//浏览前一张
void BrowsePic::on_btn_prePic_clicked()
{
    if (index.D()>0&&index[0].D() > 0)
    {
        index[0] = index[0].D() - 1;
        ReadImage(&m_hCurrentImg, HTuple(m_imgFiles[index]));
        showImg();
    }

}

//浏览后一张
void BrowsePic::on_btn_nextPic_clicked()
{
    if (index.D()>0 && index[0].D() < m_imgFiles.D() - 1)
    {
        index[0] = index[0].D() + 1;
        ReadImage(&m_hCurrentImg, HTuple(m_imgFiles[index]));
        showImg();
    }
}

//恢复图片
void BrowsePic::on_btn_resetPic_clicked()
{
    showImg();
}

//等比缩放复选框状态改变
void BrowsePic::on_checkBox_equalScale_stateChanged(int state)
{
    ui.checkBox_showCenter->setEnabled(state);
    m_bIsEqualScale = state;
    showImg();
}

//居中显示复选框状态改变
void BrowsePic::on_checkBox_showCenter_stateChanged(int state)
{
    m_bIsShowCenter = state;
    showImg();
}
