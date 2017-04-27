#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QApplication>
#include <QKeyEvent>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QDirIterator>
#include <QThreadPool>
#include <QListWidget>
#include <QElapsedTimer>

#include <la.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cstring>
#include <glwidget277.h>
#include <shaderprogram.h>
#include <277files/scene/camera.h>


#include "cchunk.h"
#include "cworld.h"
#include "cvoxel.h"
#include "voxelizer.h"
#include "squareplane.h"
#include "cchunktask.h"
#include "cworldarray.h"
#include "cworldtask.h"

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:

    glm::vec3 focusEye = glm::vec3(-237.499, 413.619, 959.962);
    glm::vec3 focusRef = glm::vec3(-243.461, 331.182, 777.767);
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    int focusWidth;
    int focusHeight;

    ShaderProgram prog_lambert;
    ShaderProgram prog_wire;
    ShaderProgram prog_color;

    GLuint vao;

    Camera camera;

    CVoxelizer mVoxelizer;

    CWorldArray* mWorldArr;

    SquarePlane squareplane;

    std::vector<CChunk*> chunks = std::vector<CChunk*>();

    QElapsedTimer timer;

    //bounds
    int boundNegX, boundNegZ, boundPosZ, boundPosX;
    int boundWorldX, boundWorldZ;

    void processFiles();

    bool isOpacityEnabled = false;
    bool isColorEnabled = false;

    double isolevel = .01; //default value so as much as possible draws

    QString ctScanFilePath = "";
    QString prevImage = "";

    int maxLayers;

    int currLayer = 0; // the slider determines this number, should show the image at this layer

    float densityThreshold = 0;

    int numThreads = 1;

    bool showPlane = false;

    int progress = 0;

    void progressFinishedBuildingWorld();
    void progressStartBuildingChunks();
    void progressFinishedBuildingChunks();
    void progressFinishCreatingChunks();

    // Camera control bools
    bool translateUp = false;
    bool translateDown = false;
    bool translateRight = false;
    bool translateLeft = false;
    bool translateForward = false;
    bool translateBackward = false;
    bool rotateRight = false;
    bool rotateLeft = false;
    bool rotateUp = false;
    bool rotateDown = false;
    bool shiftOperator = false;

    void onCameraMove();

     std::vector<CCreateChunkTask*>* chunkTasks = new std::vector<CCreateChunkTask*>();
     bool threadsDone = false;
     bool allDone = false;
     bool processStarted = false;
     bool isCreating = false;

public:

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void createChunkVectorMT();




protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

signals:
    void sig_send_max_layers(int max);
    void sig_send_image(QPixmap img);
    void sig_show_image();
    void sig_update_progress(int p);
    void sig_send_text(QString q);
    void sig_set_enabled(bool b);

public slots:
    void slot_on_newMesh_clicked();
    void slot_on_opacity_checkbox_changed(bool opa);
    void slot_on_color_checkbox_changed(bool col);
    void slot_on_slider_moved(int num);
    void slot_tissue_preset(int s);
    void slot_set_num_threads(int t);
    void slot_on_show_plane_changed(bool b);
    void slot_startLoading();


};
