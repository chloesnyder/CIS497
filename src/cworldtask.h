#ifndef CWORLDTASK_H
#define CWORLDTASK_H


#pragma once
#include <QRunnable>
#include "cworldarray.h"
#include "cvoxel.h"


class CWorldTask : public QRunnable
{
public:
    CWorldTask(std::vector<CVoxel *> *voxPlane, CWorldArray *world);
    CWorldTask();
    void run();
private:
    std::vector<CVoxel*>* mVoxPlane;
    CWorldArray* mWorldArr;
};

#endif // CWORLDTASK_H
