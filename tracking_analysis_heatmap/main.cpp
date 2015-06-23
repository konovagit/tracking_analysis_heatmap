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
    /*
    vector<float> array,array2,res;
    Mat image = imread( "/Users/konova/tracking_analysis_heatmap/Res/heatmap_gray.png", 1 );
    Mat image2 = imread( "/Users/konova/tracking_analysis_heatmap/Res/heatmap_gray2.png", 1 );
    image.convertTo(image, CV_32F);   //need to transform image CV_8U to CV_32F to use comparehist
    image2.convertTo(image2, CV_32F);
    
    array.assign(image.datastart, image.dataend);  //i send all pixels in the vector
    array2.assign(image2.datastart, image2.dataend);
    
    for (vector<float>::iterator it = array.begin() ; it != array.end(); ++it)   //adjust vector  0 to 1
    {
        if((*it)>0)
        {
            (*it)/=255;
        }
    }
    for (vector<float>::iterator it = array2.begin() ; it != array2.end(); ++it)  //adjust vector  0 to 1
    {
        if((*it)>0)
        {
            (*it)/=255;
        }
    }
    */
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
    
    //read 2 images for histogram comparing
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Mat imgA, imgB;
    imgA = imread("/Users/konova/tracking_analysis_heatmap/Res/heatmap_gray.png");
    imgB = imread("/Users/konova/tracking_analysis_heatmap/Res/heatmap_gray2.png");
    
    
    //imshow("img1", imgA);
    //imshow("img2", imgB);
    
    
    //variables preparing
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int hbins = 720, vbins = 720;
  

    //normalization
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    normalize(imgA, imgA,  0, 1, CV_MINMAX);
    
    normalize(imgB, imgB, 0, 1, CV_MINMAX);
    
    //compare histogram
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int numrows = hbins * vbins;
    
    //make signature
    Mat sig1(numrows, 3, CV_32FC1);    //row, col, type
    Mat sig2(numrows, 3, CV_32FC1);
    
    //fill value into signature       //Signature= bins value, index x, index y
    for(int h=0; h< hbins; h++)
    {
        for(int s=0; s< vbins; ++s)
        {
            float binval = imgA.at< float>(h,s);  //bins value
            
            //Formating
            sig1.at< float>( h*vbins + s, 0) = binval;
            sig1.at< float>( h*vbins + s, 1) = h;
            sig1.at< float>( h*vbins + s, 2) = s;
            
            binval = imgB.at< float>(h,s);      //bins value
            
            //Formating
            sig2.at< float>( h*vbins + s, 0) = binval;
            sig2.at< float>( h*vbins + s, 1) = h;
            sig2.at< float>( h*vbins + s, 2) = s;
        }  
    }  
    
    //compare similarity of 2images using emd.  
    float emd = EMD(sig1, sig2, CV_DIST_L2); //emd 0 is best matching.
    cout<<"EMD: "<<emd<<endl;
   // printf("similarity %5.5f %%\n", (1-emd)*100 );
    
    
/*
 
    float resultat;
    cout<<"Euclidian"<<norm(array,array2)<<endl;
    resultat=compareHist(array, array2, CV_COMP_CORREL);
    cout<<"correlation: "<<resultat<<endl;
    resultat=compareHist(array, array2,  CV_COMP_CHISQR);
    cout<<"Chi-Square: "<<resultat<<endl;
    resultat=compareHist(array, array2,  CV_COMP_INTERSECT );
    cout<<"Intersection: "<<resultat<<endl;
    resultat=compareHist(array, array2, CV_COMP_BHATTACHARYYA);
    cout<<"Bhattacharyya distance: "<<resultat<<endl;
 */
    
   
    return 0;
}

