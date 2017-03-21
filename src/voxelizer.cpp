#include "voxelizer.h"

#include <QDir>
#include <QDirIterator>
#include <QThread>

CVoxelizer::CVoxelizer() : mImageReader()
{

}

void CVoxelizer::processFiles() {

    // Access directory of all images
    //QDir targetDir = QDir("/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/PPMS/firsthalf/firstquarter/firsteighth");
    QDir targetDir = QDir("/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/one");
    QStringList qsl; qsl.append("*.ppm");
    targetDir.setNameFilters(qsl);

    int voxelLength = 0;
    std::vector<CVoxelizeAnImageSliceTask*> *imageTasks = new std::vector<CVoxelizeAnImageSliceTask*>();

    QDirIterator it(targetDir, QDirIterator::Subdirectories);
    while(it.hasNext()) {

        // read the file, get the ppm
        const char* currFile = it.next().toStdString().c_str();
        mImageReader.readPPM(currFile);
        img_t* currImg = mImageReader.getImageArray();

        // voxelize the image
         CVoxelizeAnImageSliceTask *currImageTask = new CVoxelizeAnImageSliceTask(currImg, voxelLength, mAllLayers);
         currImageTask->start();
         imageTasks->push_back(currImageTask);

         voxelLength++;

    }
    // Check to see if thread is still running, kill when done
    threadCheck(imageTasks);
}

void CVoxelizer::threadCheck(std::vector<CVoxelizeAnImageSliceTask*> *imageTasks)
{
    int numThreads = imageTasks->size();


    // Check if still running
    bool still_running;
    do
    {
        still_running = false;
        for(unsigned int J = 0; J < numThreads; J++) {
            if(imageTasks->at(J)->isRunning())
            {
                still_running = true;
                break;
            }
        }
        if(still_running)
        {
            QThread::yieldCurrentThread();
        }
    } while(still_running);

    // clean up image thread objects
    for(unsigned int K = 0; K < numThreads; K++)
    {
        delete imageTasks->at(K);
    }
    delete imageTasks;

}
