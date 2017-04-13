#ifndef CVOXELIZEANIMAGESLICETASK_H
#define CVOXELIZEANIMAGESLICETASK_H
#pragma once

#include <QRunnable>

#include "cimagereader.h"
#include "cvoxel.h"

class CVoxelizeAnImageSliceTask : public QRunnable
{
public:
    CVoxelizeAnImageSliceTask();
    CVoxelizeAnImageSliceTask(img_t *slice, int length, std::vector<std::vector<CVoxel *> *> *mAllLayers,
                              std::vector<int> *mMinXs, std::vector<int> *mMinZs,
                              std::vector<int> *mMaxXs, std::vector<int> *mMaxZs);
    void run();

    void setDensityThreshold(float dt) {densityThreshold = dt;}


private:
    img_t* mSlice;
    int mLength;
    std::vector<std::vector<CVoxel*>*> *mAllLayers;

    std::vector<int> *minXForEachLayer;
    std::vector<int> *minZForEachLayer;
    std::vector<int> *maxXForEachLayer;
    std::vector<int> *maxZForEachLayer;

    float densityThreshold;

    void checkMaxAndMinXAndZ(int minXForThisLayer, int maxXForThisLayer, int minZForThisLayer, int maxZForThisLayer, int row, int column);


};

#endif // CVOXELIZEANIMAGESLICETASK_H
