/**This project aims for real-time pedestrain detection and tracking, also the result must be stable
**/

#include <stdio.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "SortTracking.hpp"
#include "testHungarian.hpp"
#include "faster_rcnn_detection.h"
#include "utils.hpp"

cv::Mat image;

int main(int argc, char *argv[])
{
  int nrows = 5;
  int ncols = 7;
  if (argc == 3)
  {
    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
  }
  //test the hungarian algorithm
  testHungarian(nrows, ncols);

  //test the video file, tracking by detection
  std::string prototxt = "/home/samlong/Documents/py-faster-rcnn/models/ZF/faster_rcnn_end2end/test2-C.prototxt";
  std::string caffemodel = "/home/samlong/Documents/py-faster-rcnn/output/SN-perser/trainval/zf_faster_rcnn_iter_70000.caffemodel";
  std::string video_file = "/home/samlong/Videos/video1.avi";

  Sort mot_tracker; //tracker
  cv::VideoCapture cap;
  // cap.open(0);
  cap.open(video_file);
  if (!cap.isOpened())
  {
    std::cout << "It is not captured!" << std::endl;
    return 0;
  }
  Faster_Rcnn_Detection handle;
  handle.setNet(prototxt, caffemodel, "new-finetune");

  cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);

  int counter = 1;
  while (cap.read(image))
  {
    //detect first, then tracking
    if (counter % 2 == 0)
    {
      cv::Mat img1 = handle.ImDetect(image);
      std::vector<data> box_data = handle.getBoxData();
      for(unsigned int i = 0;i<box_data.size();i++)
      {
          std::vector<float> bbox = box_data[i].bbox;
          for(unsigned int j = 0; j<bbox.size();j++)
            std::cout<<bbox[j]<<" ";
          std::cout<<std::endl;

      }

      std::vector<data> trackers = mot_tracker.Update(handle.getBoxData());
      std::cout<<"The size of trackers: "<<trackers.size()<<std::endl;
      cv::Mat img = vis_tracker(image, trackers);
      cv::imshow("Video", img);
      if (cv::waitKey(10) == 'q')
        break;
    }


    counter++;
  }

  return 0;

}

