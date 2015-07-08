#include <vector>
#include <iostream>
#include <fstream>
#include <string>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>



using namespace std;
using namespace cv;


int main()
{
   
    Mat imgA, imgB;
    imgA = imread("/Users/konova/tracking_analysis_heatmap/Res/heatmap_color.png");
    imgB = imread("/Users/konova/tracking_analysis_heatmap/Res/heatmap_color5test.png");
    
    
    imshow("img1", imgA);
    imshow("img2", imgB);
    
    waitKey(0);
    
    //variables preparing
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int hbins = 30, sbins = 32;
    int channels[] = {0,  1};
    int histSize[] = {hbins, sbins};
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 255 };
    const float* ranges[] = { hranges, sranges};
    
    Mat patch_HSV;
    MatND HistA, HistB;
    
    //cal histogram & normalization
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //cvtColor(imgA, patch_HSV, CV_BGR2HSV);
    
    calcHist( &imgA, 1, channels,  Mat(), // do not use mask
             HistA, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    normalize(HistA, HistA,  0, 1, CV_MINMAX);
    
    
    //cvtColor(imgB, patch_HSV, CV_BGR2HSV);
    calcHist( &imgB, 1, channels,  Mat(),// do not use mask
             HistB, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    normalize(HistB, HistB, 0, 1, CV_MINMAX);

    //compare histogram
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int numrows = hbins * sbins;
    
    //make signature
    Mat sig1(numrows, 3, CV_32FC1);
    Mat sig2(numrows, 3, CV_32FC1);
    
    //fill value into signature
    for(int h=0; h< hbins; h++)
    {
        for(int s=0; s< sbins; ++s)
        {
            float binval = HistA.at< float>(h,s);
            sig1.at< float>( h*sbins + s, 0) = binval;
            sig1.at< float>( h*sbins + s, 1) = h;
            sig1.at< float>( h*sbins + s, 2) = s;
            
            binval = HistB.at< float>(h,s);
            sig2.at< float>( h*sbins + s, 0) = binval;
            sig2.at< float>( h*sbins + s, 1) = h;
            sig2.at< float>( h*sbins + s, 2) = s;  
        }  
    }  
    
    //compare similarity of 2images using emd.  
    float emd = EMD(sig1, sig2, CV_DIST_L2); //emd 0 is best matching.
    printf("similarity %5.5f %%\n", (1-emd)*100 );
    printf("EMD %f",emd);
    
    waitKey(0);     
    
    return 0;
}



