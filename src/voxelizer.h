#ifndef VOXELIZER_H
#define VOXELIZER_H

#include "cimagereader.h"
#include "cvoxel.h"

class Voxelizer
{
public:
    Voxelizer();
    Voxelizer(img_t *slice, double y);

    void voxelizeImageSlice();
    std::vector<CVoxel*> *getVoxelPlane();
    double getLength() {return mLength;}

private:
    img_t *mSlice;
    std::vector<CVoxel*> *mVoxelPlane; //this is an array that represents a plane of voxels constructed from an image slice
    double mLength; // this is the z value of the plane



};

#endif // VOXELIZER_H
