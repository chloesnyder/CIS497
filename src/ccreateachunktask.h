#ifndef CCREATEACHUNKTASK_H
#define CCREATEACHUNKTASK_H
#pragma once

#include <QThread>

#include "cchunk.h"
#include "cvoxel.h"
#include "cworld.h"

class CCreateAChunkTask : public QThread
{
public:
    CCreateAChunkTask();
    CCreateAChunkTask(std::vector<std::vector<CVoxel*>*>* layers,
                      std::vector<CChunk*> *chunks, CWorld world, int ymin, int ymax, GLWidget277 *context);
    void run() override;
private:
    std::vector<std::vector<CVoxel*>*>* mLayers;
    std::vector<CChunk*> *mChunks;
    CWorld mWorld;

    int ymin;
    int ymax;

    int mMaxThreadCount;

    GLWidget277* mContext;
};

#endif // CCREATEACHUNKTASK_H
