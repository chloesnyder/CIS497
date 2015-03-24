#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <glwidget277.h>
#include <la.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include <scene/camera.h>
#include <la.h>
#include "mesh.h"

#include <QListWidget>

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    QOpenGLVertexArrayObject vao;

    Cylinder geom_cylinder;
    Sphere geom_sphere;
    ShaderProgram prog_lambert;
    ShaderProgram prog_wire;

    Mesh mesh;
    HalfEdge edge;

    Camera camera;

public:

    Vertex* selectedVertex;
    Face* selectedFace;
    HalfEdge* selectedEdge;

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sig_SendFaceList(QListWidgetItem*);
    void sig_sendEdgeList(QListWidgetItem*);
private slots:
    void slot_ReceiveFaceList(QListWidgetItem*);
    void slot_ReceiveEdgeList(QListWidgetItem*);
};
