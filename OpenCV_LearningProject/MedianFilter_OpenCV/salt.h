#ifndef SALT_H
#define SALT_H

#include "opencv2/opencv.hpp"
#include <random>

using namespace cv;
class Salt
{
public:
    Salt();
    Mat setSalt(Mat img, int n);
};

#endif // SALT_H
