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

class Mesh;

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
    Camera camera;

public:
    int total_vertices;
    int total_edges;
    int total_faces;

    void SelectNextHE();
    void SelectSymHE();

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
    void sig_SendVertList(QListWidgetItem*);

    void sig_removeHEList(QListWidgetItem*);
    void sig_removeVertList(QListWidgetItem*);

    void sig_sendNextEdge(QListWidgetItem*);
    void sig_sendFace(QListWidgetItem*);
    void sig_sendSymEdge(QListWidgetItem*);
    void sig_sendVert(QListWidgetItem*);

private slots:
    void slot_ReceiveFaceList(QListWidgetItem*);
    void slot_ReceiveEdgeList(QListWidgetItem*);
    void slot_ReceiveVertList(QListWidgetItem*v);
    void slot_addVertex();
    void slot_triangulate();
    void slot_deleteVertex();

    void slot_receiveR(double r);
    void slot_receiveG(double g);
    void slot_receiveB(double b);
    void slot_receiveX(double x);
    void slot_receiveY(double y);
    void slot_receiveZ(double z);

    void slot_CatmullClark();
};
