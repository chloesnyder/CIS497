#include "ccreateworldandchunktask.h"

CCreateWorldAndChunkTask::CCreateWorldAndChunkTask(CChunk* chunk, std::vector<std::vector<CVoxel*>*>* layers) :
    mChunk(chunk), mLayers(layers)
{


}

CCreateWorldAndChunkTask::CCreateWorldAndChunkTask()
{

}

void CCreateWorldAndChunkTask::run()
{
    mChunk->createVoxelBuffer();
}
