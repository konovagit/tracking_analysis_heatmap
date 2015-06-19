#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "lodepng.h"
#include "heatmap.h"
#include "gray.h"

//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio/videoio.hpp>
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
   
    
    vector<float> array,array2,res;
    Mat image = imread( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_heatmap1.png", 1 );
    Mat image2 = imread( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_heatmap2.png", 1 );
    image.convertTo(image, CV_32F);   //need to transform image CV_8U to CV_32F to use comparehist
    image2.convertTo(image2, CV_32F);
    
    array.assign(image.datastart, image.dataend);  //i send all pixels in the vector
    array2.assign(image2.datastart, image2.dataend);
    
    for (vector<float>::iterator it = array.begin() ; it != array.end(); ++it)   //adjust vector with 0 or 1
    {
        if((*it)>0)
        {
            (*it)=1;
        }
    }
    for (vector<float>::iterator it = array2.begin() ; it != array2.end(); ++it)  //adjust vector with 0 or 1
    {
        if((*it)>0)
        {
            (*it)=1;
        }
    }
    
    //int count;
    /*
    ofstream fichier("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/comparison.txt", ios::out | ios::trunc);
    if(fichier)
    {
            for (vector<int>::iterator it = array.begin() ; it != array.end(); ++it)
            {
                if((*it)>0)
                {
                    (*it)=1;
                }
              
     
                if((array[(*it)]-array2[(*it)])!=0)
                {
                    count++;
                }
     
                
                fichier<< (*it)<<endl;
               
            }
            fichier.close();
    }
    else cerr << "Impossible d'ouvrir le fichier !" << endl;
    */
    
    float resultat;

    cout<<norm(array,array2);
    
    //float emd = EMD(array, array2, CV_DIST_L2);
    //cout<<emd;
    
    resultat=compareHist(array, array2, CV_COMP_BHATTACHARYYA);
    cout<<resultat;
    
   
    return 0;
}

