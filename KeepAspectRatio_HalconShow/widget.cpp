#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Halcon等比例图像显示");
    this->show();
    init_Form();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *ev)
{
    if (m_hHalconID != NULL)
    {
        SetWindowExtents(m_hHalconID,0,0,ui->PicShow->width(),ui->PicShow->height());
        showImg();
    }
    QWidget::resizeEvent(ev);
}

void Widget::on_btn_loadMultiPic_clicked()
{
    qDebug()<<"批量加载图像文件";
    QSettings setting("./setting.ini", QSettings::IniFormat);
    QString lastPath = setting.value("LastFilePath").toString();
    QString filePath = QFileDialog::getExistingDirectory(this,"请选择文件夹路径...",lastPath);
    if(filePath.isEmpty()){
        QMessageBox::warning(this,"Warning","该文件夹为空，请重新选择文件夹！");
        return;
    }
    CurPicIndex = 0;
    checkCurPicIndex();
    ui->listWidget->clear();
    PicNameInfo.clear();
    QDir dir(filePath);
    QStringList filter;
    filter<<"*.bmp"<<"*.jpg"<<"*.png";
    QFileInfoList list = dir.entryInfoList(filter, QDir::Files);
    if(!list.isEmpty())
    {
        for(int i=0;i<list.count();i++){
            QFileInfo fileInfo = list.at(i);
            //            qDebug() << fileInfo.fileName();//输出图片名
            QString picPath = fileInfo.filePath();
            QImage QImg = QImage(picPath);
            QPixmap pix = QPixmap::fromImage(QImg);
            //把小图像加载到listwidget中
            QListWidgetItem *listWidgetItem = new QListWidgetItem;
            listWidgetItem->setIcon(QIcon(pix));
            listWidgetItem->setText(fileInfo.fileName());
            listWidgetItem->setSizeHint(ITEM_SIZE);
            ui->listWidget->insertItem(i,listWidgetItem);
            PicNameInfo.insert(i, picPath);
        }
        QString picPathShow = PicNameInfo.value(0);
        ui->listWidget->setCurrentRow(CurPicIndex);
        ui->lineEdit_PicPath->setText(picPathShow);
        LoadHalconImg(picPathShow);
        showImg();
        ui->btnPreviousPic->setEnabled(false);
        if(list.count()>1)
        {
            ui->btnNextPic->setEnabled(true);
        }
        else
        {
            ui->btnNextPic->setEnabled(false);
        }
    }
    else
    {
        qDebug()<<"该文件夹为空";
        QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("该文件夹为空，请重新选择文件夹！"));
        return;
    }
}


void Widget::on_btnPreviousPic_clicked()
{
    qDebug()<<"上一张图像";
    CurPicIndex--;
    QString curPicPath = PicNameInfo.value(CurPicIndex);
//    QFileInfo fileInfo(curPicPath);
//    CurfileName = fileInfo.baseName();

    ui->lineEdit_PicPath->setText(curPicPath);
    LoadHalconImg(curPicPath);
    showImg();
    checkCurPicIndex();
    ui->listWidget->setCurrentRow(CurPicIndex);
}


void Widget::on_btnNextPic_clicked()
{
    qDebug()<<"下一张图像";
    CurPicIndex++;
    QString curPicPath = PicNameInfo.value(CurPicIndex);
//    QFileInfo fileInfo(curPicPath);
//    CurfileName = fileInfo.baseName();

    ui->lineEdit_PicPath->setText(curPicPath);
    LoadHalconImg(curPicPath);
    showImg();
    checkCurPicIndex();
    ui->listWidget->setCurrentRow(CurPicIndex);
}


void Widget::on_btn_AdaptiveScreen_clicked()
{
    showImg();
}


void Widget::on_btn_clearPic_clicked()
{
    qDebug()<<"清空所有图像";
    ui->listWidget->clear();
    ui->lineEdit_PicPath->clear();
    ClearWindow(m_hHalconID);
    ui->btnNextPic->setEnabled(false);
    ui->btnPreviousPic->setEnabled(false);
}

void Widget::init_Form()
{
    openWindowHandle();
    ui->btnNextPic->setEnabled(false);
    ui->btnPreviousPic->setEnabled(false);
    //初始化listwidget
    ITEM_SIZE.setHeight(120);
    ITEM_SIZE.setWidth(120);
    IMG_SIZE.setHeight(100);
    IMG_SIZE.setWidth(100);

    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setIconSize(IMG_SIZE);
//        ui->listWidget->setSpacing(10);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setWrapping(false);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);       /* 屏蔽水平滑动条 */
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);         /* 屏蔽垂直滑动条 */
    ui->listWidget->setHorizontalScrollMode(QListWidget::ScrollPerPixel);       /* 设置为像素滚动 */

}

void Widget::showImg()
{
    HTuple Row, Column, winWidth, winHeight, imgWidth, imgHeight;
    ClearWindow(m_hHalconID);
    GetWindowExtents(m_hHalconID, &Row, &Column, &winWidth, &winHeight);
    GetImageSize(CurImg, &imgWidth, &imgHeight);
    double picWHRatio,winWHRatio;
    picWHRatio = 1.0 * imgWidth.D() / imgHeight.D();
    winWHRatio = 1.0 * winWidth.D() / winHeight.D();
    HTuple dispWidth,dispHeight,offset;

    if(picWHRatio >= winWHRatio)
    {
        dispWidth = imgWidth;
        dispHeight = imgWidth / winWHRatio;
        offset = (dispHeight - imgHeight) / 2;
        SetPart(m_hHalconID, -offset, 0, dispHeight-offset, dispWidth);
    }
    else
    {
        dispWidth = imgHeight * winWHRatio;
        dispHeight = imgHeight;
        offset = (dispWidth - imgWidth) / 2;
        SetPart(m_hHalconID, 0, -offset, dispHeight, dispWidth-offset);
    }
    DispObj(CurImg,m_hHalconID);
}

void Widget::LoadHalconImg(QString imgPath)
{
    try{
        QByteArray ba1 = imgPath.toLocal8Bit();
        const char *string1 = ba1.data();
        HTuple  ImagePart_CurrentName(string1);
        //    HObject oldPic;
        ReadImage(&CurImg,ImagePart_CurrentName);
//        ui->PicShow->setHalconWnd(CurTestImg, m_hHalconID);
    }
    catch(HalconCpp::HException &except){
        qDebug()<<except.ProcName().Text();
        qDebug()<<except.ErrorMessage().Text();
        qDebug()<<except.ErrorCode();
    }
}

void Widget::openWindowHandle()
{
    Hlong windID = (Hlong)this->ui->PicShow->winId();
    OpenWindow(0,0,ui->PicShow->width(),ui->PicShow->height(),windID,"visible","",&m_hHalconID);
}

void Widget::checkCurPicIndex()
{
    if(CurPicIndex == 0)
    {
        ui->btnPreviousPic->setEnabled(false);
        ui->btnNextPic->setEnabled(true);
    }
    else if(CurPicIndex == PicNameInfo.count()-1)
    {
        ui->btnNextPic->setEnabled(false);
        ui->btnPreviousPic->setEnabled(true);
    }
    else
    {
        ui->btnPreviousPic->setEnabled(true);
        ui->btnNextPic->setEnabled(true);
    }
}

void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
        int curRow = ui->listWidget->currentRow();
        CurPicIndex = curRow;
        QString curPicPath = PicNameInfo.value(CurPicIndex);

        ui->lineEdit_PicPath->setText(curPicPath);
        LoadHalconImg(curPicPath);
        showImg();
        checkCurPicIndex();
}

