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
    string vide_name="134823_Feng";
    string coordinates="../Res/Video_" + vide_name + "/" + vide_name + "_coordinates.txt";
    cout<<"------generation heatmas----"<<endl;
    cout<<"close windows to continue" <<endl;
    
    
    generate_heatmap("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/134823_Feng_coordinates.txt", 5, 439);
    string imageName = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap.png";
    string imageName2 = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap9.png";
    
    
    conversion_grayscale(imageName2);
    
    ////Comparison///
    double psnrV;
    Scalar mssimV;
    string reference="/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap1.png";
    string compar="/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap2.png";
    
    Mat Reference= imread(reference, CV_LOAD_IMAGE_GRAYSCALE);
    Mat Compar= imread(compar, CV_LOAD_IMAGE_GRAYSCALE);
    
    psnrV = getPSNR(Reference,Compar);
    cout<<"Resultat comparison:"<<psnrV<<"%"<<endl;

    IplImage *hist, *image;
    image = cvLoadImage("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap2.png");
    hist=histogram(image);
    
    char key;
    
    cvShowImage("difference between Two heatmap", hist);
    
    // While we don't want to quit
    while(key != 'Q' && key != 'q') {
        
        key = cvWaitKey(10);
        
    }
    // Destroy the window we have created
    cvDestroyWindow("difference between Two heatmap");
    
    
    
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



void generate_heatmap(string filename, double begin, double end)
{
    // To write on specific files for each couple (begin,end).
    string b = to_string((int)begin), e = to_string((int)end);
    
    // Path of the heatmap and the video's background
    string nom_video = "134823_Feng";
    string heatmap_img = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap.png"; // where the heatmap will be saved
    string background_img = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/134823_Feng_background.png";
    
    // Size of heat map. Must be the same that the video.
    static const size_t w = 720, h = 720;
    
    // Create the heatmap object with the given dimensions (in pixel).
    heatmap_t* hm = heatmap_new(w, h);
    
    //Mat result;
    string line, pos_x, pos_y, time_msec, fps, nb_frames;
    float ftime_msec, first_time, last_time;
    size_t pos_tmp, pos_tmp2, pos_tmp3;
    unsigned int x = 0, y = 0;
    int i = 0;
    
    // Get the first time and last time of the txt positions file (x,y,t).
    ifstream input(filename); // Path of the txt file
    while (getline(input, line))
    {
        if (i == 0)// Get the first time of the txt file
        {
            pos_tmp = line.find(","); // ist comma
            pos_tmp2 = line.find(",", pos_tmp + 1); // 2nd comma
            pos_tmp3 = line.find(")");
            time_msec = line.substr(pos_tmp2 + 1, ((pos_tmp3 - 1) - (pos_tmp2)));
            ftime_msec = (stof(time_msec));
            first_time = ftime_msec;
        }
        i++;
    }
    // Get the last time
    pos_tmp = line.find(","); // 1st comma
    pos_tmp2 = line.find(",", pos_tmp + 1); // 2nd comma
    pos_tmp3 = line.find(")");
    time_msec = line.substr(pos_tmp2 + 1, ((pos_tmp3 - 1) - (pos_tmp2)));
    ftime_msec = (stof(time_msec));
    last_time = ftime_msec;
    cout << "First time : " << first_time << ", " << "Last time : "<<last_time << endl;
    input.clear();
    input.seekg(input.beg); // Go back to the begining of the file
    
    //Add points from the txt file to the heatmap
    if (begin > last_time || begin < first_time)
    {
        cout << "Incorrect begin time (" << begin << ") : first time = " << first_time << ", last time = " << last_time << endl;
    }
    else
    {
        if (end > last_time || end < first_time)
        {
            cout << "Incorrect end time (" << end << ") : first time = " << first_time << ", last time = " << last_time << endl;
        }
        else
        {
            if (end<begin)
            {
                cout << "End time anterior at begin time" << endl;
            }
            else
            {
                while (getline(input, line))
                {
                    if (i>5)// 5 first points are not interesting -> never displayed on the heatmap
                    {
                        // Get x,y and time
                        pos_tmp = line.find(","); // 1st coma
                        pos_x = line.substr(1, pos_tmp - 1);
                        pos_tmp2 = line.find(",", pos_tmp + 1); // 2nd coma
                        pos_y = line.substr(pos_tmp + 1, ((pos_tmp2 - 1) - (pos_tmp)));
                        pos_tmp3 = line.find(")");
                        x = atoi(pos_x.c_str());
                        y = atoi(pos_y.c_str());
                        time_msec = line.substr(pos_tmp2 + 1, ((pos_tmp3 - 1) - (pos_tmp2)));
                        ftime_msec = (stof(time_msec));
                        
                        if (ftime_msec >= begin && ftime_msec <= end)
                        {
                            heatmap_add_point(hm, x, y);
                        }
                    }
                    i++;
                }
                // This creates an image out of the heatmap.
                // `image` now contains the image data in 32-bit RGBA.
                vector<unsigned char> image(w*h * 4);
                //heatmap_render_default_to(hm, &image[0]);
                heatmap_render_to(hm, heatmap_cs_b2w, &image[0]);
                // Now that we've got a finished heatmap picture, we don't need the map anymore.
                heatmap_free(hm);
                
                
                // Finally, we use the lodepng library to save it as an image.
                if (unsigned error = lodepng::encode(heatmap_img, image, w, h)) {
                    cerr << "encoder error " << error << ": " << lodepng_error_text(error) << endl;
                }
            }
        }
    }
}


