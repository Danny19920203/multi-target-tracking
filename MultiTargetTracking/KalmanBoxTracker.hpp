/**This file implements the kalman tracker with opencv
**/
#ifndef KALMANBOXTRACKER_HPP
#define KALMANBOXTRACKER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class KalmanBoxTracker
{
    public:
        KalmanBoxTracker(cv::Rect&); //Rect(x1,y1,w,h),note the state is(x1,y1,s,r,...) s=w*h r=w/h
        ~KalmanBoxTracker();
        cv::Rect Prediction(); //predict the box
        cv::Rect Update(cv::Rect&); //update the kalman filter
        cv::Rect GetState();
        
        cv::KalmanFilter* KF;
        cv::Rect lastBbox;
        std::vector<cv::Rect> history;
        int state_dim = 7;
        int measure_dim = 4;
        float time_since_update;
        int id; //specify the person id
        int count = 0;
        int hits;
        int hit_streak;
        int age;

};


#endif //KALMANBOXTRACKER_HPP

