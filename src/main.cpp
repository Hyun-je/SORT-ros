#include "ros/ros.h"
#include "SortRos.h"



int main (int argc, char** argv) {

	ros::init (argc, argv, "sort_ros");

    SortRos *sortRos = SortRos::GetInstance();
    sortRos->setup();

	ros::spin();

    return 0;
}