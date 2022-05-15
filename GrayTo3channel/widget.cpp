#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"

using namespace cv;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->btn_NextPic->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_trans_clicked()
{
    //保存图像
    if(ui->edt_ImgSavePath->text().isEmpty())
    {
        QMessageBox::critical(NULL, "critical", "未指定保存文件夹，请设置！");
        return;
    }
    else
    {
        Mat curMatImg = QImage2Mat(curImg);
        //判断输入图像是不是8位灰度图像
        if(curMatImg.type() == CV_8UC4)
        {
            cvtColor(curMatImg,curMatImg,COLOR_RGBA2RGB);
            qDebug()<<"width"<<curMatImg.cols<<curMatImg.rows;
        }
        else if(curMatImg.type() == CV_8UC1)
        {
            curMatImg = convertTo3Channels(curMatImg);
        }
        else
        {
            qDebug()<<"Img is 3 Channels";
        }
        QImage QimgTrans = cvMat2QImage(curMatImg);
        QImage ShowTranImg = QimgTrans.scaled(ui->lbl_RGBImg->size(),Qt::KeepAspectRatio);
        ui->lbl_RGBImg->setPixmap(QPixmap::fromImage(ShowTranImg));
        QString SaveFilePath = ui->edt_ImgSavePath->text() + "/trans_" + QString::number(saveFile_Num) +".bmp";
        QimgTrans.save(SaveFilePath);
        saveFile_Num++;
    }
}


Mat Widget::QImage2Mat(QImage image)
{
    Mat cvMat;
    qDebug()<<image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
        cvMat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(cvMat, cvMat, COLOR_BGRA2BGR);
        break;
    case QImage::Format_ARGB32_Premultiplied:
        cvMat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        cvMat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(cvMat, cvMat, COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        cvMat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        cvMat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return cvMat;
}


QImage Widget::cvMat2QImage(const cv::Mat &mat)
{
    switch ( mat.type() )
    {
    // 8-bit  4 channel
    case CV_8UC4:
    {
        QImage image( (const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB32 );
        return image;
    }

        // 8-bit  3 channel
    case CV_8UC3:
    {
        QImage image( (const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888 );
        return image.rgbSwapped();
    }

        // 8-bit  1 channel
    case CV_8UC1:
    {
        static QVector<QRgb> sColorTable;

        // only create our color table once

        if ( sColorTable.isEmpty() )

        {

            for ( int i = 0; i < 256; ++i )

                sColorTable.push_back( qRgb( i, i, i ) );

        }

        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );

        return image;
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        qWarning() << "cvMatToQImage - cv::Mat image type not handled in switch:" << mat.type();
        break;
    }

    return QImage();
}

Mat Widget::convertTo3Channels(const cv::Mat& binImg)
{
    cv::Mat three_channel = cv::Mat::zeros(binImg.rows,binImg.cols,CV_8UC3);
    std::vector<cv::Mat> channels;
    for (int i=0;i<3;i++)
    {
        channels.push_back(binImg);
    }
    //    cv::merge()
    cv::merge(channels,three_channel);
    return three_channel;
}

void Widget::on_btn_close_clicked()
{
    close();
}

void Widget::on_btn_OpenFile_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);          //为了能记住上次打开的路径
    QString lastPath = setting.value("LastFilePath").toString();

    file_path = QFileDialog::getExistingDirectory(this, "请选择文件夹路径...", lastPath);

    if(file_path.isEmpty())
    {
        QMessageBox::warning(this,"Warning","未选择文件夹，请重新选择文件夹！");
        return;
    }
    else
    {
        PicNameInfo.clear();
        CurPicIndex = 0;
        QDir dir(file_path);  //改文件夹已经判断是否存在，保证当前必定存在
        QStringList filter;
        filter<<"*.bmp"<<"*.jpg"<<"*.png";
        QFileInfoList list = dir.entryInfoList(filter, QDir::Files);        //获取文件信息列表
        if(!list.isEmpty())
        {
            //            qDebug() << list.count();//输出图片名
            for(int i=0;i<list.count();i++){
                QFileInfo fileInfo = list.at(i);
                qDebug() << fileInfo.fileName();//输出图片名
                PicNameInfo.insert(i,fileInfo.fileName());
            }
            CurfileName = PicNameInfo.value(0);
            QString picPathShow = file_path + "/" + PicNameInfo.value(0);
            ui->edt_ImgFolderPath->setText(picPathShow);
            curImg.load(picPathShow);
            QImage ShowImg = curImg.scaled(ui->lbl_GrayImg->size(),Qt::KeepAspectRatio);
            ui->lbl_GrayImg->setPixmap(QPixmap::fromImage(ShowImg));
            ui->lbl_RGBImg->clear();
            checkCurPicIndex();
        }
        else
        {
            QMessageBox::warning(this,"Warning","该文件夹为空，请重新选择文件夹！");
            return;
        }
    }

}

void Widget::on_btn_NextPic_clicked()
{
    CurPicIndex++;
    CurfileName = PicNameInfo.value(CurPicIndex);
    QString curPicPath = file_path + "/" + PicNameInfo.value(CurPicIndex);
    ui->edt_ImgFolderPath->setText(curPicPath);
    curImg.load(curPicPath);
    QImage ShowImg = curImg.scaled(ui->lbl_GrayImg->size(),Qt::KeepAspectRatio);
    ui->lbl_GrayImg->setPixmap(QPixmap::fromImage(ShowImg));
    ui->lbl_RGBImg->clear();
    checkCurPicIndex();
}

void Widget::on_btn_LoadPic_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);          //为了能记住上次打开的路径
    QString lastPath = setting.value("LastFilePath").toString();
    QFileInfo file;
    QString PicPath =QFileDialog::getOpenFileName(this,"选择图像文件",lastPath,"图像文件 (*.bmp *.jpg *.png)");

    if(PicPath.isEmpty())
    {
        return;
    }
    else
    {
        file = QFileInfo(PicPath);
        //            CurfileName = file.fileName();  //获取文件名
        //            qDebug() << CurfileName;//输出图片名
        qDebug()<<"加载图像成功";
        //        QImage img(PicPath);
        curImg.load(PicPath);
        QImage ShowImg = curImg.scaled(ui->lbl_GrayImg->size(),Qt::KeepAspectRatio);
        ui->lbl_GrayImg->setPixmap(QPixmap::fromImage(ShowImg));
        ui->lbl_RGBImg->clear();
        //        ui->graphicsView->fitInView(ImageItem);
    }
}

void Widget::checkCurPicIndex()
{
    if(PicNameInfo.size()>1)
    {
        if(CurPicIndex == 0)
        {
            //            ui->btnPreviousPic->setEnabled(false);
            ui->btn_NextPic->setEnabled(true);
        }
        else if(CurPicIndex == PicNameInfo.count()-1)
        {
            ui->btn_NextPic->setEnabled(false);
            //            ui->btnPreviousPic->setEnabled(true);
        }
        else
        {
            //            ui->btnPreviousPic->setEnabled(true);
            ui->btn_NextPic->setEnabled(true);
        }
    }

}
void Widget::on_btn_SaveImgPath_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);          //为了能记住上次打开的路径
    QString lastPath = setting.value("LastFilePath").toString();

    QString save_path = QFileDialog::getExistingDirectory(this, "请选择文件夹路径...", lastPath);
    ui->edt_ImgSavePath->setText(save_path);
    QDir *dir = new QDir(save_path);
    QStringList filter;
    QFileInfoList fileInfoList = dir->entryInfoList(filter);
    saveFile_Num  = fileInfoList.count()-2;
}
