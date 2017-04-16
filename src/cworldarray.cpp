#include "cworldarray.h"

CWorldArray::CWorldArray()
{

}

CWorldArray::CWorldArray(int layers) : numLayers(layers)
{

    currWorldView = std::vector<CVoxel::VTYPE>();
    currWorldView.resize(513 * 513 * (numLayers+1), CVoxel::EMPTY);

    voxColArr = std::vector<glm::vec4>();
    voxColArr.resize(513 * 513 * (numLayers+1), glm::vec4(0, 0, 0, 1));

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

    int idx = x + y*xMax + z*yMax*xMax;

    //int idx = y + z*yMax + x*zMax*yMax;

   // [x + y*WIDTH + Z*WIDTH*DEPTH]
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

    int idx = calculateIndex(x, y, z);
    if(idx > -1 && idx < currWorldView.size())
    {
        return currWorldView.at(idx);
    } else {
        std::cout << "voxelAtIsType Error: " << x << ", " << y << ", " << z << " index calculation resulted in invalid index" << std::endl;
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
        std::cout << "voxelAtIsColor Error: " << x << ", " << y << ", " << z << " index calculation resulted in invalid index" << std::endl;
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
