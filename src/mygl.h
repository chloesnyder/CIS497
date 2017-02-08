#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <glwidget277.h>
#include <la.h>
#include <shaderprogram.h>
#include <277files/scene/cylinder.h>
#include <277files/scene/sphere.h>
#include <277files/scene/camera.h>
#include <la.h>
#include "277files/mesh.h"


#include <QListWidget>

class Mesh;

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    QOpenGLVertexArrayObject vao;
    ShaderProgram prog_lambert;
    ShaderProgram prog_wire;

    Mesh mesh;
    Camera camera;

public:

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:


private slots:
    void slot_importDICOMFile();

};
