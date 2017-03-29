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

void MyGL::slot_on_slider_moved(int num)
{
    // figure out the "percentage" of the way up it is
    float percent = (float) num / 100.0f;
    float layer = maxLayers * percent;
    currLayer = std::round(layer);
    currLayer = glm::clamp(currLayer, 0, maxLayers - 1);


    // If there are image files associated with this, display the image on the slider
    if(!ctScanFilePath.isEmpty())
    {
        QDir imgDir = QDir(ctScanFilePath);
        QStringList qsl; qsl.append("*.ppm");
        imgDir.setNameFilters(qsl);

        QDirIterator it(imgDir, QDirIterator::Subdirectories);
        QStringList fileList = imgDir.entryList();
        QString currFile = imgDir.absolutePath().append("/").append(fileList.at(currLayer));


        QPixmap pix = QPixmap(currFile);

        emit sig_send_image(pix);
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

void MyGL::slot_on_red_changed(bool red)
{
    isRedOnly = red;
    update();
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
    }

    file.close();

    allLayerChunk->setVertices(vertices);
    allLayerChunk->setIndices(indices);
    allLayerChunk->setCtScanFilePath(ctScanFilePath);
    allLayerChunk->create();
    chunks.push_back(allLayerChunk);
    update();

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
    ctScanFilePath = dirName;
    mVoxelizer = CVoxelizer();
    mVoxelizer.setTargetDirPath(dirName);
    processFiles();
    update();
}


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent), prog_lambert(this), prog_wire(this), prog_color(this), prog_red(this), mImageScroller(this), camera(Camera())
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
    prog_red.create(":/glsl/red_only.vert.glsl", ":/glsl/red_only.frag.glsl");

    mImageScroller.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);



}

void MyGL::processFiles() {

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
    prog_color.setViewProjMatrix(viewproj);
    prog_red.setViewProjMatrix(viewproj);

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
    prog_red.setViewProjMatrix(camera.getViewProj());

    glm::mat4 model = glm::mat4(1.0f);
    prog_lambert.setModelMatrix(model);
    prog_color.setModelMatrix(model);
    prog_wire.setModelMatrix(model);
    prog_red.setModelMatrix(model);

    if(isOpacityEnabled || isRedOnly){
        // PLAY WITH BLEND FUNC? GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE and GL_SAMPLE_COVERAGE?
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    if(isRedOnly)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHT0);
        GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }

    // prog_wire.draw(mImageScroller);

    for(unsigned int i = 0; i < chunks.size(); i++) {
        CChunk* currChunk = chunks[i];
        if(isColorEnabled){
            prog_color.draw(*currChunk);
        } else if(isRedOnly){
            prog_red.draw(*currChunk);
        } else {
            prog_lambert.draw(*currChunk);
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

