#include "SortRos.h"


SortRos* SortRos::instance = nullptr;

ros::Publisher SortRos::pub;
ros::Subscriber SortRos::sub;

Sort *SortRos::s;


void SortRos::setup(void) {

    double maxAge = 2;
    double minHits = 3;
    double iouThreshold = 0.3;

    ros::param::get("/sort_ros/max_age", maxAge);
    ros::param::get("/sort_ros/min_hits", minHits);
    ros::param::get("/sort_ros/iou_threshold", iouThreshold);

    SortRos::s = new Sort(maxAge, minHits, iouThreshold);

	SortRos::sub = nh.subscribe<sort_ros::RectArray> ("/rect_detected", 1, SortRos::rectArrayCallback);
    SortRos::pub = nh.advertise<sort_ros::RectArray> ("/rect_tracked", 1);

}


void SortRos::rectArrayCallback (const sort_ros::RectArray::ConstPtr& rectArrayMsg) {

    std::vector<SortRect> rects;

    for(auto rectMsg : rectArrayMsg->rects) {

        SortRect rect;
        rect.id = 0;
        rect.centerX    = rectMsg.centerX;
        rect.centerY    = rectMsg.centerY;
        rect.width      = rectMsg.width;
        rect.height     = rectMsg.height;

        rects.push_back(rect);

    }

    std::vector<SortRect> output = SortRos::s->update(rects);

    sort_ros::RectArray rectArrayMsgOutput;

    for(auto rect : rects) {

        sort_ros::Rect rectMsg;
        rectMsg.centerX     = rect.centerX;
        rectMsg.centerY     = rect.centerY;
        rectMsg.width       = rect.width;
        rectMsg.height      = rect.height;

        rectArrayMsgOutput.rects.push_back(rectMsg);
    }

    SortRos::pub.publish(rectArrayMsg);

}
