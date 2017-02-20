#include "mygl.h"
#include <la.h>
#include<QFileDialog>
#include<QFile>
#include<QStringList>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

using namespace glm;

//#define OLD_CAMERA;

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
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

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
    /// TODO: In future, this needs to iterate over multiple file paths to get multiple images
    // For now, only 1 image array, with height 0
    const char* filepath = "/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/colorseg.ppm";
    mImageReader.readPPM(filepath);
    img_t* img = mImageReader.getImageArray();
    mVoxelizer = Voxelizer(img, 0);
    mVoxelizer.voxelizeImageSlice();

    createChunkVector();
}

void MyGL::resizeGL(int w, int h)
{
    camera = Camera(w, h);

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

    mat4 model = mat4(1.0f);
    prog_lambert.setModelMatrix(model);

    for(unsigned int i = 0; i < chunks.size(); i++) {
        CChunk* currChunk = chunks[i];
        prog_lambert.draw(*currChunk);
    }
}

void MyGL::createChunkVector()
{
    chunks = std::vector<CChunk*>();
    std::vector<CVoxel*> *voxelPlane = mVoxelizer.getVoxelPlane();
    float chunkLength = 512.0f;

    CChunk* currChunk = new CChunk(this);
    currChunk->setXMin(0);
    currChunk->setXMax(512);
    currChunk->setYMin(0);
    currChunk->setYMax(1);
    currChunk->setZMin(0);
    currChunk->setZMax(512);

    CVoxel* prevV = nullptr;
    currChunk->setWorld(&mWorld);
    // Eventually this will be modified to do every image?
    // Right now: max height of 1 because only one image
    // In future: make the y height = # of images, make it all be in one chunk
    for(CVoxel* v : *voxelPlane) {

        glm::vec4 voxPos = v->getPosition();
        glm::vec4 voxCol = v->getColor();

      //  mWorld.addVoxelAt(voxPos.x, voxPos.y, voxPos.z, CVoxel::TYPE);

        // To modify: pass in the voxCol to createChunkVoxelData
        // modify this to take in a vec4?
        // also get rid of the prev->to drawing? might be why I'm getting double square plane
        if(prevV != nullptr)
        {
            glm::vec4 prevPos = prevV->getPosition();
            mWorld.createChunkVoxelData(voxPos.x, prevPos.x,
                                        voxPos.y, prevPos.y,
                                        voxPos.z, prevPos.z, voxCol);
        } else {
            mWorld.createChunkVoxelData(voxPos.x, voxPos.x,
                                        voxPos.y, voxPos.y,
                                        voxPos.z, voxPos.z, voxCol);
        }

        // TEST TO SEE IF GIT WORKS


        prevV = v;

    }

    currChunk->create();
    chunks.push_back(currChunk);

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

