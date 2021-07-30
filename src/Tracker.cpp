#include "Tracker.h"


int Tracker::kf_count = 0;


Tracker::Tracker(TrackerState state) {

    kf_count++;

    // Initialize variables
    m_time_since_update = 0;
    m_hits = 0;
    m_hit_streak = 0;
    m_id = kf_count;


    // Initialize kalman filter
    const int stateNum = 7;
    const int measureNum = 4;

    kf = cv::KalmanFilter(stateNum, measureNum, 0);

    kf.transitionMatrix = (cv::Mat_<float>(stateNum, stateNum) <<
        1, 0, 0, 0, 1, 0, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 0, 1, 0, 0, 0, 1,
        0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 1
    );

    cv::setIdentity( kf.measurementMatrix );
    cv::setIdentity( kf.processNoiseCov,        cv::Scalar::all(1e-2)   );
    cv::setIdentity( kf.measurementNoiseCov,    cv::Scalar::all(1e-1)   );
    cv::setIdentity( kf.errorCovPost,           cv::Scalar::all(1)      );

    // Initialize state vector
    kf.statePost.at<float>(0, 0) = state.centerX;
    kf.statePost.at<float>(1, 0) = state.centerY;
    kf.statePost.at<float>(2, 0) = state.area;
    kf.statePost.at<float>(3, 0) = state.aspectRatio;
    
}


TrackerState Tracker::predict(void) {

    if (m_time_since_update > 0)
        m_hit_streak = 0;
    m_time_since_update += 1;

    TrackerState state;
    state.fromMat(kf.predict());
    
    return state;

}


void Tracker::update(TrackerState state) {

    m_time_since_update = 0;
    m_hits += 1;
    m_hit_streak += 1;

    cv::Mat measurement = cv::Mat::zeros(4, 1, CV_32F);
    measurement.at<float>(0, 0) = state.centerX;
    measurement.at<float>(1, 0) = state.centerY;
    measurement.at<float>(2, 0) = state.area;
    measurement.at<float>(3, 0) = state.aspectRatio;

    kf.correct(measurement);

}


TrackerState Tracker::getState(void) {

    TrackerState state;
    state.fromMat(kf.statePost);

    return state;
}
