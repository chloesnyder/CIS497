#ifndef CCREATEACHUNKTASK_H
#define CCREATEACHUNKTASK_H
#pragma once


#include "cchunk.h"/#include "cworld.h"

#include <QRunnable>

#include "cvoxel.h"
#include "cworldarr.h"
#include "cchunkarr.h"

class CCreateAChunkTask : public QRunnable
{
public:
    CCreateAChunkTask();

    CCreateAChunkTask(std::vector<std::vector<CVoxel *> *> *layers,
                                         std::vector<CChunk*> *chunks, CWorld world, int ymin, int ymax,  GLWidget277 *context);

    CCreateAChunkTask(std::vector<std::vector<CVoxel *> *> *layers,
                                         std::vector<CChunkArr*> *chunks, CWorldArr *world, int ymin, int ymax,  GLWidget277 *context);
    void run();
private:
    std::vector<std::vector<CVoxel*>*>* mLayers;
    std::vector<CChunk*> *mChunks;
    CWorld mWorld;

    std::vector<CChunkArr*> *mChunkArrs;
    CWorldArr *mWorldArr;


    int ymin;
    int ymax;

    int mMaxThreadCount;

    GLWidget277* mContext;
};

#endif // CCREATEACHUNKTASK_H
