#ifndef CWORLDARRAY_H
#define CWORLDARRAY_H

#pragma once

#include "cvoxel.h"
#include <la.h>
#include <iostream>
#include <math.h>

// This is when I admit defeat to Adam and try to


class CWorldArray
{
public:
    CWorldArray();
    CWorldArray(int layers);
    CWorldArray(CWorldArray const &cwa); // copy constructor

    bool hasVoxelAt(int x, int y, int z);
    CVoxel::VTYPE voxelAtIsType(int x, int y, int z);
    glm::vec4 voxelAtIsColor(int x, int y, int z);

    void createChunkVoxelData(glm::vec4 position, glm::vec4 color, int id);
    int getSize();

    glm::vec3 to3D(int idx);


private:
    int numLayers;
    std::vector<CVoxel::VTYPE> currWorldView;
    std::vector<glm::vec4> voxColArr;

    int calculateIndex(int x, int y, int z);

    bool addVoxelAt(int x, int y, int z, CVoxel::VTYPE type, glm::vec4 color, int id); // return true if successful

};

#endif // CWORLDARRAY_H
