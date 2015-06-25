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

#define size_width 100
#define size_height 100

#define area_width 5
#define area_height 5
#define area 2

struct Pixel{
    int x;
    int y;
    float intensity;
};

long scrutation(Mat image1, Mat image2);
long comparison(Pixel pixel,Mat image1, Mat image2);



int main()
{
   
    string image="/Users/konova/tracking_analysis_heatmap/Res/heatmap_color.png";
    string image2="/Users/konova/tracking_analysis_heatmap/Res/heatmap_color2.png";
    
    //Resize image
    Size size(100,100);//the dst image size,e.g.100x100
    Mat img=imread(image, CV_LOAD_IMAGE_COLOR);
    Mat img2=imread(image2, CV_LOAD_IMAGE_COLOR);
    
    resize(img,img,size);//resize image
    resize(img2,img2,size);//resize image
    
    imwrite("/Users/konova/tracking_analysis_heatmap/Res/heatmap_gray_small.png", img);
    imwrite("/Users/konova/tracking_analysis_heatmap/Res/heatmap_gray6_small.png", img2);
    
    long distance=0;
    distance=scrutation(img,img2);
    cout<<"Distance:"<<distance<<endl;
    return 0;
}


long scrutation(Mat image1, Mat image2)
{
    Pixel pixel;
    long distance=0;
    for (int i=0; i<(size_width-1); i++)  //Rows
    {
        for (int j=0; j<(size_height-1); j++) //Cols
        {
            pixel.intensity=image1.at<uchar>(i,j);
            pixel.x=i;
            pixel.y=j;
            if(pixel.intensity==image2.at<uchar>(i,j))
            {
                //nothing
            }
            else distance+=comparison(pixel, image1, image2);
        }
    }
    return distance;
}

long comparison(Pixel pixel,Mat image1, Mat image2)
{
    int row=0, col=0;
    long distance=0;
    char match=0;
    
    Point2f a(pixel.x,pixel.y);
    
    for ( int i=(pixel.x-area); row<(area_width); i++ )  //Rows
    {
       for ( int j=(pixel.y-area); col<(area_height); j++ ) //Cols
       {
           if ((pixel.x-area<0) || (pixel.y-area<0))
           {
                        //nothing out of the image
           }
           else if(image2.at<uchar>(i,j)==pixel.intensity)
           {
               Point2f b(i,j);
               if (distance==0) {
                   distance= norm(Mat(a), Mat(b));
               }
               else //if (((norm(Mat(a), Mat(b))<distance)))
               {
                   distance= norm(Mat(a), Mat(b));
               }
               match++;
           }
           col++;
        }
        row++;
        col=0;
    }
    if(match>0)
    {
        return distance;
    }
    else return distance+25;
    
}






















