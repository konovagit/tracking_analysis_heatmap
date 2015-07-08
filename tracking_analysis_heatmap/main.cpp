#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "lodepng.h"
#include "heatmap.h"
#include "gray.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>


using namespace std;
using namespace cv;


Mat diff_heatmap(const Mat& I1, const Mat& I2);

double getPSNR(const Mat& I1, const Mat& I2); //for the comparison

int main()
{
    string vide_name="134823_Feng";
    string coordinates="../Res/Video_" + vide_name + "/" + vide_name + "_coordinates.txt";
    cout<<"------generation heatmas----"<<endl;
    cout<<"close windows to continue" <<endl;
    
    string imageName = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap.png";
    string imageName2 = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap9.png";
    
    ////Comparison///
    double psnrV;
    Scalar mssimV;
    string reference="/Users/konova/tracking_analysis_heatmap/Res/heatmap_eq.png";
    string compar="/Users/konova/tracking_analysis_heatmap/Res/heatmap_eq5R25_X.png";
    
    Mat Reference= imread(reference, CV_LOAD_IMAGE_GRAYSCALE);
    Mat Compar= imread(compar, CV_LOAD_IMAGE_GRAYSCALE);
    
    psnrV = getPSNR(Reference,Compar);
    cout<<"Resultat comparison:"<<psnrV<<"%"<<endl;

    /*
    // DIFF
     Mat diff=diff_heatmap(Reference, Compar);
     IplImage *img;
     char key;
     cvNamedWindow("difference between Two heatmap", CV_WINDOW_AUTOSIZE);
     img = new IplImage(diff);
     cvShowImage("difference between Two heatmap", img);
     
     // While we don't want to quit
     while(key != 'Q' && key != 'q') {
     
     key = cvWaitKey(10);
     
     }
     // Destroy the window we have created
     cvDestroyWindow("difference between Two heatmap");
    */
    
    return 0;
}


Mat diff_heatmap(const Mat& I1, const Mat& I2)
{
    string test_test = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/test_test.png";
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2
    
    
    return s1;
}


double getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2
    
    Scalar s = sum(s1);        // sum elements per channel
    
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
    
    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double mse  = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        
        return (psnr*100)/59.44;
    }
}





