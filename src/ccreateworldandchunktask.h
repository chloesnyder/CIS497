#ifndef CCREATEWORLDANDCHUNKTASK_H
#define CCREATEWORLDANDCHUNKTASK_H

#pragma once
#include <QRunnable>
#include "cchunk.h"
#include "cvoxel.h"


class CCreateWorldAndChunkTask : public QRunnable
{
public:
    CCreateWorldAndChunkTask();
    CCreateWorldAndChunkTask(CChunk* chunk, std::vector<std::vector<CVoxel*>*>* layers);
    void run();

private:
    CChunk* mChunk;
    std::vector<std::vector<CVoxel*>*>* mLayers;
};

#endif // CCREATEWORLDANDCHUNKTASK_H
