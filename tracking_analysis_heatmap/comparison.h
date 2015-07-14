#ifndef __tracking_analysis_heatmap__comparison__
#define __tracking_analysis_heatmap__comparison__

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

#define ar_width 5
#define ar_height 5
#define ar 2

#define width 1280
#define height 720

struct Pixel{
    int x;
    int y;
    long intensity;
};

long scrutation(Mat image1, Mat image2);
long comparison(Pixel pixel, Mat image2);

#endif /* defined(__tracking_analysis_heatmap__comparison__) */
