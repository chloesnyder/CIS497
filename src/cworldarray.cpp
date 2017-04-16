#include "cworldarray.h"

CWorldArray::CWorldArray()
{

}

CWorldArray::CWorldArray(int layers) : numLayers(layers)
{

    currWorldView = std::vector<CVoxel::VTYPE>();
    currWorldView.resize(512 * 512 * numLayers, CVoxel::EMPTY);

    voxColArr = std::vector<glm::vec4>();
    voxColArr.resize(512 * 512 * numLayers, glm::vec4(0, 0, 0, 0));

}

CWorldArray::CWorldArray(const CWorldArray &cwa)
    : currWorldView(cwa.currWorldView), voxColArr(cwa.voxColArr), numLayers(cwa.numLayers)
{

}

int CWorldArray::calculateIndex(int x, int y, int z)
{
    int xMax = 512;
    int zMax = 512;
    int yMax = numLayers;

   // int idx = (y * xMax * zMax) + (z * xMax) + x;
    int idx = (z * xMax * yMax) + (y * xMax) + x;
    return idx;
}

bool CWorldArray::hasVoxelAt(int x, int y, int z)
{
    int idx = calculateIndex(x, y, z);

    if(idx > -1 && idx < currWorldView.size())
    {
        bool isEmpty = (currWorldView.at(idx) == CVoxel::EMPTY); // true if empty
        return isEmpty;
    } else {
        std::cout << "hasVoxelAt Error: " << x << ", " << y << ", " z << " index calculation resulted in invalid index" << std::endl;
        return false;
    }

}

CVoxel::VTYPE CWorldArray::voxelAtIsType(int x, int y, int z)
{

    int idx = calculateIndex(x, y, z);
    if(idx > -1 && idx < currWorldView.size())
    {
        return currWorldView.at(idx);
    } else {
        std::cout << "voxelAtIsType error: x, y, z index calculation resulted in invalid index" << std::endl;
        return CVoxel::NULLTYPE;
    }
}

glm::vec4 CWorldArray::voxelAtIsColor(int x, int y, int z)
{
    int idx = calculateIndex(x, y, z);
    if(idx > -1 && idx < voxColArr.size())
    {
        return voxColArr.at(idx);
    } else {
        std::cout << "x, y, z index calculation resulted in invalid index" << std::endl;
        return glm::vec4(0,0,0,0);
    }
}

bool CWorldArray::addVoxelAt(int x, int y, int z, CVoxel::VTYPE type, glm::vec4 color, int id)
{

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
