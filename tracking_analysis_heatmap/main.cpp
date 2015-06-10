#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "lodepng.h"
#include "heatmap.h"
#include "gray.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>


using namespace std;
using namespace cv;

void generate_heatmap(string filename, double begin, double end);

void conversion_grayscale(string png_name);

IplImage* histogram(IplImage *image);

double getPSNR(const Mat& I1, const Mat& I2); //for the comparison


int main()
{

    
    
    
    return 0;
}

void conversion_grayscale(string png_name)
{
    Mat image = imread(png_name, CV_LOAD_IMAGE_COLOR);
    if(! image.data )
    {
        cout <<  "Could not open or find the image" << endl ;
    }
    // Create a new matrix to hold the gray image
    Mat gray;
    // convert RGB image to gray
    cvtColor(image, gray, CV_BGR2GRAY);
    imwrite( png_name, gray );
}





