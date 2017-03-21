#include "cworldarr.h"

CWorldArr::CWorldArr()
{
    CWorldArr(512, 801, 512);
}

CWorldArr::CWorldArr(int max_x, int max_y, int max_z)
{
    int size = max_x + max_y*max_x + max_z*max_x*max_y;

    voxColArr = new glm::vec4[size];
    voxelExistsAt = new int[size];
}


glm::vec4 CWorldArr::voxelAtIsColor(int x, int y, int z, int max_x, int max_y)
{
    int address = x + y*max_x + z*max_x*max_y;

    glm::vec4 color = voxColArr[address];
    return color;
}

int CWorldArr::hasVoxelAt(int x, int y, int z, int max_x, int max_y)
{
    int address = x + y*max_x + z*max_x*max_y;
    return voxelExistsAt[address];
}

void CWorldArr::addVoxelAt(int x, int y, int z, int max_x, int max_y, glm::vec4 color)
{
    int address = x + y*max_x + z*max_x*max_y;
    voxelExistsAt[address] = 1;
    voxColArr[address] = color;

}

bool CWorldArr::destroyVoxelAt(int x, int y, int z, int max_x, int max_y)
{
 //TODO
}
