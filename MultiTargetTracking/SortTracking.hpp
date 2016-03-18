/**This file implements the main tracking algorithm, sort tracking
Please find the paper: Simple online and realtime tracking
**/
#ifndef SORTTRACKING_HPP
#define SORTTRACKING_HPP

#include "utils.hpp"
#include "KalmanBoxTracker.hpp"
#include "HungarianAlign.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>

class Sort
{
public:
    Sort(int age = 1, int hits = 3);
    ~Sort();
    data Update(data dets); //data: struct, bbox,score,index

    int max_age;
    int min_hits;
    int frame_count;
    std::vector<KalmanBoxTracker> trackers;

};

#endif /*SORTTRACKING_HPP*/