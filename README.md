# SORT-ros
Implementation of ***SORT(Simple Online and Realtime Tracking)*** algorithm for ROS Melodic. It can be used track objects in realtime from image or pointcloud detection. To maximize computational speed, all of soruce codes is written fully c++ language.

## Installation
1. Install OpenCV for ROS Melodic
```
apt-get install ros-melodic-opencv*
```
2. Clone source code
```
cd ~/catkin_ws/src
git clone https://github.com/Hyun-je/SORT-ros/
```
3. Build package
```
cd ~/catkin_ws
catkin_make
```

## References
- https://github.com/abewley/sort **abewley/sort**
- https://github.com/mcximing/sort-cpp **mcximing/sort-cpp**
