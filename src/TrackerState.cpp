#include "TrackerState.h"


cv::Mat TrackerState::toMat(void) {

    cv::Mat mat = cv::Mat::zeros(MEASURE_NUM, 1, CV_32F);
    mat.at<float>(0, 0) = centerX;
    mat.at<float>(1, 0) = centerY;
    mat.at<float>(2, 0) = area;
    mat.at<float>(3, 0) = aspectRatio;

    return mat;
}

void TrackerState::fromMat(cv::Mat mat) {

    centerX     = mat.at<float>(0, 0);
    centerY     = mat.at<float>(1, 0);
    area        = mat.at<float>(2, 0);
    aspectRatio = mat.at<float>(3, 0);
    
}