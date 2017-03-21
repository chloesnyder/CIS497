#include "cworld.h"

CWorld::CWorld()
{
}

std::tuple<int, int, int> CWorld::coords(int x, int y, int z)
{
    return std::tuple<int, int, int>(x, y, z);
}

bool CWorld::hasVoxelAt(int x, int y, int z)
{
   bool result =  currWorldView.find(coords(x, y, z)) != currWorldView.end();
   return result;
}

CVoxel::VTYPE CWorld::voxelAtIsType(int x, int y, int z)
{
    if(!hasVoxelAt(x, y, z)) {
        //std::cout << "No block at " << x << ", " << y << ", " << z << std::endl;
    } else {
        return currWorldView.at(coords(x,y,z));//currWorldView[coords(x, y, z)];
    }
}

glm::vec4 CWorld::voxelAtIsColor(int x, int y, int z) {
    if(!hasVoxelAt(x, y, z)) {
        return glm::vec4(0, 0, 0, 0);
        //std::cout << "Can't get color, no voxel at " << x << ", " << y << ", " << z << std::endl;
    } else {
        return voxColMap.at(coords(x,y,z));//voxColMap[coords(x, y, z)];
    }
}

bool CWorld::addVoxelAt(int x, int y, int z, CVoxel::VTYPE type, glm::vec4 color, int id)
{
    if(!hasVoxelAt(x, y, z)) {

         std::map<std::tuple<int, int, int>, CVoxel::VTYPE>::iterator it = currWorldView.begin();
         std::map<std::tuple<int, int, int>, glm::vec4>::iterator itCol = voxColMap.begin();
         currWorldView.insert(it, std::pair<std::tuple<int, int, int>, CVoxel::VTYPE>(coords(x,y,z),type));
         voxColMap.insert(itCol, std::pair<std::tuple<int, int, int>, glm::vec4>(coords(x,y,z),color));
        return true;
    } else {
        if(voxelAtIsType(x, y, z) == CVoxel::EMPTY) {

            std::map<std::tuple<int, int, int>, CVoxel::VTYPE>::iterator it = currWorldView.begin();
            std::map<std::tuple<int, int, int>, glm::vec4>::iterator itCol = voxColMap.begin();
            currWorldView.insert(it, std::pair<std::tuple<int, int, int>, CVoxel::VTYPE>(coords(x,y,z),type));
            voxColMap.insert(itCol, std::pair<std::tuple<int, int, int>, glm::vec4>(coords(x,y,z),color));
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
void CWorld::createChunkVoxelData(glm::vec4 position, glm::vec4 color, int id)
{
    addVoxelAt(position.x, position.y, position.z, CVoxel::NONEMPTY, color, id);
}

