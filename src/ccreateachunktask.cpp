#include "ccreateachunktask.h"

CCreateAChunkTask::CCreateAChunkTask()
{

}

CCreateAChunkTask::CCreateAChunkTask(std::vector<std::vector<CVoxel *> *> *layers, std::vector<CChunk *> *chunks, CWorld* world, int ymin,
                                     int ymax, GLWidget277* context)
    : mLayers(layers), mChunks(chunks), mWorld(world), ymin(ymin), ymax(ymax), mContext(context)
{

}

CCreateAChunkTask::CCreateAChunkTask(CChunk* chunk)
    : mChunk(chunk)
{

}

void CCreateAChunkTask::run() {





  /*  // go through each layer, and voxelize that layer's voxel plane
    for(int i = ymin; i < ymax; i++) {

        std::vector<CVoxel*> *currVoxelPlane = mLayers->at(i);
        currChunk ->setWorld(&mWorld);

        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld.createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }*/

    //currChunk->create();
    mChunk->populateVoxelBuffer();
    //mChunks->push_back(currChunk);

}
