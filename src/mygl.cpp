#include "mygl.h"
#include <la.h>
#include<QFileDialog>
#include<QFile>
#include<QStringList>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cstring>
#include <QApplication>
#include <QKeyEvent>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QDirIterator>
#include <QThreadPool>

//#define MULTITHREADING



void MyGL::slot_tissue_preset(int s)
{
    densityThreshold = s;
}

void MyGL::slot_get_density_threshold(double thresh)
{
    densityThreshold = thresh;
}

void MyGL::slot_on_slider_moved(int num)
{


    // If there are image files associated with this, display the image on the slider
    if(!ctScanFilePath.isEmpty())
    {
        QDir imgDir = QDir(ctScanFilePath);
        QStringList qsl; qsl.append("*.ppm");
        imgDir.setNameFilters(qsl);

        QDirIterator it(imgDir, QDirIterator::Subdirectories);
        QStringList fileList = imgDir.entryList();

        // figure out the "percentage" of the way up it is
        float percent = (float) num / 100.0f;
        float layer = maxLayers * percent;
        currLayer = std::round(layer);

        if(currLayer < 0) {
            currLayer = 0;
        } else if (currLayer >= fileList.size()) {
            currLayer = fileList.size() - 1;
        }

        QString currFile = imgDir.absolutePath().append("/").append(fileList.at(currLayer));

        QPixmap pix = QPixmap(currFile);

        emit sig_send_image(pix);
        update();

    }

}


void MyGL::slot_on_color_checkbox_changed(bool col)
{
    isColorEnabled = col;
    update();
}


void MyGL::slot_on_isolevel_changed(double iso)
{
    isolevel = iso;
}

void MyGL::slot_on_opacity_checkbox_changed(bool opa)
{
    isOpacityEnabled = opa;
    update();
}

void MyGL::slot_on_text_changed(QString s)
{
    newFileName = "/Users/chloebrownsnyder/Desktop/Spring2017/SeniorDesign/preloaded_volumes/" + s;
}


void MyGL::slot_on_loadMesh_clicked()
{
    // allow user to select a file
    // read through the file to get the vertices and indices
    // set these verts and ints for currChunk
    // create and draw

    chunks.clear();

    CChunk* allLayerChunk = new CChunk(this);

    std::vector<glm::vec4>* vertices = new std::vector<glm::vec4>();
    std::vector<GLuint>* indices = new std::vector<GLuint>();

    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/Users/chloebrownsnyder/Desktop/Spring2017/SeniorDesign/preloaded_volumes/",
                                                    tr("Text Files (*.txt)"));

    if(!filename.isNull() && !filename.isEmpty())
    {
        QFile file(filename);
        bool isReadingVerts = false;
        bool isReadingIndices = false;

        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);

            while(!in.atEnd())
            {
                QString line = in.readLine();

                if(isReadingVerts)
                {
                    if (line.compare("i") == 0)
                    {
                        isReadingVerts = false;
                        isReadingIndices = true;
                    } else {

                        //split the line around commas and create a new vec4
                        //push back into vertices vector
                        QStringList fields = line.split(",");
                        float x = fields.at(0).toFloat();
                        float y = fields.at(1).toFloat();
                        float z = fields.at(2).toFloat();
                        float a = fields.at(3).toFloat();

                        glm::vec4 newVert = glm::vec4(x, y, z, a);
                        vertices->push_back(newVert);

                    }
                } else if (isReadingIndices)
                {
                    unsigned int uint = line.toUInt();
                    GLuint newGLuint = uint;
                    indices->push_back(newGLuint);
                }

                // toggles reading behavior for verts and indices
                if(line.compare("v") == 0)
                {
                    isReadingVerts = true;
                    isReadingIndices = false;
                } else if (!isReadingIndices && !isReadingVerts){
                    // first line is the file path, second is the max layers
                    if(line.contains("/")) {
                        ctScanFilePath = line;
                    } else {
                        bool ok;
                        maxLayers = line.toInt(&ok, 10);
                        emit sig_send_max_layers(maxLayers);
                    }
                }
            }


            file.close();

            allLayerChunk->setVertices(vertices);
            allLayerChunk->setIndices(indices);
            allLayerChunk->setCtScanFilePath(ctScanFilePath);
            allLayerChunk->create();
            chunks.push_back(allLayerChunk);
            update();
        }
    }

}

void MyGL::slot_on_newMesh_clicked()
{
    // allow user to select a directory, set this in voxelizer
    // run the program as normal, but at end, export the indices and vertices to a file

    chunks.clear();

    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        tr("Open Directory"),
                                                        "/Users/chloebrownsnyder/Desktop/Spring2017/SeniorDesign/CTScanImages/",
                                                        QFileDialog::ShowDirsOnly);
    if(!dirName.isNull() && !dirName.isEmpty()){
        ctScanFilePath = dirName;
        mVoxelizer = CVoxelizer();
        mVoxelizer.setTargetDirPath(dirName);
        mVoxelizer.setDensityThreshold(densityThreshold);
        processFiles();
        emit sig_send_max_layers(maxLayers);
        update();
    }
}


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent), prog_lambert(this), prog_wire(this), prog_color(this), squareplane(this), camera(Camera())
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
    // Create and set up the heat map shader
    prog_color.create(":/glsl/colormap.vert.glsl", ":/glsl/colormap.frag.glsl");

    squareplane.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

}

void MyGL::processFiles() {

    mVoxelizer.processFiles();
#ifndef MULTITHREADING
    createChunkVector();
#else
    createChunkVectorMT();
#endif
}

