/**This project aims for real-time pedestrain detection and tracking, also the result must be stable
**/

#include <stdio.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "SortTracking.hpp"
#include "utils.hpp"

cv::Mat image;

int main(int argc, char *argv[])
{
   std::string video_file = "/home/samlong/Videos/video1.avi";
   cv::VideoCapture cap(video_file);
   if(!cap.isOpened())
       std::cout<<"It is not captured!"<<std::endl;

    double total_time = 0.0;
    int total_frames = 0;

   while(cap.read(image))
   {
    /*detect first, then tracking*/


    cv::imshow("test_video", image);
    if(cv::waitKey(30)=='q')
        return 0;
   }


return 0;

}

