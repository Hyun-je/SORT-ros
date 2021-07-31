#include "Sort.h"
#include <cfloat>


Sort::Sort(int maxAge = 2, int minHits = 3, float iouThreshold = 0.3) {

    this->maxAge = maxAge;
    this->minHits = minHits;
    this->iouThreshold = iouThreshold;

}


std::vector<SortRect> Sort::update(std::vector<SortRect> detections) {

    if(trackers.size() == 0) {
        
        for (int i=0; i<detections.size(); i++) {

            TrackerState state = detections[i].toTrackerState();
            Tracker tracker = Tracker(state);

            trackers.push_back(tracker);
        }

        return std::vector<SortRect>();
    }



    std::vector<SortRect> predictions;
    for(int i=0; i<trackers.size(); i++) {
        TrackerState state = trackers[i].predict();

        SortRect rect;
        rect.fromTrackerState(state);
        rect.id = 0;

        predictions.push_back(rect);
    }



    vector<vector<double>> iouMatrix;
	iouMatrix.resize(predictions.size(), vector<double>(detections.size(), 0));

    for(int i=0; i<predictions.size(); i++)
        for(int j=0; j<detections.size(); j++)
            iouMatrix[i][j] = 1 - iou(predictions[i], detections[j]);



    vector<int> assignment;

    HungAlgo.Solve(iouMatrix, assignment);



	set<int> allItems;
	set<int> matchedItems;
    set<int> unmatchedDetections;
    
    for (int i=0; i<detections.size(); i++)
        allItems.insert(i);

    for (int i=0; i<predictions.size(); i++)
        if (assignment[i] != -1)
            matchedItems.insert(assignment[i]);

    std::set_difference(
        allItems.begin(), allItems.end(),
        matchedItems.begin(), matchedItems.end(),
        insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin())
    );



    vector<std::pair<int, int>> matchedPairs;
    for(int i=0; i<assignment.size(); i++) {

        if(assignment[i] == -1) continue;

        if(1 - iouMatrix[i][assignment[i]] < iouThreshold)
            unmatchedDetections.insert(assignment[i]);
        else
            matchedPairs.push_back(make_pair(i, assignment[i]));
    }
    


    for(auto pair : matchedPairs) {

        int trackerIndex = pair.first;
        int detectionIndex = pair.second;

        TrackerState state = detections[detectionIndex].toTrackerState();

        trackers[trackerIndex].update(state);
    }


    
    for (auto detectionIndex : unmatchedDetections) {

        TrackerState state = detections[detectionIndex].toTrackerState();

        Tracker tracker = Tracker(state);
        trackers.push_back(tracker);
    }

    
    
    std::vector<SortRect> result;
    for (auto it = trackers.begin(); it != trackers.end();) {

        if((*it).m_time_since_update > maxAge) {

            it = trackers.erase(it);

        }
        else {

            if( (*it).m_time_since_update < 1 && (*it).m_hit_streak >= minHits ) {

                TrackerState state = (*it).getState();

                SortRect rect;
                rect.fromTrackerState(state);
                rect.id = (*it).m_id;

                result.push_back(rect);

                
            }

            it++;
        }

    }

    return result;

}


float Sort::iou(SortRect rect1, SortRect rect2) {

    float rect1_x1 = rect1.centerX - rect1.width/2;
    float rect1_y1 = rect1.centerY - rect1.height/2;
    float rect1_x2 = rect1.centerX + rect1.width/2;
    float rect1_y2 = rect1.centerY + rect1.height/2;

    float rect2_x1 = rect2.centerX - rect2.width/2;
    float rect2_y1 = rect2.centerY - rect2.height/2;
    float rect2_x2 = rect2.centerX + rect2.width/2;
    float rect2_y2 = rect2.centerY + rect2.height/2;


    float x1 = max(rect1_x1, rect2_x1);
    float y1 = max(rect1_y1, rect2_y1);
    float x2 = min(rect1_x2, rect2_x2);
    float y2 = min(rect1_y2, rect2_y2);

    float w = max(0.f, x2 - x1);
    float h = max(0.f, y2 - y1);

    float area1 = (rect1_x2 - rect1_x1) * (rect1_y2 - rect1_y1);
    float area2 = (rect2_x2 - rect2_x1) * (rect2_y2 - rect2_y1);
    float area3 = w * h;

    float iou = area3 / (area1 + area2 - area3 + DBL_EPSILON);

    return iou;
}