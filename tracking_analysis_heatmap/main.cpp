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

void generate_heatmap(string filename, double begin, double end);

void conversion_grayscale(string png_name);

IplImage* histogram(IplImage *image);

double getPSNR(const Mat& I1, const Mat& I2); //for the comparison

void create_histogram(string path_save, string image);

Mat compute_signatures(IplImage* hist1,IplImage* hist2, int h_bins = 30, int s_bins = 32);

void equalization(string source, string destination);


int main()
{
    /*
    string vide_name="134823_Feng";
    string coordinates="../Res/Video_" + vide_name + "/" + vide_name + "_coordinates.txt";
    cout<<"------generation heatmas----"<<endl;
    cout<<"close windows to continue" <<endl;
    
    
    generate_heatmap("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/134823_Feng_coordinates.txt", 5, 439);
    string imageName = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap.png";
    string imageName2 = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap9.png";
    
    
    conversion_grayscale(imageName2);
    
    create_histogram("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_histogramme.png", "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_heatmap1.png");
   
    equalization("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_heatmap1.png", "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/Equ_histogramme.png");
    
    return 0;
     */
    //read 2 images for histogram comparing
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

void create_histogram(string path_save, string image)
{
    
    Mat src, dst;
    
    /// Load image
    src = imread( image, 1 );
    
    /* /todo
     if( !src.data )
     { return -1; }
     */
    
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( src, bgr_planes );
    
    /// Establish the number of bins
    int histSize = 256;
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    Mat b_hist, g_hist, r_hist, gray_hist;
    
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
             Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
             Scalar( 0, 0, 255), 2, 8, 0  );
        
    }
    
    /// Display
    namedWindow("Histogramme", CV_WINDOW_AUTOSIZE );
    imshow("Histogramme", histImage );
    
    waitKey(0);
    
    //save histogram
    imwrite( path_save, histImage );
}


void equalization(string source, string destination)
{
    Mat src, dst;
    string source_window = "Source image";
    string equalized_window = "Equalized Image";
    
    /// Load image
    src = imread( source, 1 );
    
    cvtColor( src, src, CV_BGR2GRAY );
    
    equalizeHist( src, dst );
    
    
    /// Display results
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    namedWindow( equalized_window, CV_WINDOW_AUTOSIZE );
    
    imshow( source_window, src );
    imshow( equalized_window, dst );
    
    /// Wait until user exits the program
    waitKey(0);
    
    imwrite( destination, dst );
}

