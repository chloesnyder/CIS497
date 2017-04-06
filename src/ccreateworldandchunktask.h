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
    CCreateWorldAndChunkTask(CChunk* chunk);
    void run();

private:
    CChunk* mChunk;
};

#endif // CCREATEWORLDANDCHUNKTASK_H
