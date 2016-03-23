#include "KalmanBoxTracker.hpp"

KalmanBoxTracker::KalmanBoxTracker(std::vector<float>& bbox)
{
    /*we use 7-dim vector as the state, 4-dim vector as the measure vector, no control vector*/

    KF = new cv::KalmanFilter(state_dim, measure_dim, 0);
    //transition matrix
    KF->transitionMatrix = (cv::Mat_<float>(7, 7) << 1, 0, 0, 0, 1, 0, 0,  0, 1, 0, 0, 0, 1, 0,  0, 0, 1, 0, 0, 0, 1,
                            0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0,  0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 0, 0, 1); //Matrix F
    //TODO: implement with the var--state_dim and measure_dim
    KF->measurementMatrix = cv::Mat::eye(measure_dim, state_dim, CV_32F); //Matrix H


    KF->measurementNoiseCov = cv::Mat::eye(measure_dim, measure_dim, CV_32F); //R
    cv::Mat diagVec1 = KF->measurementNoiseCov.diag(0);
    for (int i = 2; i < measure_dim; i++)
    {
        diagVec1.at<float>(i) = 10;
    }

    KF->errorCovPost = cv::Mat::eye(state_dim, state_dim, CV_32F); //P
    cv::Mat diagVec2 = KF->errorCovPost.diag(0);
    for (int i = 0; i < state_dim; i++)
    {
        if (i <= 2)
            diagVec2.at<float>(i) = 10;
        else
            diagVec2.at<float>(i) = 1000;
    }

    KF->processNoiseCov = cv::Mat::eye(state_dim, state_dim, CV_32F); //Q
    cv::Mat diagVec3 = KF->processNoiseCov.diag(0);
    for (int i = 4; i < state_dim; i++)
    {
        if (i <= 5)
            diagVec3.at<float>(i) = 0.01;
        else
            diagVec2.at<float>(i) = 0.0001;
    }

    std::vector<float> Z =  ConvertBox2Z(bbox);

    KF->statePre.at<float>(0) = Z[0];
    KF->statePre.at<float>(1) = Z[1];
    KF->statePre.at<float>(2) = Z[2]; //scale = w*h
    KF->statePre.at<float>(3) = Z[3]; //retiao
    KF->statePre.at<float>(4) = 0;
    KF->statePre.at<float>(5) = 0; //scale = w*h
    KF->statePre.at<float>(6) = 0; //retiao

    KF->statePost.at<float>(0) = Z[0];
    KF->statePost.at<float>(1) = Z[1];
    KF->statePost.at<float>(2) = Z[2]; //scale = w*h
    KF->statePost.at<float>(3) = Z[3]; //retiao
    KF->statePost.at<float>(4) = 0;
    KF->statePost.at<float>(5) = 0; //scale = w*h
    KF->statePost.at<float>(6) = 0; //retiao


    lastBbox = bbox;
    history.clear();
    time_since_update = 0;
    id = 0;
    hits = 0;
    hit_streak = 0;
    age = 0;
    score = 0.0;

}

KalmanBoxTracker::~KalmanBoxTracker()
{
    delete KF;
}

std::vector<float> KalmanBoxTracker::Predict()
{

    if (KF->statePost.at<float>(2) + KF->statePost.at<float>(6) <= 0)
        KF->statePost.at<float>(6) *= 0.0;

    cv::Mat prediction = KF->predict();

    age += 1;
    if (time_since_update > 0)
        hit_streak = 0;
    time_since_update += 1;

    std::vector<float> pred_Z(prediction.rows);
    for (int i = 0; i < prediction.rows; i++)
    {
        pred_Z[i] = prediction.at<float>(i);
    }
    lastBbox = ConvertZ2Box(pred_Z);
    history.push_back(lastBbox);

    return lastBbox;
}

std::vector<float> KalmanBoxTracker::Update(std::vector<float>& newBox)
{   //update the box state with the detection box
    time_since_update = 0;
    history.clear();
    hits += 1;
    hit_streak += 1;

    cv::Mat measurement(4, 1, CV_32FC1);

    std::vector<float> Z =  ConvertBox2Z(newBox);
    measurement.at<float>(0) = Z[0];
    measurement.at<float>(1) = Z[1];
    measurement.at<float>(2) = Z[2];
    measurement.at<float>(3) = Z[3];

    cv::Mat estimated = KF->correct(measurement);
    std::vector<float> est_Z(estimated.rows);
    for (int i = 0; i < estimated.rows; i++)
    {
        est_Z[i] = estimated.at<float>(i);
    }
    lastBbox = ConvertZ2Box(est_Z);
    return lastBbox;
}



std::vector<float> KalmanBoxTracker::GetState()
{
    return lastBbox;
}




