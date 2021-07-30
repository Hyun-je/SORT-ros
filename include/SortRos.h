#ifndef SORT_ROS_H
#define SORT_ROS_H

#include "ros/ros.h"
#include "sort_ros/Rect.h"
#include "sort_ros/RectArray.h"

#include "Sort.h"


class SortRos {

private:

    static SortRos* instance;

    SortRos(void) {};
    SortRos(const SortRos& other);
    ~SortRos() {};

public:

    static SortRos* GetInstance() {
        if(instance == NULL) instance = new SortRos();
        return instance;
    }



private:

    ros::NodeHandle nh;
    static ros::Publisher pub;
    static ros::Subscriber sub;

    static void rectArrayCallback(const sort_ros::RectArray::ConstPtr& rectArrayMsg);



private:

    static Sort *s;

public:

    void setup(void);

};


#endif