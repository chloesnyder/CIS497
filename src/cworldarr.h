#ifndef CWORLD_H
#define CWORLD_H

#include "cvoxel.h"
#include <la.h>
#include <iostream>
#include <math.h>


typedef struct {
    float r, g, b, a;
} COLOR;

// Class that stores the positions and locations of each block in the world
// Initializes a 512x512xN environment, where N is the number of image slices that have been read in
// For now, N = 512
// Every block has a specific height based on what image slice it represents

// Height is based off of which slice is being read in (i.e. image 0 = height 0), not perlin noise
class CWorldArr
{
private:

    glm::vec4 *voxColArr; // an array of the colors stored at a given voxel position, where index = x + max_x*y + max_x*max_y*z
    int *voxelExistsAt; // an array of 0 and 1. 0 means that there is no voxel here, 1 means that there is

public:
    CWorldArr();
    CWorldArr(int max_x, int max_y, int max_z);

    glm::vec4 voxelAtIsColor(int x, int y, int z, int max_x, int max_y);

    void addVoxelAt(int x, int y, int z, int max_x, int max_y, glm::vec4 color); // return true if successful
    bool destroyVoxelAt(int x, int y, int z, int max_x, int max_y);

    int hasVoxelAt(int x, int y, int z, int max_x, int max_y);

};

#endif // CWORLD_H
