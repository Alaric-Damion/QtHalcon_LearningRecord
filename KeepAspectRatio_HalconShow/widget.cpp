#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
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

    }
}


void Widget::on_btnPreviousPic_clicked()
{
    qDebug()<<"上一张图像";
    CurPicIndex--;
    QString curPicPath = PicNameInfo.value(CurPicIndex);
//    QFileInfo fileInfo(curPicPath);
//    CurfileName = fileInfo.baseName();
    ui->PicShow->curFileName = CurfileName;
    ui->lineEdit_PicPath->setText(curPicPath);
    LoadHalconImg(curPicPath);
    showImg();
    checkCurPicIndex();
}


void Widget::on_btnNextPic_clicked()
{
    qDebug()<<"下一张图像";
    CurPicIndex++;
    QString curPicPath = PicNameInfo.value(CurPicIndex);
//    QFileInfo fileInfo(curPicPath);
//    CurfileName = fileInfo.baseName();
    ui->PicShow->curFileName = CurfileName;
    ui->lineEdit_PicPath->setText(curPicPath);
    LoadHalconImg(curPicPath);
    showImg();
    checkCurPicIndex();
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
    ui->btnNextPic->setEnabled(false);
    ui->btnPreviousPic->setEnabled(false);
    //初始化listwidget
    ITEM_SIZE.setHeight(120);
    ITEM_SIZE.setWidth(120);
    IMG_SIZE.setHeight(100);
    IMG_SIZE.setWidth(100);

    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setIconSize(IMG_SIZE);
    //    ui->listWidget->setSpacing(10);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);       /* 屏蔽水平滑动条 */
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);         /* 屏蔽垂直滑动条 */
    ui->listWidget->setHorizontalScrollMode(QListWidget::ScrollPerPixel);       /* 设置为像素滚动 */

}

void Widget::showImg()
{

}
