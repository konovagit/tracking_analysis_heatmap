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
   
    
    vector<int> array,array2,res;
    Mat image = imread( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_heatmap1.png", 1 );
    Mat image2 = imread( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_heatmap3.png", 1 );
    
    array.assign(image.datastart, image.dataend);
    array2.assign(image2.datastart, image2.dataend);
    
    for (vector<int>::iterator it = array.begin() ; it != array.end(); ++it)
    {
        if((*it)>0)
        {
            (*it)=1;
        }
    }
    for (vector<int>::iterator it = array2.begin() ; it != array2.end(); ++it)
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
    
    //double resultat;

    cout<<norm(array,array2);
    //resultat=compareHist(array, array, CV_COMP_BHATTACHARYYA);
    //printf("Resultat:%f",resultat);
    
   
    return 0;
}

