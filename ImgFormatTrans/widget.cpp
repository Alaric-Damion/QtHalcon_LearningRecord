#include "widget.h"
#include "ui_widget.h"

#pragma comment  (lib,"User32.lib")
#pragma comment  (lib,"Gdi32.lib")


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->show();
    init_DisplayWindow();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init_DisplayWindow()
{
    //创建Halcon窗口，并绑定控件
    Hlong WinIDcurrent = (Hlong)ui->lbl_DispHalcon->winId();
    OpenWindow(0,0,ui->lbl_DispHalcon->width(),ui->lbl_DispHalcon->height(),WinIDcurrent,"visible","",&hv_WindowHandleCurrent);

    //创建Opencv窗口，并绑定控件
    cv_winName = "lena";
    cv::namedWindow(cv_winName, cv::WINDOW_NORMAL);
    HWND hwnd = (HWND)cvGetWindowHandle(cv_winName.c_str());
    HWND hparent = GetParent(hwnd);
    SetParent(hwnd, (HWND)ui->lbl_DispCV->winId());
    ShowWindow(hparent,SW_HIDE);
}


void Widget::on_btn_LoadImg_clicked()
{
    QString InPut_Format = ui->cmb_InputFormat->currentText();
    QString ImgChannels = ui->cmb_ImgChannels->currentText();
    QString ImagePath;
    qDebug()<<"Format"<<InPut_Format<<ImgChannels;
    //先清空图像显示区
    ClearWindow(hv_WindowHandleCurrent);
    cv::Mat emptyImg;
    emptyImg=cv::Mat::zeros(500,620,CV_8UC3);;
    cv::imshow(cv_winName,emptyImg);
    //    ui->lbl_DispCV->clear();
    ui->lbl_DispQImage->clear();

    if(ImgChannels == "Gray")
    {
        ImagePath = "C:\\Users\\Administrator\\Desktop\\testImg\\InputImg\\Lena_Gray.jpg";
    }
    else
    {
        ImagePath = "C:\\Users\\Administrator\\Desktop\\testImg\\InputImg\\lena.png";
    }
    if(InPut_Format == "HObject")
    {
        QByteArray ba1 = ImagePath.toLocal8Bit();
        const char *string1 = ba1.data();
        HTuple  ImagePart_CurrentName(string1);
        ReadImage(&InPut_HobjectImg,ImagePart_CurrentName);
        DispHObject(InPut_HobjectImg);
    }
    else if(InPut_Format == "Mat")
    {
        InPut_MatImg = cv::imread(ImagePath.toStdString());
        DispMat(InPut_MatImg);
    }
    else
    {
        InPut_QImg.load(ImagePath);
        DispQImage(InPut_QImg);
    }
}

void Widget::on_btn_TransImg_clicked()
{
    QString InPut_Format = ui->cmb_InputFormat->currentText();
    QString OutPut_Format = ui->cmb_OutputFormat->currentText();

    if(InPut_Format == OutPut_Format)
    {
        QMessageBox::warning(this, "Warning!", "转换格式相同，请检查!");
        return;
    }
    else
    {
        if(InPut_Format == "HObject" && OutPut_Format == "Mat" )            //HObject2Mat
        {
            OutPut_MatImg = HObject2Mat(InPut_HobjectImg);
            DispMat(OutPut_MatImg);
        }
        else if(InPut_Format == "HObject" && OutPut_Format == "QImage" )    //HObject2Qimage
        {
            //            HObjectToQImage(InPut_HobjectImg,OutPut_QImg);
            OutPut_QImg = HObject2QImage(InPut_HobjectImg);
            DispQImage(OutPut_QImg);
        }
        else if(InPut_Format == "Mat" && OutPut_Format == "HObject" )       //Mat2HObject
        {
            OutPut_HobjectImg = Mat2Hobject(InPut_MatImg);
            DispHObject(OutPut_HobjectImg);
        }
        else if(InPut_Format == "Mat" && OutPut_Format == "QImage" )        //Mat2QImage
        {
            OutPut_QImg = cvMat2QImage(InPut_MatImg);
            DispQImage(OutPut_QImg);
        }
        else if(InPut_Format == "QImage" && OutPut_Format == "Mat" )        //QImage2Mat
        {
            OutPut_MatImg = QImage2Mat(InPut_QImg);
            DispMat(OutPut_MatImg);
        }
        else                                                                //QImage2HObject
        {
            OutPut_HobjectImg = QImage2HObject(InPut_QImg);
            DispHObject(OutPut_HobjectImg);
        }
    }

}

