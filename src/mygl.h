#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <glwidget277.h>
#include <la.h>
#include <shaderprogram.h>
#include <277files/scene/camera.h>


#include "cchunk.h"
//#include "cimagereader.h"
#include "cworld.h"
#include "cvoxel.h"
#include "voxelizer.h"

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

    std::vector<CChunk*> chunks = std::vector<CChunk*>();

    QString newFileName;

    //bounds
    int boundNegX, boundNegZ, boundPosZ, boundPosX;
    int boundWorldX, boundWorldZ;

    void createChunkVector();
    void processFiles();

    bool isOpacityEnabled = false;
    double isolevel = .01; //default value so as much as possible draws


public:

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();


protected:
    void keyPressEvent(QKeyEvent *e);

signals:

public slots:
    void slot_on_loadMesh_clicked();
    void slot_on_newMesh_clicked();
    void slot_on_text_changed(QString s);
    void slot_on_opacity_checkbox_changed(bool opa);
    void slot_on_isolevel_changed(double iso);



};
