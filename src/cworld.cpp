#include "cworld.h"

CWorld::CWorld()
{
    for (int i = 0; i < 512; i++) {
        for(int j = 0; j <=1; j++) {
            for(int k = 0; k < 512; k++) {
                addVoxelAt(i, j, k, CVoxel::EMPTY, glm::vec4());
            }
        }
    }
}

std::tuple<int, int, int> CWorld::coords(int x, int y, int z)
{
    return std::tuple<int, int, int>(x, y, z);
}




void CWorld::addVoxelLocationData(int x1, int x2, int y1, int y2, int z1, int z2, glm::vec4 color)
{
    // check order of given coordinates
    // make sure (xyz)_1 <= (xyz)_2
   if (x1 > x2) {
        int prev = x2;
        x2 = x1;
        x1 = prev;
    }
    if (y1 > y2) {
        int prev = y2;
        y2 = y1;
        y1 = prev;
    }
    if (z1 > z2) {
        int prev = z2;
        z2 = z1;
        z1 = prev;
    }

    for (int i = x1; i <= x2; i++) {
        for (int j = y1; j <= y2; j++) {
            for (int k = z1; k <= z2; k++) {
                addVoxelAt(i, j, k, CVoxel::NONEMPTY, color);
                //addVoxelColorAt(i, j, k, color);
            }
        }
    }
}


bool CWorld::hasVoxelAt(int x, int y, int z)
{
   bool result =  currWorldView.find(coords(x, y, z)) != currWorldView.end();
   return result;
}

CVoxel::VTYPE CWorld::voxelAtIsType(int x, int y, int z)
{
    if(!hasVoxelAt(x, y, z)) {
        std::cout << "No block at " << x << ", " << y << ", " << z << std::endl;
    } else {
        return currWorldView[coords(x, y, z)];
    }
}

glm::vec4 CWorld::voxelAtIsColor(int x, int y, int z) {
    if(!hasVoxelAt(x, y, z)) {
        std::cout << "Can't get color, no voxel at " << x << ", " << y << ", " << z << std::endl;
    } else {
        return voxColMap[coords(x, y, z)];
    }
}

bool CWorld::addVoxelAt(int x, int y, int z, CVoxel::VTYPE type, glm::vec4 color)
{
    if(!hasVoxelAt(x, y, z)) {
        currWorldView[coords(x,y,z)] = type;
        voxColMap[coords(x,y,z)] = color;
        return true;
    } else {
        if(voxelAtIsType(x, y, z) == CVoxel::EMPTY) {
            currWorldView[coords(x,y,z)] = type;
            voxColMap[coords(x,y,z)] = color;
            return true;
        }
        return false;
    }

}

bool CWorld::addVoxelColorAt(int x, int y, int z, glm::vec4 color)
{
    if(!hasVoxelAt(x, y, z)) {
        voxColMap[coords(x,y,z)] = color;
        return true;
    } else {
        if(voxelAtIsType(x, y, z) == CVoxel::EMPTY) {
            voxColMap[coords(x,y,z)] = color;
            return true;
        }
        return false;
    }

}

bool CWorld::destroyVoxelAt(int x, int y, int z)
{
    if(hasVoxelAt(x, y, z)) {
        std::map<std::tuple<int, int, int>, CVoxel::VTYPE>::iterator it;
        it = currWorldView.find(coords(x,y,z));
        currWorldView.erase(it);
        return true;
    } else {
        return false;
    }
}

// add in data for new chunk
// If data for a block is already there, then use old data
void CWorld::createChunkVoxelData(int x1, int x2, int y1, int y2, int z1, int z2, glm::vec4 color)
{
    addVoxelLocationData(x1, x2, y1, y2, z1, z2, color);
}

