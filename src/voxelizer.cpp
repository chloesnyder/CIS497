#include "voxelizer.h"

#include <QDir>
#include <QDirIterator>
#include <QRunnable>
#include <QThreadPool>

CVoxelizer::CVoxelizer() : mImageReader()
{

}

void CVoxelizer::setTargetDirPath(QString path)
{
    targetDirPath = path;

}

void CVoxelizer::setDensityThreshold(float t)
{
    densityThreshold = t;
}

void CVoxelizer::processFiles()
{

    // Access directory of all images
    QDir targetDir = QDir(targetDirPath);
    QStringList qsl; qsl.append("*.ppm");
    targetDir.setNameFilters(qsl);

    int voxelLength = 0;
    std::vector<CVoxelizeAnImageSliceTask*> *imageTasks = new std::vector<CVoxelizeAnImageSliceTask*>();

    std::vector<CVoxel*>* firstVoxelPlane = new std::vector<CVoxel*>();

    QDirIterator it(targetDir, QDirIterator::Subdirectories);
    while(it.hasNext()) {

        // read the file, get the ppm
        const char* currFile = it.next().toStdString().c_str();
        mImageReader.readPPM(currFile);
        img_t* currImg = mImageReader.getImageArray();

        // voxelize the image
        CVoxelizeAnImageSliceTask *currImageTask = new CVoxelizeAnImageSliceTask(currImg, voxelLength, mAllLayers);
        currImageTask->setDensityThreshold(densityThreshold);
        imageTasks->push_back(currImageTask);

        if(voxelLength == 0)
        {
            voxelLength += 2;
        } else {
            voxelLength++;
        }

    }


    // start each of the threads and wait until all of them finished before exiting the function
    for(CVoxelizeAnImageSliceTask* task : *imageTasks)
    {
        QThreadPool::globalInstance()->start(task);
    }

    QThreadPool::globalInstance()->waitForDone();
}

