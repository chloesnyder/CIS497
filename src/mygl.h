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
#include "ccreateachunktask.h"

#include "ccreateworldandchunk.h"

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

    CVoxelizer mVoxelizer;
    CWorld mWorld;

    std::vector<CChunk*> chunks = std::vector<CChunk*>();
    std::vector<CWorld*> worlds = std::vector<CWorld*>();
    std::vector<std::vector<CVoxel*>*>* layers = new std::vector<std::vector<CVoxel*>*>();

    std::vector<glm::vec4>* vertices = new std::vector<glm::vec4>();
    std::vector<GLuint>* indices = new std::vector<GLuint>();

    CChunk* theChunk;


    //bounds
    int boundNegX, boundNegZ, boundPosZ, boundPosX;
    int boundWorldX, boundWorldZ;

    void createChunkVector();
    void processFiles();

    bool initTextures3d();

    void threadCheck(std::vector<CCreateAChunkTask*> *chunkTasks);


public:

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();


protected:
    void keyPressEvent(QKeyEvent *e);

signals:



};
