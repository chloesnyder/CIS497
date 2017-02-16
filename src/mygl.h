#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <glwidget277.h>
#include <la.h>
#include <shaderprogram.h>
#include <277files/scene/camera.h>


#include "cchunk.h"
#include "cimagereader.h"
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

    GLuint vao;

    Camera camera;

    CImageReader mImageReader;
    Voxelizer mVoxelizer;
    CWorld mWorld;

    std::vector<CChunk*> chunks;



    //bounds
    int boundNegX, boundNegZ, boundPosZ, boundPosX;
    int boundWorldX, boundWorldZ;


public:

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

   // void drawVoxels();

    void createChunkVector();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:



};
