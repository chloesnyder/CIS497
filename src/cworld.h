#ifndef CWORLD_H
#define CWORLD_H

#include "cvoxel.h"
#include <la.h>
#include <iostream>
#include <math.h>

// Class that stores the positions and locations of each block in the world
// Initializes a 512x512xN environment, where N is the number of image slices that have been read in
// For now, N = 512
// Every block has a specific height based on what image slice it represents

// Height is based off of which slice is being read in (i.e. image 0 = height 0), not perlin noise
class CWorld
{
private:
    std::map<std::tuple<int, int, int>, CVoxel::VTYPE> currWorldView = std::map<std::tuple<int, int, int>, CVoxel::VTYPE>();

    std::tuple<int, int, int> coords(int x, int y, int z);

    std::map<std::tuple<int, int, int>, glm::vec4> voxColMap = std::map<std::tuple<int, int, int>, glm::vec4>();

    void addVoxelLocationData(int x1, int x2, int y1, int y2, int z1, int z2, glm::vec4 color);

    // Declare array of pairs of voxel address with voxel type, or voxel color
    // Each array index corresponds to voxel ID
    // Start with max voxels possible (801 * 512 * 512, may update this later)
    std::pair<std::tuple<int, int, int>, CVoxel::VTYPE>* currWorldArr = new std::pair<std::tuple<int, int, int>, CVoxel::VTYPE>[801*512*512];
    std::pair<std::tuple<int, int, int>, glm::vec4>* voxColArr = new std::pair<std::tuple<int, int, int>, glm::vec4>[801*512*512];

public:
    CWorld();

    int viewWorld = 512; // height width length of view of world by num voxels
    bool hasVoxelAt(int x, int y, int z);
    CVoxel::VTYPE voxelAtIsType(int x, int y, int z);
    glm::vec4 voxelAtIsColor(int x, int y, int z);

    bool addVoxelAt(int x, int y, int z, CVoxel::VTYPE type, glm::vec4 color, int id); // return true if successful
    bool destroyVoxelAt(int x, int y, int z);

    // add in data for new chunk
    // If data for a block is already there, then use old data
    void createChunkVoxelData(glm::vec4 position, glm::vec4 color, int id);

    void destroy();

};

#endif // CWORLD_H
