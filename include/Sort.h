#ifndef SORT_H
#define SORT_H

#include "Tracker.h"
#include "Hungarian.h"
#include "SortRect.h"

#include <vector>
#include <set>



class Sort {

    public:

        Sort(int maxAge, int minHits, float iouThreshold);

        std::vector<SortRect> update(std::vector<SortRect> rects);


    private:

        HungarianAlgorithm HungAlgo;

        int maxAge;
        int minHits;
        float iouThreshold;

        std::vector<Tracker> trackers;

        float iou(SortRect rect1, SortRect rect2);

};


#endif