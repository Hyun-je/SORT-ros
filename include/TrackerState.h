#ifndef TRACKER_STATE_H
#define TRACKER_STATE_H


#include "cv_bridge/cv_bridge.h"


struct TrackerState {

	float centerX;
	float centerY;
	float area;
	float aspectRatio;

	cv::Mat toMat(void);
	void fromMat(cv::Mat mat);

};


#endif