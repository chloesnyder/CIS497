#ifndef CVOXELIZEANIMAGESLICETASK_H
#define CVOXELIZEANIMAGESLICETASK_H
#pragma once

#include <QThread>

#include "cimagereader.h"
#include "cvoxel.h"

class CVoxelizeAnImageSliceTask : public QThread
{
public:
    CVoxelizeAnImageSliceTask();
    CVoxelizeAnImageSliceTask(img_t *slice, int length, std::vector<std::vector<CVoxel *> *> *mAllLayers);
    void run();

    void setDensityThreshold(float dt) {densityThreshold = dt;}

private:
    img_t* mSlice;
    int mLength;
    std::vector<std::vector<CVoxel*>*> *mAllLayers;

    float densityThreshold;

    int mMaxThreadCount;
};

#endif // CVOXELIZEANIMAGESLICETASK_H
