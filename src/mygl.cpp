#include "mygl.h"
#include <la.h>
#include<QFileDialog>
#include<QFile>
#include<QStringList>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <QApplication>
#include <QKeyEvent>
#include <QThreadPool>


//#define OLD_MULTITHREADING
#define MULTITHREADING

MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent), prog_lambert(this), prog_wire(this), camera(Camera())
{
    setFocusPolicy(Qt::ClickFocus);

}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);

}


void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);


    ///TRIANGLE LINES
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);


    // PLAY WITH BLEND FUNC? GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE and GL_SAMPLE_COVERAGE?
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // DO I WANT THIS?
    /* glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);*/


    /// TRIANGLE LINES
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);


    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(.5, .5, .5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);


    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl", ":/glsl/wire.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    // Read in the test image
    //mImageReader = CImageReader();

    processFiles();
}

void MyGL::processFiles() {

    mVoxelizer = CVoxelizer();
    mVoxelizer.processFiles();
    createChunkVector();

    // call create on every chunk
    /* for(int i = 0; i < chunks.size(); i++)
    {
        chunks.at(i)->populateVoxelBuffer();
        chunks.at(i)->create();
    }*/
}

void MyGL::resizeGL(int w, int h)
{
    // camera = Camera(w, h, glm::vec3(256, 100, 265), glm::vec3(255, 0, 260), glm::vec3(0, 1, 0));//Camera(w, h);
    camera = Camera(w, h, glm::vec3(256, 100, 265), glm::vec3(255, 0, 260), glm::vec3(0, 1, 0));
    //Camera()


    glm::mat4 viewproj = camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_wire.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(camera.getViewProj());
    prog_wire.setViewProjMatrix(camera.getViewProj());

    glm::mat4 model = glm::mat4(1.0f);
    prog_lambert.setModelMatrix(model);

    for(unsigned int i = 0; i < chunks.size(); i++) {
        //CChunk* currChunk = chunks[i];
        CChunkArr* currChunk = chunks[i];
        prog_lambert.draw(*currChunk);
    }
}


//update to do multithreading
void MyGL::createChunkVector()
{

    /// NEW WITH ARRAYS INSTEAD


    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    std::vector<CCreateAChunkTask*> *chunkTasks = new std::vector<CCreateAChunkTask*>();

    // Init world array and populate with color data
    // A voxel at (x, y, z) has a color
    // This color stored in the world array at address = x + max_x*y + max_x*max_y*z
    mWorldArr = new CWorldArr(513, allLayers->size(), 513);

    for(int i = 0; i < allLayers->size(); i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);
        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();

            mWorldArr->addVoxelAt(voxPos.x, voxPos.y, voxPos.z, voxCol);
        }
    }

   CChunkArr* allLayerChunk = new CChunkArr(this);
   allLayerChunk ->setXMin(0);
   allLayerChunk ->setXMax(512);
   allLayerChunk ->setYMin(0);
   allLayerChunk ->setYMax(allLayers->size());
   allLayerChunk ->setZMin(0);
   allLayerChunk ->setZMax(512);
   allLayerChunk ->setWorld(mWorldArr);


   allLayerChunk->populateVoxelBuffer();
   allLayerChunk->create();
   chunks.push_back(allLayerChunk);

  /*  // threading
    int totalLayers = 50;//allLayers->size();
    int incr = totalLayers / 10;
    int layer;
    int ymin;
    int ymax;
    int numThreads = 0;

    for(layer = 0; layer < totalLayers; layer += incr)
    {
        ymin = layer;
        ymax = layer + incr;
        if(ymax > totalLayers) ymax = totalLayers;

        CCreateAChunkTask *currChunkTask = new CCreateAChunkTask(allLayers, &chunks, mWorldArr, ymin, ymax, this);
        QThreadPool::globalInstance()->start(currChunkTask);
        numThreads++;
    }

    if(QThreadPool::globalInstance()->waitForDone())
    {
       for(int thread = 0; thread < numThreads; thread++)
        {
            chunks.at(thread)->create();
        }
    }

*/


    /*#ifdef OLD_MULTITHREADING
    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    std::vector<CCreateAChunkTask*> *chunkTasks = new std::vector<CCreateAChunkTask*>();
    int numThreads = 0;

    int totalLayers = 20;//allLayers->size();
    int incr = totalLayers / 10;
    int layer;
    int ymin;
    int ymax;


    // Each thread populates the vertex and index buffers for a chunk
    // a chunk is a subset of the overall structure, from layer ymin to layer ymax
    for(layer = 0; layer < totalLayers; layer += incr)
    {

        ymin = layer;
        ymax = layer + incr;
        if (ymax > totalLayers) ymax = totalLayers;
        CCreateAChunkTask *currChunkTask = new CCreateAChunkTask(allLayers, &chunks, mWorld, ymin, ymax, this);
        currChunkTask->start();
        chunkTasks->push_back(currChunkTask);
        numThreads++;
    }

    for(int thread = 0; thread < numThreads; thread++)
    {
        chunkTasks->at(thread)->wait();
        chunks.at(thread)->populateVoxelBuffer();
        chunks.at(thread)->create();
    }

    threadCheck(chunkTasks);

#elif

     std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();

    CChunk* allLayerChunk = new CChunk(this);
    allLayerChunk ->setXMin(0);
    allLayerChunk ->setXMax(512);
    allLayerChunk ->setYMin(0);
    allLayerChunk ->setYMax(allLayers->size());
    //allLayerChunk ->setYMax(20);
    allLayerChunk ->setZMin(0);
    allLayerChunk ->setZMax(512);

    // turn this part into the multithreading, to have multiple chunks, push back, render later

    // go through each layer, and voxelize that layer's voxel plane
    for(int i = 0; i < allLayers->size(); i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);
        allLayerChunk ->setWorld(&mWorld);

        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld.createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }

    allLayerChunk->populateVoxelBuffer();
    allLayerChunk->create();
    chunks.push_back(allLayerChunk);
#endif*/
}

void MyGL::threadCheck(std::vector<CCreateAChunkTask*> *chunkTasks)
{
//    int numThreads = chunkTasks->size();

//    // Check if still running
//    bool still_running;
//    do
//    {
//        still_running = false;
//        for(unsigned int J = 0; J < numThreads; J++) {
//            if(chunkTasks->at(J)->isRunning())
//            {
//                still_running = true;
//                break;
//            }
//        }
//        if(still_running)
//        {
//            QThread::yieldCurrentThread();
//        }
//    } while(still_running);

//    // clean up image thread objects
//    for(unsigned int K = 0; K < numThreads; K++)
//    {
//        delete chunkTasks->at(K);
//    }
//    delete chunkTasks;
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 10.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        camera.TranslateAlongUp(amount);
    }
    camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

bool MyGL::initTextures3d(){

}

