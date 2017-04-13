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

    void setTargetDirPath(QString path);
    void setDensityThreshold(float t);

    std::vector<int> *getMinXForAllLayers() { return minXForAllLayers; }
    std::vector<int> *getMinZForAllLayers() { return minZForAllLayers; }
    std::vector<int> *getMaxXForAllLayers() { return maxXForAllLayers; }
    std::vector<int> *getMaxZForAllLayers() { return maxZForAllLayers; }

    void destroy();

private:


    std::vector<img_t*> *mSlices;
    std::vector<std::vector<CVoxel*>*> *mAllLayers = new std::vector<std::vector<CVoxel*>*>(); // vector of every voxel plane

    CImageReader mImageReader;

    void threadCheck(std::vector<CVoxelizeAnImageSliceTask*> *imageTasks);

    QString targetDirPath;

    float densityThreshold;


    // stores the min and max location for voxels added to the plane
    // this is done to cut down on the number of loops later in the code
    std::vector<int> *minXForAllLayers = new std::vector<int>();
    std::vector<int> *minZForAllLayers = new std::vector<int>();
    std::vector<int> *maxXForAllLayers = new std::vector<int>();
    std::vector<int> *maxZForAllLayers = new std::vector<int>();


};

#endif // VOXELIZER_H
