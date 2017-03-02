#ifndef CVOXELIZER_H
#define CVOXELIZER_H

#include "cvoxelizeanimageslicetask.h"
#include "cimagereader.h"

class CVoxelizer
{
public:
    CVoxelizer();

    void processFiles();

    std::vector<std::vector<CVoxel*>*> *getAllLayers() { return mAllLayers; }

private:


    std::vector<img_t*> *mSlices;
    std::vector<std::vector<CVoxel*>*> *mAllLayers = new std::vector<std::vector<CVoxel*>*>(); // vector of every voxel plane

    CImageReader mImageReader;

    void threadCheck(std::vector<CVoxelizeAnImageSliceTask*> *imageTasks);


};

#endif // VOXELIZER_H
