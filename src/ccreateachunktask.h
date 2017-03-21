#ifndef CCREATEACHUNKTASK_H
#define CCREATEACHUNKTASK_H
#pragma once

//#include <QThread>
#include <QRunnable>

#include "cchunk.h"
#include "cvoxel.h"
#include "cworld.h"

class CCreateAChunkTask : public QRunnable
{
public:
    CCreateAChunkTask();
    CCreateAChunkTask(std::vector<std::vector<CVoxel*>*>* layers,
                      std::vector<CChunk*> *chunks, CWorld* world, int ymin, int ymax, GLWidget277 *context);
    CCreateAChunkTask(CChunk* chunk);
    void run();
private:
    std::vector<std::vector<CVoxel*>*>* mLayers;
    std::vector<CChunk*> *mChunks;
    CWorld* mWorld;

    CChunk* mChunk;

    int ymin;
    int ymax;

    int mMaxThreadCount;

    GLWidget277* mContext;
};

#endif // CCREATEACHUNKTASK_H
