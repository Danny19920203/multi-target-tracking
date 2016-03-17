/**Storage for some useful help functions, define some useful structs**/
#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>


/*Computes the IOU distance between two bounding boxes
 the boundingbox form is[x1,y1,x2,y2]
*/
double IOU(cv::Vec4d& bb_test, cv::Vec4d& bb_gt); 

/*Transform a bounding box in a form[x1,y1,x2,y2] into form [x,y,s,r]
where the x,y is the center of the box, s is the scale/area and r is the aspect ratio
*/
cv::Vec4d ConvertBox2Z(cv::Vec4d& bbox);

/*Transform the box in a form[x,y,s,r] into form [x1,y1,x2,y2]
where x1,y1 is the top left and x2,y2 is the bottom right
*/
cv::Vec4d ConvertZ2Box(cv::Vec4d& Z);

cv::Rect ConvertY2Box(cv::Mat& Y);
#endif //UTILS_HPP

