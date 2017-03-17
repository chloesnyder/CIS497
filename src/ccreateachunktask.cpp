#include "ccreateachunktask.h"

CCreateAChunkTask::CCreateAChunkTask()
{

}

CCreateAChunkTask::CCreateAChunkTask(std::vector<std::vector<CVoxel *> *> *layers, std::vector<CChunk *> *chunks, CWorld world, int ymin,
                                     int ymax, GLWidget277* context)
    : mLayers(layers), mChunks(chunks), mWorld(world), ymin(ymin), ymax(ymax), mContext(context)
{

}

void CCreateAChunkTask::run() {


    CChunk* currChunk = new CChunk(mContext);
    currChunk ->setXMin(0);
    currChunk ->setXMax(512);
    currChunk ->setYMin(ymin);
    currChunk ->setYMax(ymax);
    currChunk ->setZMin(0);
    currChunk ->setZMax(512);


    // go through each layer, and voxelize that layer's voxel plane
    for(int i = ymin; i < ymax; i++) {

        std::vector<CVoxel*> *currVoxelPlane = mLayers->at(i);
        currChunk->setWorld(&mWorld);

        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld.createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }

    //currChunk->populateVoxelBuffer();
   // currChunk->create();
    mChunks->push_back(currChunk);

}
