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


void create_histogram(string path_save, string image);



int main()
{
    string vide_name="134823_Feng";
    string coordinates="../Res/Video_" + vide_name + "/" + vide_name + "_coordinates.txt";
    cout<<"------generation heatmas----"<<endl;
    cout<<"close windows to continue" <<endl;
    
    
    generate_heatmap("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/134823_Feng_coordinates.txt", 5, 439);
    string imageName = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap.png";
    string imageName2 = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap9.png";
    
    
    conversion_grayscale(imageName2);
    

    
    return 0;

}


