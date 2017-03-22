#include "ccreateworldandchunk.h"

CCreateWorldAndChunk::CCreateWorldAndChunk()
{


}

CCreateWorldAndChunk::CCreateWorldAndChunk(CChunk* chunk, CWorld* world, std::vector<std::vector<CVoxel*>*>* layers)
    : mChunk(chunk), mWorld(world), mLayers(layers)
{

}

void CCreateWorldAndChunk::run()
{

    //first, populate the world with the voxel data stored in the given layers
    for(int i = 0; i < mLayers->size(); i++) {

        std::vector<CVoxel*> *currVoxelPlane = mLayers->at(i);

        for(int vp = 0; vp < currVoxelPlane->size(); vp++)
        {
        //for(CVoxel* v : *currVoxelPlane) {
            CVoxel* v = currVoxelPlane->at(vp);

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld->createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }

    //then, create the chunk
    mChunk->populateVoxelBuffer();
    //mChunk->create();

}



