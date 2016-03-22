#include "utils.hpp"


float IOU(std::vector<float>& bb_test, std::vector<float>& bb_gt)
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

std::vector<float> ConvertBox2Z(std::vector<float>& bbox)
{
    std::vector<float> z_box(4);
    double w = bbox[2] - bbox[0];
    double h = bbox[3] - bbox[1];
    z_box[0] = (bbox[0] + bbox[2]) / 2;
    z_box[1] = (bbox[1] + bbox[3]) / 2;
    z_box[2] = w * h;
    z_box[3] = w / h; //it is the width/height

    return z_box;
}

std::vector<float> ConvertZ2Box(std::vector<float>& Z)
{
    std::vector<float> bbox(4);
    double w = std::sqrt(Z[2] * Z[3]);
    double h = Z[2] / w;
    bbox[0] = Z[0] - w / 2.0;
    bbox[1] = Z[1] - h / 2.0;
    bbox[2] = Z[0] + w / 2.0;
    bbox[3] = Z[1] + h / 2.0;
    return bbox;
}

cv::Mat vis_tracker(cv::Mat_<double> colours, cv::Mat& img, std::vector<data>& trackers)
{

    for (unsigned int i = 0; i < trackers.size(); i++)
    {
        std::cout<<"The trackers' index: "<<trackers[i].index<<std::endl;
        int selRows = trackers[i].index % colours.rows;

        std::vector<float> bbox = trackers[i].bbox;
        cv::rectangle(img, cv::Point(bbox[0], bbox[1]), cv::Point(bbox[2], bbox[3]), cv::Scalar(colours(selRows,0), colours(selRows,1), colours(selRows,2)), 2); //draw the bounding box
    }
    cv::Mat image = img.clone();
    return image;
}