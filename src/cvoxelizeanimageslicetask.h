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
    CVoxelizeAnImageSliceTask(img_t *slice, int length, std::vector<std::vector<CVoxel *> *> *mAllLayers);
    void run();

private:
    img_t* mSlice;
    int mLength;
    std::vector<std::vector<CVoxel*>*> *mAllLayers;

    int mMaxThreadCount;
};

#endif // CVOXELIZEANIMAGESLICETASK_H
