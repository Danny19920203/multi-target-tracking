/**Storage for some useful help functions, define some useful structs**/
#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <cmath>
#include <iostream>
#include <vector>

struct data 
{
    std::vector<float> bbox;
    float score;
    int index;
};

struct get_matched
{
    std::vector<std::pair<int, int> > matched;
    std::vector<int> unmatched_dets;
    std::vector<int> unmatched_trks;
};

/*Computes the IOU distance between two bounding boxes
 the boundingbox form is[x1,y1,x2,y2]
*/
float IOU(std::vector<float>& bb_test, std::vector<float>& bb_gt); 

/*Transform a bounding box in a form[x1,y1,x2,y2] into form [x,y,s,r]
where the x,y is the center of the box, s is the scale/area and r is the aspect ratio
*/
std::vector<float>& ConvertBox2Z(std::vector<float>& bbox);

/*Transform the box in a form[x,y,s,r] into form [x1,y1,x2,y2]
where x1,y1 is the top left and x2,y2 is the bottom right
*/
std::vector<float>& ConvertZ2Box(std::vector<float>& Z);

/*Visualize the tracking result*/
cv::Mat vis_tracker(cv::Mat& img, std::vector<data>& trackers);

#endif //UTILS_HPP

