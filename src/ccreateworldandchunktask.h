#ifndef CCREATEWORLDANDCHUNKTASK_H
#define CCREATEWORLDANDCHUNKTASK_H

#include <QThreadPool>


class CCreateWorldAndChunkTask : public QThreadPool
{
public:
    CCreateWorldAndChunkTask();
    void run();
};

#endif // CCREATEWORLDANDCHUNKTASK_H
