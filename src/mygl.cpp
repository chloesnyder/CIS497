#include "mygl.h"
#include <la.h>
#include<QFileDialog>
#include<QFile>
#include<QStringList>
#include<QThreadPool>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <QApplication>
#include <QKeyEvent>


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
}

void MyGL::resizeGL(int w, int h)
{
    camera = Camera(w, h, glm::vec3(256, 100, 265), glm::vec3(255, 0, 260), glm::vec3(0, 1, 0));//Camera(w, h);

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
        CChunk* currChunk = chunks[i];
        // currChunk->setCameraForward(glm::vec4(camera.look, 0));
        // currChunk->recomputeAttributes();
        prog_lambert.draw(*currChunk);
    }
}


//update to do multithreading
void MyGL::createChunkVector()
{

#ifdef MULTITHREADING

    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    std::vector<CCreateWorldAndChunk*> *chunkTasks = new std::vector<CCreateWorldAndChunk*>();
    int numThreads = 0;

    int totalLayers = allLayers->size();
    int incr = totalLayers / 2;
    int layer;
    int curr_ymin;
    int curr_ymax;


    for(int layer = 0; layer < totalLayers; layer += incr)
    {
        curr_ymin = layer;
        curr_ymax = layer + incr;
        if(curr_ymax > totalLayers) curr_ymax = totalLayers;

        CWorld* currWorld = new CWorld();

        CChunk* currChunk = new CChunk(this);
        currChunk ->setXMin(0);
        currChunk ->setXMax(512);
        currChunk ->setYMin(curr_ymin);
        currChunk ->setYMax(curr_ymax);
        currChunk ->setZMin(0);
        currChunk ->setZMax(512);
        currChunk ->setWorld(currWorld);

        chunks.push_back(currChunk);
        worlds.push_back(currWorld);

    }

    for(int i = 0; i < chunks.size(); i++)
    {

        CChunk* chunk = chunks.at(i);
        CWorld* world = worlds.at(i);

        int ymin = chunk->getYMin();
        int ymax = chunk->getYMax();

        // get a vector that copies the data from allLayers.at(ymin) to allLAyers.at(ymax)
        // this is newly allocated memory, so multiple threads hsould never be reading from this
        // basically, in this sequential part of my code, I am allocating distinct portions of memory
        // then passing pointers to these places in memories, so multiple threads can operate on multiple places
        // i am trading space for time!
        layers = new std::vector<std::vector<CVoxel*>*>(allLayers->begin() + ymin, allLayers->begin() + ymax);
        // it is ok that this is an obejct and
        //that this data will be popped off the stack because it'll be copied into world which is maintained from here in a poitner
        // so the data will be preserved

        CCreateWorldAndChunk* thread = new CCreateWorldAndChunk(chunk, world, layers);
        chunkTasks->push_back(thread);
    }

    QThreadPool::globalInstance()->setMaxThreadCount(1);

    for(CCreateWorldAndChunk* task : *chunkTasks)
    {
        QThreadPool::globalInstance()->start(task);
    }

   if(QThreadPool::globalInstance()->waitForDone())
    {
        for (CChunk* chunkToCreate : chunks) {
            chunkToCreate->create();


            /* Question:
             *
             * Do I need to bring the "create" into mygl so this all happens in one place?
             * i.e., do I ned to create a bunch of separate bufIdx and bufVertdata
             * (so generate a couple of separate buffers) and then call swap buffers on it?
             * Do I need to swap buffers when I call draw?
             *
             * Do I need to interleave my data so all these vertices and indices get loaded into
             * just one buffer that is drawn from when i draw in paintGl?
             *
             * */

           /*
            std::vector<glm::vec4>* vertices = chunkToCreate->getVertices();
            std::vector<GLuint>* indicies = chunkToCreate->getIndices();

            count = indices->size();
            generateIdx();
            context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
            context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() *
                                  sizeof(GLuint), mIndices->data(), GL_STATIC_DRAW);

            generateVertData();
            context->glBindBuffer(GL_ARRAY_BUFFER, bufVertData);
            context->glBufferData(GL_ARRAY_BUFFER, vertices->size() *
                                  sizeof(glm::vec4), vertices->data(), GL_STATIC_DRAW);

            */
        }
    }


    /*
    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    std::vector<CCreateAChunkTask*> *chunkTasks = new std::vector<CCreateAChunkTask*>();
    int numThreads = 0;

    int totalLayers = 20;//allLayers->size();
    int incr = totalLayers / 2;
    int layer;
    int ymin;
    int ymax;

    for(int i = 0; i < totalLayers; i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);

        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld.createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }

    //QThreadPool::globalInstance()->setMaxThreadCount(1);

    int prevMax = 0;

    for(layer = 0; layer < totalLayers; layer += incr)
    {

        if(layer != 0)
        {
            ymin = prevMax;
        } else {
            ymin = 0;
        }

        //ymin = layer;
        ymax = layer + incr;
        prevMax = ymax;
        if(ymax > totalLayers) ymax = totalLayers;

        CChunk* currChunk = new CChunk(this);
        currChunk ->setXMin(0);
        currChunk ->setXMax(513);
        currChunk ->setYMin(ymin);
        currChunk ->setYMax(ymax+1);
        currChunk ->setZMin(0);
        currChunk ->setZMax(513);
        currChunk ->setWorld(&mWorld);

        chunks.push_back(currChunk);
    }

    for(CChunk* chunk : chunks)
    {
        CCreateAChunkTask *currChunkTask = new CCreateAChunkTask(chunk);
        numThreads++;
        QThreadPool::globalInstance()->start(currChunkTask);

    }


    if(QThreadPool::globalInstance()->waitForDone())
    {
        for (CChunk* chunk : chunks) {
            chunk->create();
        }
    }
*/

    /*
    // Each thread populates the vertex and index buffers for a chunk
    // a chunk is a subset of the overall structure, from layer ymin to layer ymax
    for(layer = 0; layer <= totalLayers; layer += incr)
    {

        ymin = layer;
        ymax = layer + incr - 1;
        if (ymax > totalLayers) ymax = totalLayers;
        CCreateAChunkTask *currChunkTask = new CCreateAChunkTask(allLayers, &chunks, mWorld, ymin, ymax, this);
        currChunkTask->start();
        // nothing is getting pushed back... why? do thread check first?
        chunkTasks->push_back(currChunkTask);
        numThreads++;
    }

    threadCheck(chunkTasks);

    // call create on every chunk
    for(int i = 0; i < chunks.size(); i++)
    {
        chunks.at(i)->create();
    }

   // threadCheck(chunkTasks);*/
#else

    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();

    CChunk* allLayerChunk = new CChunk(this);
    allLayerChunk ->setXMin(0);
    allLayerChunk ->setXMax(512);
    allLayerChunk ->setYMin(0);
    // allLayerChunk ->setYMax(allLayers->size());
    allLayerChunk ->setYMax(20);
    allLayerChunk ->setZMin(0);
    allLayerChunk ->setZMax(512);
    allLayerChunk ->setWorld(&mWorld);

    // turn this part into the multithreading, to have multiple chunks, push back, render later

    // go through each layer, and voxelize that layer's voxel plane
    for(int i = 0; i < 20; i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);

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
#endif
}

void MyGL::threadCheck(std::vector<CCreateAChunkTask*> *chunkTasks)
{
    /*    int numThreads = chunkTasks->size();

    // Check if still running
    bool still_running;
    do
    {
        still_running = false;
        for(unsigned int J = 0; J < numThreads; J++) {
            if(chunkTasks->at(J)->isRunning())
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
        delete chunkTasks->at(K);
    }
    delete chunkTasks;*/
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

