#include "comparison.h"

long scrutation(Mat image1, Mat image2)
{
    /*Variables*/
    Pixel pixel;
    long distance=0;
    
    /*Image Model LOOP*/
    for (int i=0; i<(image1.rows-1); i++)  //Rows
    {
        for (int j=0; j<(image1.cols-1); j++) //Cols
        {
            pixel.intensity=image1.at<uchar>(i,j);
            pixel.x=i;
            pixel.y=j;
            if(pixel.intensity==image2.at<uchar>(i,j))
            {
                //nothing
            }
            else distance+=comparison(pixel, image2);
        }
    }
    return distance;
}

long comparison(Pixel pixel,Mat image2) //pixel => struc image model   //image2 =>image for comparison

{
    /*Variables*/
    int row=0, col=0;
    long distance=0;
    Pixel pixel2=pixel; //set pixel2 at the value of model
    
    
    /*Area LOOP*/
    for ( int i=(pixel.x-ar); row<(ar_height); i++ )  //Rows
    {
        for ( int j=(pixel.y-ar); col<(ar_width); j++ ) //Cols
        {
            if ((pixel.x-ar<0) || (pixel.y-ar<0))
            {
                //nothing out of the image
            }
            else if(image2.at<uchar>(i,j)== pixel.intensity)
            {
                return distance=0;   //one pixel match
            }
            else if (pixel2.intensity==pixel.intensity)  //first loop
            {
                pixel2.intensity=image2.at<uchar>(i,j);
                distance=(abs(pixel2.intensity-pixel.intensity));
            }
            else if((abs(image2.at<uchar>(i,j)-pixel.intensity)<distance))  //update distance if --
            {
                distance=abs(image2.at<uchar>(i,j)-pixel.intensity);
            }
            col++;
        }
        row++;
        col=0;
    }
    
    return distance;
}
