#include "SortRect.h"


TrackerState SortRect::toTrackerState(void) {

    TrackerState state;
    state.centerX = centerX;
    state.centerY = centerY;
    state.area = width * height;
    state.aspectRatio = width / height;
    
    return state;
}

void SortRect::fromTrackerState(TrackerState state) {

    centerX = state.centerX;
    centerY = state.centerY;

    if(state.area > 0) {
        width = sqrt(state.area * state.aspectRatio);
        height = state.area / width;
    }
    else {
        width = 0;
        height = 0;
    }

}