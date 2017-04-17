#include "cworldtask.h"

CWorldTask::CWorldTask()
{

}


CWorldTask::CWorldTask(std::vector<CVoxel*> *voxPlane, CWorldArray* world) :
    mVoxPlane(voxPlane), mWorldArr(world)
{

}

void CWorldTask::run()
{
    for(CVoxel* v : *mVoxPlane) {

        glm::vec4 voxPos = v->getPosition();
        glm::vec4 voxCol = v->getColor();
        int voxID = v->getID();

        mWorldArr->createChunkVoxelData(voxPos, voxCol, voxID);
    }

}
