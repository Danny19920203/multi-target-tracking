/**This file implements the kalman tracker with opencv
**/
#ifndef KALMANBOXTRACKER_HPP
#define KALMANBOXTRACKER_HPP

#include "utils.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class KalmanBoxTracker
{
public:
    KalmanBoxTracker(std::vector<float>&); //vector(x1,y1,x2,y2),note the state is(x1,y1,s,r,...) s=w*h r=w/h
    ~KalmanBoxTracker();
    std::vector<float> Predict(); //predict the box
    std::vector<float> Update(std::vector<float>&); //update the kalman filter
    std::vector<float> GetState();

    cv::KalmanFilter* KF;
    std::vector<float> lastBbox;
    std::vector<std::vector<float> > history;
    float score;
    int state_dim = 7;
    int measure_dim = 4;
    float time_since_update;
    int id; //specify the person id
    int hits;
    int hit_streak;
    int age;
};


#endif //KALMANBOXTRACKER_HPP

