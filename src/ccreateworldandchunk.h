#ifndef CCREATEWORLDANDCHUNK_H
#define CCREATEWORLDANDCHUNK_H

#pragma once

#include "cchunk.h"

#include <QRunnable>


class CCreateWorldAndChunk : public QRunnable
{
public:
    CCreateWorldAndChunk();

    CCreateWorldAndChunk(CChunk* chunk, CWorld* world, std::vector<std::vector<CVoxel*>*>* layers);

    void run();

private:

    CChunk* mChunk;
    CWorld* mWorld;
    std::vector<std::vector<CVoxel*>*>* mLayers;

};

#endif // CCREATEWORLDANDCHUNK_H