void MyGL::resizeGL(int w, int h)
{
//    camera = Camera(w, h, glm::vec3(256, 100, 265), glm::vec3(255, 0, 260), glm::vec3(0, 1, 0));//Camera(w, h);
    // camera = Camera(w, h, glm::vec3(0, 2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));//Camera(w, h);
    camera = Camera(w, h, glm::vec3(-256, 200, 265), glm::vec3(-255, 0, 260), glm::vec3(0, 1, 0));//Camera(w, h);



    glm::mat4 viewproj = camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_wire.setViewProjMatrix(viewproj);
    prog_color.setViewProjMatrix(viewproj);

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
    prog_color.setViewProjMatrix(camera.getViewProj());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(-1, 1, 1));
    prog_lambert.setModelMatrix(model);
    prog_color.setModelMatrix(model);
    prog_wire.setModelMatrix(model);

    glm::mat4 translate = glm::translate(model, glm::vec3(0, currLayer, 0));

    prog_lambert.setModelMatrix(translate);

    prog_lambert.draw(squareplane);

    prog_lambert.setModelMatrix(model);

    if(isOpacityEnabled){
        // PLAY WITH BLEND FUNC? GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE and GL_SAMPLE_COVERAGE?
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    for(unsigned int i = 0; i < chunks.size(); i++) {
        CChunk* currChunk = chunks[i];
        if(isColorEnabled){
            prog_color.draw(*currChunk);
        } else {
            prog_lambert.draw(*currChunk);
        }
    }

}

void MyGL::createChunkVectorMT()
{
    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    std::vector<CCreateWorldAndChunkTask*>* chunkTasks = new std::vector<CCreateWorldAndChunkTask*>();

    int totalLayers = 20; //allLayers->size();
    int numThreads = 2;
    int incr = totalLayers / numThreads;

    int layer;
    int curr_ymin;
    int curr_ymax;

    // build the world. All threads will read from this world
    // go through each layer, and voxelize that layer's voxel plane
    for(int i = 0; i < totalLayers; i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);
        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld.createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }


    // now, allocate the memory for each chunk
    for(layer = 0; layer < totalLayers; layer += incr)
    {
        curr_ymin = layer;
        curr_ymax = layer + incr;
        if(curr_ymax > totalLayers) curr_ymax = totalLayers;

        CChunk* currChunk = new CChunk(this);
        currChunk ->setXMin(0);
        currChunk ->setXMax(512);
        currChunk ->setYMin(curr_ymin);
        currChunk ->setYMax(curr_ymax);
        currChunk ->setZMin(0);
        currChunk ->setZMax(512);
        currChunk ->setIsolevel(isolevel);

        std::vector<glm::vec4>* vertices = new std::vector<glm::vec4>();
        std::vector<GLuint>* indices = new std::vector<GLuint>();
        currChunk->setVertices(vertices);
        currChunk->setIndices(indices);

        // the different chunks all read from the same world but
        // this shouldn't cause a race condition because
        // reading from a map used .at() which is const?
        currChunk->setWorld(&mWorld);

        chunks.push_back(currChunk);
    }

    // create the thread for each chunk
    for(int i = 0; i < chunks.size(); i++)
    {

        CChunk* chunk = chunks.at(i);

        int ymin = chunk->getYMin();
        int ymax = chunk->getYMax();

        // get a vector that copies the data from allLayers.at(ymin) to allLAyers.at(ymax)
        // this is newly allocated memory, so multiple threads should never be reading from this
        // basically, in this sequential part of my code, I am allocating distinct portions of memory
        // then passing pointers to these places in memories, so multiple threads can operate on multiple places
        // i am trading space for time!
        std::vector<std::vector<CVoxel*>*>* layers = new std::vector<std::vector<CVoxel*>*>(allLayers->begin() + ymin, allLayers->begin() + ymax);

        // it is ok that this is an obejct and
        //that this data will be popped off the stack because it'll be copied into world which is maintained from here in a pointer
        // so the data will be preserved
        CCreateWorldAndChunkTask* thread = new CCreateWorldAndChunkTask(chunk, layers);
        chunkTasks->push_back(thread);
    }

    // QThreadPool::globalInstance()->setMaxThreadCount(1);

    // now actually run each thread
    for(CCreateWorldAndChunkTask* task : *chunkTasks)
    {
        QThreadPool::globalInstance()->start(task);
    }

    // create each chunk
    if(QThreadPool::globalInstance()->waitForDone()){
        for(int c = 0; c < chunks.size(); c++)
        {
            chunks.at(c)->create();
        }
    }
}

void MyGL::createChunkVector()
{


    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    CChunk* allLayerChunk = new CChunk(this);
    allLayerChunk ->setXMin(0);
    allLayerChunk ->setXMax(512);
    allLayerChunk ->setYMin(0);
    allLayerChunk ->setYMax(allLayers->size());
    allLayerChunk ->setZMin(0);
    allLayerChunk ->setZMax(512);

    allLayerChunk->setIsolevel(isolevel);

    std::vector<glm::vec4>* vertices = new std::vector<glm::vec4>();
    std::vector<GLuint>* indices = new std::vector<GLuint>();
    allLayerChunk->setVertices(vertices);
    allLayerChunk->setIndices(indices);

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

    allLayerChunk->createVoxelBuffer();
    allLayerChunk->create();
    chunks.push_back(allLayerChunk);

    // after the chunk has been created, save the verts and indices to a file
    // so they can be used
    allLayerChunk->setNewFileName(newFileName);
    allLayerChunk->setCtScanFilePath(ctScanFilePath);
    allLayerChunk->exportVerticesAndIndicesToFile();
    maxLayers = allLayers->size();

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