IplImage* histogram(IplImage *image)
{
    //Function which takes an image as parameter and returns the image containing the gray-scale intensity histogram
    
    IplImage* imgHistogram; IplImage* gray; //Variables for the histogram's image and the gray-scale intermediate
    CvHistogram* hist; //To store the histogram variable in the proper format
    
    //The next few lines of declarations are all important variables for executing the OpenCV histogram functions
    
    int bins = 256;//Size of the histogram
    int hsize[] = {bins}; //One-dimensional histogram, so one value in array
    
    float max_value = 0, min_value = 0; //Max and min value of the histogram
    
    int value; //Value of frequency;
    int normalized, scale = 200; //Variables for storing the normalized value, and the scaled maximum (200)
    
    float xranges[] = {0,255};//Ranges of the values
    float* ranges[] = {xranges};//One-dimensional image, so only one value in array
    
    gray = cvCreateImage(cvGetSize(image), 8, 1);
    cvCvtColor(image, gray, CV_BGR2GRAY);//Grayscale equivalent created
    
    IplImage* planes[] = {gray};//Planes to obtain the histogram, in this case just one
    
    hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges,1);//Creates the histogram with the necessary parameters
    cvCalcHist(planes, hist, 0, NULL); //Calculates the histogram with the parameters
    cvGetMinMaxHistValue(hist, &min_value, &max_value); //Reports the minimal and maximal values in histogram
    
    imgHistogram = cvCreateImage(cvSize(bins,scale),8,3); //Creates the image of the histogram of width 'bins' (256) and height 'scale' (200)
    CvScalar CV_BGR(255,255,255), CV_BGR1(0,0,0);
    
    
    cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(bins-1,scale-1), CV_BGR,-1); //Draws a white rectangle from (0,0) to (256,200)
    
    //Draws the vertically flipped histogram
    
    for(int i=0; i < bins; i++)
   	{
        value = cvGetReal1D(hist->bins, i); //Obtains the frequency for the intensity i
        normalized = cvRound(value*scale/max_value); //Scales it down
        cvLine(imgHistogram,cvPoint(i,scale), cvPoint(i,scale-normalized), CV_BGR1); //Draws a line in the vertical direction of that height from point (i,200) to (i,200-normalized)
    }
    
    cvFlip(image, NULL, 1);// flips the image upside down
    
    cvReleaseImage(&gray);
    return imgHistogram; //Returns the image
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



