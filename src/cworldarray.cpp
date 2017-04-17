#include "cworldarray.h"

CWorldArray::CWorldArray()
{

}

CWorldArray::CWorldArray(int layers) : numLayers(layers)
{

    currWorldView = std::vector<CVoxel::VTYPE>(512 * 512 * (numLayers), CVoxel::EMPTY);
    voxColArr = std::vector<glm::vec4>(512 * 512 * (numLayers), glm::vec4(1, 1, 1, 0));

}

CWorldArray::CWorldArray(const CWorldArray &cwa)
    : currWorldView(cwa.currWorldView), voxColArr(cwa.voxColArr), numLayers(cwa.numLayers)
{

}

int CWorldArray::getSize()
{
    return currWorldView.size();
}

glm::vec3 CWorldArray::to3D(int idx)
{
    int xMax = 512;
    int yMax = numLayers;

    int z = idx / (xMax * yMax);
    idx -= (z * xMax * yMax);
    int y = idx / xMax;
    int x = idx % xMax;
    return glm::vec3(x, y, z);
}

int CWorldArray::calculateIndex(int x, int y, int z)
{
    int xMax = 512;
    int zMax = 512;
    int yMax = numLayers;

    int idx = x + y*xMax + z*yMax*xMax;

    return idx;
}

bool CWorldArray::hasVoxelAt(int x, int y, int z)
{
    int idx = calculateIndex(x, y, z);

    if(idx > -1 && idx < currWorldView.size())
    {
        bool isNotEmpty = (currWorldView.at(idx) == CVoxel::NONEMPTY); // true if not empty
        return isNotEmpty;
    } else {
        //   std::cout << "hasVoxelAt Error: " << x << ", " << y << ", " << z << " index calculation resulted in invalid index" << std::endl;
        // for grid cells at edge cases, such as 512, 0, 512, this case will be hit because grid cell will cehck something like 513, 0, 512
        return false;
    }

}

CVoxel::VTYPE CWorldArray::voxelAtIsType(int x, int y, int z)
{
    if(hasVoxelAt(x, y, z))
    {
        int idx = calculateIndex(x, y, z);
        return currWorldView.at(idx);
    } else {
        return CVoxel::EMPTY;
    }
}

glm::vec4 CWorldArray::voxelAtIsColor(int x, int y, int z)
{
    if(hasVoxelAt(x, y, z))
    {
        int idx = calculateIndex(x, y, z);
        return voxColArr.at(idx);
    } else {
       return glm::vec4(.5, .5, .5, 0);
    }
}

bool CWorldArray::addVoxelAt(int x, int y, int z, CVoxel::VTYPE type, glm::vec4 color, int id)
{

    // need a case to put empty voxels with a density at the boundaries? why is it not showing


    if(!hasVoxelAt(x, y, z))
    {
        int idx = calculateIndex(x, y, z);
        if(idx > -1 && idx < voxColArr.size() && idx < currWorldView.size())
        {
            currWorldView.at(idx) = type;
            voxColArr.at(idx) = color;
            return true;
        }
        std::cout << "invalid index" << std::endl;
        return false;
    } else {
        if(voxelAtIsType(x, y, z) == CVoxel::EMPTY)
        {
            int idx = calculateIndex(x, y, z);
            if(idx > -1 && idx < voxColArr.size() && idx < currWorldView.size())
            {
                currWorldView.at(idx) = type;
                voxColArr.at(idx) = color;
                return true;
            }
        }
        std::cout << "invalid index" << std::endl;
        return false;
    }
}

void CWorldArray::createChunkVoxelData(glm::vec4 position, glm::vec4 color, int id)
{
    addVoxelAt(position.x, position.y, position.z, CVoxel::NONEMPTY, color, id);
}
