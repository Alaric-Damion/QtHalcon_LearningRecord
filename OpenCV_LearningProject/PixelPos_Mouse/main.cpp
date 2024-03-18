#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include "onMouse.h"
#include <QDebug>

using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Mat image1 = imread("lena.png");
    if(image1.empty()){
        qDebug()<<"读取图像错误";
    }
    imshow("image1",image1);

    setMouseCallback("image1", onMouse, reinterpret_cast<void*>(&image1));


    return a.exec();
}