void Widget::DispQImage(QImage QImg)
{
    QImage curImg = QImg.scaled(ui->lbl_DispQImage->size(), Qt::KeepAspectRatio);
    ui->lbl_DispQImage->setPixmap(QPixmap::fromImage(curImg));
}

void Widget::DispMat(Mat cvImg)
{
    cv::imshow(cv_winName,cvImg);
    cv::resizeWindow(cv_winName,cv::Size(ui->lbl_DispCV->width(),ui->lbl_DispCV->height()));
}

void Widget::DispHObject(HObject hvImg)
{
    HTuple Img_height,Img_width;
    GetImageSize(hvImg,&Img_width,&Img_height);
    SetPart(hv_WindowHandleCurrent, 0, 0, Img_height-1, Img_width-1);
    DispObj(hvImg,hv_WindowHandleCurrent);
}
//QImage转Mat
Mat Widget::QImage2Mat(QImage image)
{
    Mat cvMat;
    qDebug()<<image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
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
//Mat转QImage
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
        static QVector<QRgb>  sColorTable;
        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );
            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }
        QImage image( (const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Indexed8 );
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
//QImage转HObject
HObject Widget::QImage2HObject(QImage qimage)
{
    HObject hv_image;
    int width = qimage.width();
    int height = qimage.height();
    QImage::Format format = qimage.format();
    if(format == QImage::Format_RGB32 ||
            format == QImage::Format_ARGB32 ||
            format == QImage::Format_ARGB32_Premultiplied)
    {
        GenImageInterleaved(&hv_image,(Hlong)qimage.bits(),"bgrx",qimage.width(),qimage.height(),0,"byte",width, height, 0, 0, 8, 0);
    }
    else if(format == QImage::Format_RGB888)
    {
        GenImageInterleaved(&hv_image,(Hlong)qimage.bits(),"bgr",qimage.width(),qimage.height(),0,"byte",width, height, 0, 0, 8, 0);
    }
    else if(format == QImage::Format_Grayscale8 || format == QImage::Format_Indexed8)
    {
        GenImage1(&hv_image,"byte", width, height, (Hlong)qimage.bits());
    }
    else
    {

    }

    return hv_image;
}

//此方法单通道图像转换会出错
void Widget::HObjectToQImage(HObject hv_image, QImage &qimage)
{
    //    QImage qimage;
    HTuple hChannels,htype,hpointer;
    HTuple width=0;
    HTuple height=0;
    ConvertImageType(hv_image,&hv_image,"byte");//将图片转化成byte类型
    CountChannels(hv_image,&hChannels);       //判断图像通道数

    if(hChannels[0].I()==1)//单通道图
    {
        unsigned char *ptr;
        GetImagePointer1(hv_image,&hpointer,&htype,&width,&height);
        ptr=(unsigned char *)hpointer[0].L();
        qimage= QImage(ptr,width,height,QImage::Format_Indexed8);
    }
    else if(hChannels[0].I()==3)//三通道图
    {
        unsigned char *ptr3;
        HObject ho_ImageInterleaved;

        rgb3_to_interleaved(hv_image, &ho_ImageInterleaved);
        GetImagePointer1(ho_ImageInterleaved, &hpointer, &htype, &width, &height);

        ptr3=(unsigned char *)hpointer[0].L();
        qimage= QImage(ptr3,width/3,height,width,QImage::Format_RGB888);
    }
    else
    {

    }
    //    return qimage;
}

void Widget::rgb3_to_interleaved (HObject ho_ImageRGB, HObject *ho_ImageInterleaved)
{
    // Local iconic variables
    HObject  ho_ImageAffineTrans, ho_ImageRed, ho_ImageGreen;
    HObject  ho_ImageBlue, ho_RegionGrid, ho_RegionMoved, ho_RegionClipped;

    // Local control variables
    HTuple  hv_PointerRed, hv_PointerGreen, hv_PointerBlue;
    HTuple  hv_Type, hv_Width, hv_Height, hv_HomMat2DIdentity;
    HTuple  hv_HomMat2DScale;

    GetImagePointer3(ho_ImageRGB, &hv_PointerRed, &hv_PointerGreen, &hv_PointerBlue,
                     &hv_Type, &hv_Width, &hv_Height);
    GenImageConst(&(*ho_ImageInterleaved), "byte", hv_Width*3, hv_Height);
    //
    HomMat2dIdentity(&hv_HomMat2DIdentity);
    HomMat2dScale(hv_HomMat2DIdentity, 1, 3, 0, 0, &hv_HomMat2DScale);
    AffineTransImageSize(ho_ImageRGB, &ho_ImageAffineTrans, hv_HomMat2DScale, "constant",
                         hv_Width*3, hv_Height);
    //
    Decompose3(ho_ImageAffineTrans, &ho_ImageRed, &ho_ImageGreen, &ho_ImageBlue);
    GenGridRegion(&ho_RegionGrid, 2*hv_Height, 3, "lines", hv_Width*3, hv_Height+1);
    MoveRegion(ho_RegionGrid, &ho_RegionMoved, -1, 0);
    ClipRegion(ho_RegionMoved, &ho_RegionClipped, 0, 0, hv_Height-1, (3*hv_Width)-1);

    ReduceDomain(ho_ImageRed, ho_RegionClipped, &ho_ImageRed);
    MoveRegion(ho_RegionGrid, &ho_RegionMoved, -1, 1);
    ClipRegion(ho_RegionMoved, &ho_RegionClipped, 0, 0, hv_Height-1, (3*hv_Width)-1);
    ReduceDomain(ho_ImageGreen, ho_RegionClipped, &ho_ImageGreen);
    MoveRegion(ho_RegionGrid, &ho_RegionMoved, -1, 2);
    ClipRegion(ho_RegionMoved, &ho_RegionClipped, 0, 0, hv_Height-1, (3*hv_Width)-1);
    ReduceDomain(ho_ImageBlue, ho_RegionClipped, &ho_ImageBlue);
    OverpaintGray((*ho_ImageInterleaved), ho_ImageRed);
    OverpaintGray((*ho_ImageInterleaved), ho_ImageGreen);
    OverpaintGray((*ho_ImageInterleaved), ho_ImageBlue);
    return;
}

//HObject转Mat
Mat Widget::HObject2Mat(HObject Hobj)
{
    HTuple htCh;
    HString cType;
    cv::Mat Image;
    ConvertImageType(Hobj, &Hobj, "byte");
    CountChannels(Hobj, &htCh);
    Hlong wid = 0;
    Hlong hgt = 0;
    if (htCh[0].I() == 1)
    {
        HImage hImg(Hobj);
        void *ptr = hImg.GetImagePointer1(&cType, &wid, &hgt);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
        int W = wid;
        int H = hgt;
        Image.create(H, W, CV_8UC1);
        unsigned char *pdata = static_cast<unsigned char *>(ptr);
        memcpy(Image.data, pdata, W*H);
    }
    else if (htCh[0].I() == 3)
    {
        void *Rptr;
        void *Gptr;
        void *Bptr;
        HImage hImg(Hobj);
        hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &wid, &hgt);
        int W = wid;
        int H = hgt;
        Image.create(H, W, CV_8UC3);
        std::vector<cv::Mat> VecM(3);
        VecM[0].create(H, W, CV_8UC1);
        VecM[1].create(H, W, CV_8UC1);
        VecM[2].create(H, W, CV_8UC1);
        unsigned char *R2 = (unsigned char *)Rptr;
        unsigned char *G = (unsigned char *)Gptr;
        unsigned char *B = (unsigned char *)Bptr;
        memcpy(VecM[2].data, R2, W*H);
        memcpy(VecM[1].data, G, W*H);
        memcpy(VecM[0].data, B, W*H);
        cv::merge(VecM, Image);
    }
    return Image;
}
//Mat转Hobject
HObject Widget::Mat2Hobject(Mat& image)
{
    HObject Hobj = HObject();
    int hgt = image.rows;
    int wid = image.cols;
    int i;
    if (image.type() == CV_8UC3)
    {
        std::vector<Mat> imgchannel;
        split(image, imgchannel);
        Mat imgB = imgchannel[0];
        Mat imgG = imgchannel[1];
        Mat imgR = imgchannel[2];
        uchar* dataR = new uchar[hgt*wid];
        uchar* dataG = new uchar[hgt*wid];
        uchar* dataB = new uchar[hgt*wid];
        for (i = 0; i < hgt; i++)
        {
            memcpy(dataR + wid*i, imgR.data + imgR.step*i, wid);
            memcpy(dataG + wid*i, imgG.data + imgG.step*i, wid);
            memcpy(dataB + wid*i, imgB.data + imgB.step*i, wid);
        }
        GenImage3(&Hobj, "byte", wid, hgt, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
        delete[]dataR;
        delete[]dataG;
        delete[]dataB;
        dataR = NULL;
        dataG = NULL;
        dataB = NULL;
    }
    else if (image.type() == CV_8UC1)
    {
        uchar* data = new uchar[hgt*wid];
        for (i = 0; i < hgt; i++)
            memcpy(data + wid*i, image.data + image.step*i, wid);
        GenImage1(&Hobj, "byte", wid, hgt, (Hlong)data);
        delete[] data;
        data = NULL;
    }
    return Hobj;
}
//Halcon中的HObject类型转QImage类型
QImage Widget::HObject2QImage(const HalconCpp::HObject &Hobj)
{
    HalconCpp::HTuple htCh = HalconCpp::HTuple();
    HalconCpp::HTuple hv_Length;
    HalconCpp::HTuple cType;
    QImage Image;
    HalconCpp::HObject ho_img;
    ConvertImageType(Hobj, &ho_img, "byte");
    CountChannels(ho_img, &htCh);
    TupleLength(htCh, &hv_Length);
    if (hv_Length.L() == 0)
    {
        return Image;
    }

    HalconCpp::HTuple width;
    HalconCpp::HTuple height;
    int W = 0, H = 0;

    if (htCh[0].I() == 1)
    {
        HalconCpp::HTuple ptr;
        GetImagePointer1(ho_img, &ptr, &cType, &width, &height);
        W = (int)width;
        H = (int)height;
        BYTE *p = (BYTE *)ptr[0].L(); //必须是L(),不能是I()

        //Set buf data
        QImage tmp = QImage(W, H, QImage::Format_Indexed8);
        Image = tmp.copy();
        memcpy(Image.bits(), p, W * H); //单通道

        //Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> vtColorTable;
        for (int k = 0; k < 256; ++k)
        {
            vtColorTable.push_back(qRgb(k, k, k));
        }

        Image.setColorTable(vtColorTable);
    }
    else if (htCh[0].I() == 3)
    {
        HalconCpp::HTuple ptrR, ptrG, ptrB;
        GetImagePointer3(ho_img, &ptrR, &ptrG, &ptrB, &cType, &width, &height);
        W = (int)width;
        H = (int)height;
        uchar *pr = (uchar *)ptrR[0].L(); //三通道
        uchar *pg = (uchar *)ptrG[0].L();
        uchar *pb = (uchar *)ptrB[0].L();

        QImage tmp = QImage(W, H, QImage::Format_RGB888);
        Image = tmp.copy();

#if 0 //该方法行不通
        for (int row = 0; row < height; row++)
        {
            QRgb *line = reinterpret_cast<QRgb *>(Image.scanLine(row));
            for (int col = 0; col < width; col++)
            {
                line[col] = qRgb(*pr++, *pg++, *pb++);
            }
        }
#else //该方法可行
        int bytesPerLine = W * 3;
        int lineHeadId = 0;
        int pixId = 0;
        BYTE *data24 = Image.bits();

        for (int i = 0; i < H; i++)
        {
            lineHeadId = bytesPerLine * i;
            for (int k = 0; k < W; k++)
            {
                pixId = lineHeadId + k * 3;
                data24[pixId + 0] = pr[W * i + k];
                data24[pixId + 1] = pg[W * i + k];
                data24[pixId + 2] = pb[W * i + k];
            }
        }
#endif
    }

    return Image;
}
