#include "ccreateworldandchunktask.h"

CCreateWorldAndChunkTask::CCreateWorldAndChunkTask(CChunk* chunk) :
    mChunk(chunk)
{


}

CCreateWorldAndChunkTask::CCreateWorldAndChunkTask()
{

}

void CCreateWorldAndChunkTask::run()
{
    mChunk->createVoxelBuffer();
}
