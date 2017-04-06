#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <glwidget277.h>
#include <la.h>
#include <shaderprogram.h>
#include <277files/scene/camera.h>


#include "cchunk.h"
#include "cworld.h"
#include "cvoxel.h"
#include "voxelizer.h"
#include "cube.h"
#include "ccreateworldandchunktask.h"

#include <QListWidget>



class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:

    ShaderProgram prog_lambert;
    ShaderProgram prog_wire;
    ShaderProgram prog_color;

    GLuint vao;

    Camera camera;

    CVoxelizer mVoxelizer;
    CWorld mWorld;

    SquarePlane squareplane;

    std::vector<CChunk*> chunks = std::vector<CChunk*>();

    QString newFileName;

    //bounds
    int boundNegX, boundNegZ, boundPosZ, boundPosX;
    int boundWorldX, boundWorldZ;

    void createChunkVector();
    void processFiles();

    bool isOpacityEnabled = false;
    bool isColorEnabled = false;

    double isolevel = .01; //default value so as much as possible draws

    QString ctScanFilePath = "";
    QString prevImage = "";

    int maxLayers;

    int currLayer = 0; // the slider determines this number, should show the image at this layer

    float densityThreshold = 0;

public:

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void createChunkVectorMT();


protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sig_send_max_layers(int max);
    void sig_send_image(QPixmap img);
    void sig_show_image();

public slots:
    void slot_on_loadMesh_clicked();
    void slot_on_newMesh_clicked();
    void slot_on_text_changed(QString s);
    void slot_on_opacity_checkbox_changed(bool opa);
    void slot_on_color_checkbox_changed(bool col);
    void slot_on_slider_moved(int num);
    void slot_tissue_preset(int s);


};
