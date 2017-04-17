#include "cchunktask.h"

CCreateChunkTask::CCreateChunkTask(CChunk* chunk) :
    mChunk(chunk)
{


}

CCreateChunkTask::CCreateChunkTask()
{

}

void CCreateChunkTask::run()
{
    mChunk->createVoxelBuffer();
}
