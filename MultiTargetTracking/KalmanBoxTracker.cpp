#include "KalmanBoxTracker.hpp"

KalmanBoxTracker::KalmanBoxTracker(cv::Rect& bbox)
{
    /*we use 7-dim vector as the state, 4-dim vector as the measure vector, no control vector*/

    KF = new KalmanFilter(state_dim, measure_dim, 0);
    //transition matrix
    KF->transitionMatrix = (cv::Mat_<float>(7,7) <<1,0,0,0,1,0,0,  0,1,0,0,0,1,0,  0,0,1,0,0,0,1,  
                                                    0,0,0,1,0,0,0,  0,0,0,0,1,0,0,  0,0,0,0,0,1,0,  0,0,0,0,0,0,1); //Matrix F
    //TODO: implement with the var--state_dim and measure_dim
    KF->measurementMatrix = cv::Mat::eye(state_dim,measure_dim,CV_32F);  //Matrix H
    cv::setIdentity(KF->measurementNoiseCov,cv::Scalar(1,1,10,10,10,10,10));
    cv::setIdentity(KF->errorCovPost, cv::Scalar(10,10,10,10,1000,1000,1000)); //P
    cv::setIdentity(KF->processNoiseCov, cv::Scalar(1,1,1,1,0.01,0.01,0.0001)); //Q
    
    KF->statePre.at<float>(0) = bbox.x;
    KF->statePre.at<float>(1) = bbox.y;
    KF->statePre.at<float>(2) = bbox.width * bbox.height;//scale = w*h
    KF->statePre.at<float>(3) = bbox.width/bbox.height; //retiao
    KF->statePre.at<float>(4) = 0;
    KF->statePre.at<float>(5) = 0; //scale = w*h
    KF->statePre.at<float>(6) = 0; //retiao

    KF->statePost.at<float>(0) = bbox.x;
    KF->statePost.at<float>(1) = bbox.y;
    KF->statePost.at<float>(2) = bbox.width * bbox.height;//scale = w*h
    KF->statePost.at<float>(3) = bbox.width/bbox.height; //retiao

    lastBbox = bbox;
    history.clear();
    time_since_update = 0;
    id = count;
    count +=1;
    hits = 0;
    hit_streak = 0;
    age = 0;

}

KalmanBoxTracker::~KalmanBoxTracker()
{
    delete KF;
}

cv::Rect KalmanBoxTracker::Update(cv::Rect& newBox)
{//update the box state with the new box
    time_since_update =0;
    history.clear();
    hits += 1;
    hit_streak += 1;

    cv::Mat measurement(4, 1, CV_32FC1);
    measurement.at<float>(0) = newBox.x;
    measurement.at<float>(1) = newBox.y;
    measurement.at<float>(2) = newBox.width * newBox.height;
    measurement.at<float>(3) = newBox.width/newBox.height;

    cv::Mat estimated = KF->correct(measurement);
    lastBbox.x = estimated.at<float>(0);
    lastBbox.y = estimated.at<float>(1);
    lastBbox.width = std::sqrt(estimated.at<float>(2) * estimated.at<float>(3));
    lastBbox.height = estimated.at<float>(2) / lastBbox.width;
    return lastBbox;
}

cv::Rect KalmanBoxTracker::Prediction()
{
    cv::Mat prediction = KF->predict();
    lastBbox.x = prediction.at<float>(0);
    lastBbox.y = prediction.at<float>(1);
    lastBbox.width = std::sqrt(prediction.at<float>(2) * prediction.at<float>(3));
    lastBbox.height = prediction.at<float>(2) / lastBbox.width;
    return lastBbox;
}

cv::Rect KalmanBoxTracker::GetState()
{
    return lastBbox;
}




