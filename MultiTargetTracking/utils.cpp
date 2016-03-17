#include "utils.hpp"


double IOU(cv::Vec4d& bb_test, cv::Vec4d& bb_gt)
{
    double xx1 = std::max(bb_test[0], bb_gt[0]);
    double yy1 = std::max(bb_test[1], bb_gt[1]);
    double xx2 = std::min(bb_test[2], bb_gt[2]);
    double yy2 = std::min(bb_test[3], bb_gt[3]);

    double iou_w = std::max(0.0, xx2 - xx1);
    double iou_h = std::max(0.0, yy2 - yy1);

    double region_area = iou_w * iou_h;
    return region_area / ((bb_test[2] - bb_test[0]) * (bb_test[3] - bb_test[1]) + 
        (bb_gt[2] - bb_gt[0]) * (bb_gt[3] - bb_gt[1]) - region_area);

}

cv::Vec4d ConvertBox2Z(cv::Vec4d& bbox)
{
    cv::Vec4d z_box;
    double w = bbox[2] - bbox[0];
    double h = bbox[3] - bbox[1];
    z_box[0] = (bbox[0] + bbox[2]) / 2;
    z_box[1] = (bbox[1] + bbox[3]) / 2;
    z_box[2] = w*h;
    z_box[3] = w/h; //it is the width/height

    return z_box;
}

cv::Vec4d ConvertZ2Box(cv::Vec4d& Z)
{
    cv::Vec4d bbox;
    double w = std::sqrt(Z[2] * Z[3]);
    double h = Z[2] / w;
    bbox[0] = Z[0] - w/2.0;
    bbox[1] = Z[1] - h/2.0;
    bbox[2] = Z[0] + w/2.0;
    bbox[3] = Z[1] + h/2.0;
    return bbox;
}