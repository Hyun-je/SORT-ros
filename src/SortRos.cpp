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

	SortRos::sub = nh.subscribe<visualization_msgs::MarkerArray> ("/markers_detected", 1, SortRos::rectArrayCallback);
    SortRos::pub = nh.advertise<visualization_msgs::MarkerArray> ("/markers_tracked", 1);

}


void SortRos::rectArrayCallback (const visualization_msgs::MarkerArray::ConstPtr& markerArray) {

    std::string frame_id;
    std::vector<SortRect> rects;

    for(auto marker : markerArray->markers) {

        frame_id = marker.header.frame_id;

        SortRect rect;
        rect.id = 0;
        rect.centerX    = marker.pose.position.x;
        rect.centerY    = marker.pose.position.y;
        rect.width      = marker.scale.x;
        rect.height     = marker.scale.y;

        rects.push_back(rect);

    }

    std::vector<SortRect> output = SortRos::s->update(rects);


    visualization_msgs::MarkerArray markerArrayOutput;

    for(auto rect : output) {

        visualization_msgs::Marker marker;

        marker.header.stamp = ros::Time::now();
        marker.header.frame_id = frame_id;

        marker.frame_locked = true;
        marker.lifetime = ros::Duration(0.5);
        marker.ns = "bounding_box";
        marker.id = rect.id;
        marker.action = visualization_msgs::Marker::ADD;
        marker.type = visualization_msgs::Marker::CUBE;
        
        marker.pose.position.x = rect.centerX;
        marker.pose.position.y = rect.centerY;
        marker.pose.position.z = 1.0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        
        marker.scale.x = rect.width;
        marker.scale.y = rect.height;
        marker.scale.z = 1.0;
        marker.color.a = 0.3;
        marker.color.r = 1.0;
        marker.color.g = 1.0;
        marker.color.b = 1.0;

        markerArrayOutput.markers.push_back(marker);
    }

    SortRos::pub.publish(markerArrayOutput);               

}
