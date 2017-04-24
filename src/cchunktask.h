#ifndef CCREATEWORLDANDCHUNKTASK_H
#define CCREATEWORLDANDCHUNKTASK_H

#pragma once
#include <QRunnable>
#include "cchunk.h"
#include "cvoxel.h"


class CCreateChunkTask : public QRunnable
{
public:
    CCreateChunkTask();
    CCreateChunkTask(CChunk* chunk);
    void run();
    bool isDone();

private:
    CChunk* mChunk;
};

#endif // CCREATEWORLDANDCHUNKTASK_H
