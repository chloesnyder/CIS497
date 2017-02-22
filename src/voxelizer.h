#ifndef VOXELIZER_H
#define VOXELIZER_H

#include "cimagereader.h"
#include "cvoxel.h"

class Voxelizer
{
public:
    Voxelizer();
    Voxelizer(std::vector<img_t *> *slices);

    void voxelizeAllImages();

    std::vector<std::vector<CVoxel*>*> *getAllLayers() { return mAllLayers; }

private:


    std::vector<img_t*> *mSlices;
    std::vector<std::vector<CVoxel*>*> *mAllLayers = new std::vector<std::vector<CVoxel*>*>(); // vector of every voxel plane



};

#endif // VOXELIZER_H
