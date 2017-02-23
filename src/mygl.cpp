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

#include <QDir>
#include <QDirIterator>
#include <QStringList>

//#define OLD_CAMERA;
//#define ONESLICE

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
   // glEnable(GL_LINE_SMOOTH);
   // glEnable(GL_POLYGON_SMOOTH);


    // PLAY WITH BLEND FUNC? GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE and GL_SAMPLE_COVERAGE?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // DO I WANT THIS?
   /* glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);*/


    /// TRIANGLE LINES
   // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  //  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);


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
    mImageReader = CImageReader();

    processFiles();
}

void MyGL::processFiles() {

#ifdef ONESLICE
    /// TODO: In future, this needs to iterate over multiple file paths to get multiple images
    // For now, only 1 image array, with height 0
    const char* filepath = "/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/RabbitLayer1.ppm";
    mImageReader.readPPM(filepath);
    img_t* img = mImageReader.getImageArray();
    mVoxelizer = Voxelizer(img, 0);
    mVoxelizer.voxelizeImageSlice();
    createChunkVector();
#else
    std::vector<img_t*> slices;

    QDir targetDir = QDir("/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/PPMS/firsthalf/firstquarter/firsteighth");
   // QDir targetDir = QDir("/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/one");
    QStringList qsl; qsl.append("*.ppm");
    targetDir.setNameFilters(qsl);

    QDirIterator it(targetDir, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        const char* currFile = it.next().toStdString().c_str();
        mImageReader.readPPM(currFile);
        img_t* currImg = mImageReader.getImageArray();
        slices.push_back(currImg);
    }

    mVoxelizer = Voxelizer(&slices);
    mVoxelizer.voxelizeAllImages();
    createChunkVector();

#endif

}

void MyGL::resizeGL(int w, int h)
{
    camera = Camera(w, h, glm::vec3(256, 100, 1000), glm::vec3(256, 0, 256), glm::vec3(0, 1, 0));//Camera(w, h);

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

void MyGL::createChunkVector()
{

    //  std::vector<CVoxel*> *voxelPlane = mVoxelizer.getVoxelPlane();
    // double length = mVoxelizer.getLength();

    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    CChunk* allLayerChunk = new CChunk(this);
    allLayerChunk ->setXMin(0);
    allLayerChunk ->setXMax(512);
    allLayerChunk ->setYMin(0);
    allLayerChunk ->setYMax(allLayers->size());
    allLayerChunk ->setZMin(0);
    allLayerChunk ->setZMax(512);

    // go through each layer, and voxelize that layer's voxel plane
    for(int i = 0; i < allLayers->size(); i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);
        allLayerChunk ->setWorld(&mWorld);

        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();

            mWorld.createChunkVoxelData(voxPos, voxCol);
        }
    }

  //  allLayerChunk->setCameraForward(glm::vec4(camera.look, 0));
    allLayerChunk->create();
    chunks.push_back(allLayerChunk);

}


void MyGL::keyPressEvent(QKeyEvent *e)
{
#ifdef OLD_CAMERA
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        camera.theta += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Left) {
        camera.theta -= 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Up) {
        camera.phi -= 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Down) {
        camera.phi += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_I) {
        camera.zoom -= 0.5f;
    } else if (e->key() == Qt::Key_O) {
        camera.zoom += 0.5f;
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= 5.0f * DEG2RAD;
    }
    camera.RecomputeEye();
    update();  // Calls paintGL, among other things*/
#else
    float amount = 2.0f;
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
#endif
}

