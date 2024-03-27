#include "salt.h"

Salt::Salt()
{

}

Mat Salt::setSalt(Mat img, int n)
{
    Mat saltImg = img.clone();
    std::default_random_engine genNum;
    // c++11 生成随机数
    std::uniform_int_distribution<int>randomRow(0, img.rows-1);
    std::uniform_int_distribution<int>randomCol(0, img.cols-1);

    int i,j;
    for(int k=0;k<n;k++)
    {
        i = randomRow(genNum);
        j = randomCol(genNum);
        if(img.channels() == 1)
        {
            saltImg.at<uchar>(j,i) = 255;
        }
        else if(img.channels() == 3)
        {
            saltImg.at<Vec3b>(j,i)[0] = 255;
            saltImg.at<Vec3b>(j,i)[1] = 255;
            saltImg.at<Vec3b>(j,i)[2] = 255;
        }
    }
    return saltImg;
}
