

#include "cv_bridge/cv_bridge.h"
#include "opencv2/video/tracking.hpp"


using namespace cv;


class Tracker {

public:

	Tracker();


private:

	cv::Mat measurement;
    cv::KalmanFilter kf;

};