#ifndef VOXELIZER_H
#define VOXELIZER_H

#include "cimagereader.h"
#include "277files/scene/voxel.h"

class Voxelizer
{
public:
    Voxelizer();
    Voxelizer(img_t *slice, double z);

    void voxelizeImageSlice();
    std::vector<Voxel*> *getVoxelPlane();

private:
    img_t *mSlice;
    std::vector<Voxel*> *mVoxelPlane; //this is an array that represents a plane of voxels constructed from an image slice
    double mLength; // this is the z value of the plane



};

#endif // VOXELIZER_H
