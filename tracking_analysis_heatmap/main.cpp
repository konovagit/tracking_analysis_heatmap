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


//Configurations
#define size_width 1280
#define size_height 720

#define area_width 11
#define area_height 11
#define area 5

// Size of heat map. Must be the same that the video.
static const size_t w = 1280, h = 720;

struct Pixel{
    int x;
    int y;
    long intensity;
};
//End configuration

//Mat new_matrix;
Mat new_matrix( h, w, CV_8UC1, Scalar(0) );

void generate_heatmap(string filename, double begin, double end);
void generate_heatmap_bis(Mat new_matrix);

long sum_pixel(Pixel pixel,Mat image);

Mat scrutation(Mat image);



int main()
{

    generate_heatmap("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/FeetLocations_bis.txt", 0, 2062);
    generate_heatmap_bis(new_matrix);
    
    //Superposition//
    Mat dst;
    Mat src1 = imread("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap_new_matrix.png");
    Mat src2 = imread("/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/background_garden.png");
    addWeighted( src1, 0.5, src2, 0.5, 0.0, dst);
    imwrite( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/garden_heatmap_original_result.png", dst );


    return 0;
}


void generate_heatmap(string filename, double begin, double end)
{

    // To write on specific files for each couple (begin,end).
    string b = to_string((int)begin), e = to_string((int)end);
    
    // Path of the heatmap and the video's background
    string nom_video = "134823_Feng";
    string heatmap_img = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap_garden.png"; // where the heatmap will be saved
    string background_img = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/background_garden.png";
    
    // Create the heatmap object with the given dimensions (in pixel).
    heatmap_t* hm = heatmap_new(w, h);
    

    // Create a matrix (rows, cols, type, initial value)
    Mat matrix( h, w, CV_8UC1, Scalar(0) );
    
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
                           //heatmap_add_point(hm, x, y);
                           matrix.at<uchar>(y,x)+=1;  //add 1 each time pixel is activated
                         }
                    }
                    i++;
                }
                imwrite( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/TEST.png", matrix );
                
                new_matrix=scrutation(matrix);
                imwrite( "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/new_matrix.png", matrix );
                // This creates an image out of the heatmap.
                // `image` now contains the image data in 32-bit RGBA.
                vector<unsigned char> image(w*h * 4);
                heatmap_render_default_to(hm, &image[0]);
                //heatmap_render_to(hm, heatmap_cs_b2w, &image[0]);
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

Mat scrutation(Mat image)
{
    /*Variables*/
    Pixel pixel;
    Mat buff_image( h, w, CV_8UC1, Scalar(0) );


    /*Image Model LOOP*/
    for (int i=0; i<(image.rows); i++)  //Rows
    {
        for (int j=0; j<(image.cols); j++) //Cols
        {
            pixel.intensity=image.at<uchar>(i,j);
            pixel.x=i;
            pixel.y=j;
            buff_image.at<uchar>(i,j)=sum_pixel(pixel, image); //set pixel to sum of pixels (11x11 window)
        }
    }
    return buff_image; //return the new_matrix to create the heatmap
}

long sum_pixel(Pixel pixel,Mat image)
{
    /*Variables*/
    int row=0, col=0;
    long value=0;
    
    /*Area LOOP*/
    for ( int i=(pixel.x-area); row<(area_height); i++ )  //Rows
    {
        for ( int j=(pixel.y-area); col<(area_width); j++ ) //Cols
        {
            if ((i<0) || (j<0) || (i>(size_height-1)) || (j>(size_width-1)))
            {
                //nothing out of the image
            }
            else {value+=image.at<uchar>(i,j);}   //add value of curent pixel
            
            col++;
        }
        row++;
        col=0;
    }
    
    return value;
}

void generate_heatmap_bis(Mat new_matrix)
{
    // Create the heatmap object with the given dimensions (in pixel).
    heatmap_t* hm = heatmap_new(w, h);
    
    string heatmap_img = "/Users/konova/tracking_analysis_heatmap/Res/Video_134823_Feng/heatmap_new_matrix.png"; // where the heatmap will be saved
    
    int time=0, inc=0;
    
    /*Image Model LOOP*/
    for (int i=0; i<(size_height-1); i++)  //Rows
    {
        for (int j=0; j<(size_width-1); j++) //Cols
        {
                time=new_matrix.at<uchar>(i,j);   //save number of time activated
                inc=0;
               while(inc!=time)
               {
                    heatmap_add_point(hm, j, i);     //draw
                    inc++;
               }
        }
    }
    // This creates an image out of the heatmap.
    // `image` now contains the image data in 32-bit RGBA.
    vector<unsigned char> image(w*h * 4);
    heatmap_render_default_to(hm, &image[0]);  //color mode
    //heatmap_render_to(hm, heatmap_cs_b2w, &image[0]); //grayscale mode
    // Now that we've got a finished heatmap picture, we don't need the map anymore.
    heatmap_free(hm);
    
    // Finally, we use the lodepng library to save it as an image.
    if (unsigned error = lodepng::encode(heatmap_img, image, w, h))
    {
        cerr << "encoder error " << error << ": " << lodepng_error_text(error) << endl;
    }
}


