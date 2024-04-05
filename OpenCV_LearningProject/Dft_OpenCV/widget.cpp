#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCV傅里叶变换频谱图");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btn_loadPic_clicked()
{
    Mat Img = imread("lena.png");
    cvtColor(Img, grayImg, COLOR_BGR2GRAY);

    QImage qImg_Gray = cvMat2QImage(grayImg);
    ui->lbl_grayPic->setPixmap(QPixmap::fromImage(qImg_Gray.scaled(ui->lbl_grayPic->size())));
}


void Widget::on_btn_DftCV_clicked()
{
    Mat img_Dft,img_Trans;
    DftOwn(grayImg,img_Dft,img_Trans);
//    imshow("img_Dft", img_Dft);

    QImage qImg_Dft = cvMat2QImage(img_Dft);
    ui->lbl_dftPic->setPixmap(QPixmap::fromImage(qImg_Dft.scaled(ui->lbl_dftPic->size())));
}


void Widget::on_btn_resetPic_clicked()
{
    ui->lbl_dftPic->clear();
    ui->lbl_grayPic->clear();
}

QImage Widget::cvMat2QImage(const cv::Mat& mat)
{
    if (mat.empty())
    {
        return QImage();
    }
    QImage image;
    switch (mat.type())
    {
    case CV_8UC1:
    {
        image = QImage((const uchar*)(mat.data),
                       mat.cols, mat.rows, static_cast<int>(mat.step),
                       QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8UC2:
    {
        mat.convertTo(mat, CV_8UC1);
        image = QImage((const uchar*)(mat.data),
                       mat.cols, mat.rows, static_cast<int>(mat.step),
                       QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8UC3:
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC4:
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);
        return image.copy();
    }
    case CV_32FC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_32FC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX,-1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_64FC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_64FC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_32SC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_32SC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_16SC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_16SC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, static_cast<int>(normalize_mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8SC1:
    {
        //Mat normalize_mat;
        //normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        mat.convertTo(mat, CV_8U);
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8SC3:
    {
        mat.convertTo(mat, CV_8U);
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    default:
        mat.convertTo(mat, CV_8UC3);
        QImage image((const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
        return QImage();
        break;
    }
}

void Widget::DftOwn(Mat input_Img, Mat &output_Img, Mat &transform_Img)
{
    //1.扩展图像矩阵，为2，3，5的倍数时运算速度快
    int m = getOptimalDFTSize(input_Img.rows);
    int n = getOptimalDFTSize(input_Img.cols);
    copyMakeBorder(input_Img, input_Img, 0, m - input_Img.rows, 0, n - input_Img.cols, BORDER_CONSTANT, Scalar::all(0));

    //2.创建一个双通道矩阵planes，用来储存复数的实部与虚部
    Mat planes[] = { Mat_<float>(input_Img), Mat::zeros(input_Img.size(), CV_32F) };

    //3.从多个单通道数组中创建一个多通道数组:transform_image。函数Merge将几个数组合并为一个多通道阵列，即输出数组的每个元素将是输入数组元素的级联
    merge(planes, 2, transform_Img);

    //4.进行傅立叶变换
    dft(transform_Img, transform_Img);

    //5.计算复数的幅值，保存在output_image（频谱图）
    split(transform_Img, planes); // 将双通道分为两个单通道，一个表示实部，一个表示虚部
    magnitude(planes[0], planes[1], output_Img); //计算复数的幅值，保存在output_image（频谱图）

    //6.前面得到的频谱图数级过大，不好显示，因此转换
    output_Img += Scalar(1);   // 取对数前将所有的像素都加1，防止log0
    log(output_Img, output_Img);   // 取对数
    normalize(output_Img, output_Img, 0, 1, NORM_MINMAX); //归一化

    //7.剪切和重分布幅度图像限
    output_Img = output_Img(Rect(0, 0, output_Img.cols & -2, output_Img.rows & -2));

    // 重新排列傅里叶图像中的象限，使原点位于图像中心
    int cx = output_Img.cols / 2;
    int cy = output_Img.rows / 2;
    Mat q0(output_Img, Rect(0, 0, cx, cy));   // 左上区域
    Mat q1(output_Img, Rect(cx, 0, cx, cy));  // 右上区域
    Mat q2(output_Img, Rect(0, cy, cx, cy));  // 左下区域
    Mat q3(output_Img, Rect(cx, cy, cx, cy)); // 右下区域

    //交换象限中心化
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);//左上与右下进行交换
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);//右上与左下进行交换
}

cv::Mat Widget::QImage2cvMat(QImage& image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB32:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        //cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        //cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
