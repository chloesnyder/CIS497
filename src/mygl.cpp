#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

using namespace glm;



MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent)
{
    setFocusPolicy(Qt::ClickFocus);
    selectedFace = NULL;
    selectedEdge = NULL;
    selectedVertex = NULL;

}

MyGL::~MyGL()
{
    makeCurrent();

    vao.destroy();
    geom_cylinder.destroy();
    geom_sphere.destroy();

    mesh.destroy();
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
    vao.create();

    mesh.create();
    total_vertices = mesh.v_list.size();
    total_edges = mesh.HE_list.size();
    total_faces = mesh.f_list.size();


    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl", ":/glsl/wire.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();

    //face, edge, vertex lists
    for(int i = 0; i < total_faces; i++){
        emit sig_SendFaceList(mesh.f_list[i]);
    }
    for(int i = 0; i < total_edges; i++){
        emit sig_sendEdgeList(mesh.HE_list[i]);
    }
    for(int i = 0; i < total_vertices; i++){
        emit sig_SendVertList(mesh.v_list[i]);
    }

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

    mesh.v_list.erase(mesh.v_list.begin(), mesh.v_list.end());
    mesh.HE_list.erase(mesh.HE_list.begin(), mesh.HE_list.end());

    mesh.destroy();
    mesh.create();

    mat4 model = mat4(1.0f);//translate(mat4(1.0f), vec3(1, 0, 0)) * scale(mat4(1.0f), vec3(1,1,1));
    prog_lambert.setModelMatrix(model);
    prog_lambert.draw(*this, mesh);

    if(selectedEdge != NULL){
        if(!selectedEdge->bindPos()){
            selectedEdge->create();
        }
        //other stuff to set up shader vars
        glDisable(GL_DEPTH_TEST);
        prog_wire.setModelMatrix(model);
        prog_wire.draw(*this, *selectedEdge);
        glEnable(GL_DEPTH_TEST);
    }
    if(selectedVertex != NULL){
        if(!selectedVertex->bindPos()){
            selectedVertex->create();
        }
        //other stuff to set up shader vars
        glDisable(GL_DEPTH_TEST);
        prog_wire.setModelMatrix(model);
        prog_wire.draw(*this, *selectedVertex);
        glEnable(GL_DEPTH_TEST);

    }

}

void MyGL::keyPressEvent(QKeyEvent *e)
{
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
        camera.zoom -= 0.5f * DEG2RAD;
    } else if (e->key() == Qt::Key_O) {
        camera.zoom += 0.5f;
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += 5.0f;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= 5.0f * DEG2RAD;
    }
    camera.RecomputeEye();
    update();  // Calls paintGL, among other things
}

void MyGL::slot_ReceiveFaceList(QListWidgetItem * f){
    selectedFace = (Face*) f;
    mesh.selectedFace = selectedFace;
    update();
}

void MyGL::slot_ReceiveEdgeList(QListWidgetItem* e){
    selectedEdge = (HalfEdge*) e;
    update();
}

void MyGL::slot_ReceiveVertList(QListWidgetItem *v){
    selectedVertex = (Vertex*) v;
    update();
}

//Why are my numbers all weird? I call pushback in mesh but the size of the list doesn't seem to actually increase
void MyGL::slot_addVertex() {
    if(selectedEdge!= NULL) {
        total_vertices = mesh.v_list.size();
        total_edges = mesh.HE_list.size();
        selectedVertex = mesh.addVertex(selectedEdge, total_vertices, total_edges);

        update();
        emit sig_SendVertList(mesh.v_list.at(total_vertices));
        emit sig_sendEdgeList(mesh.HE_list.at(total_edges - 1));
        emit sig_sendEdgeList(mesh.HE_list.at(total_edges));


    }
}


//void MyGL::slot_triangulate() {
//    if(selectedFace != NULL) {
//        selectedFace = mesh.triangulate(selectedFace);
//        emit sig_SendFaceList(selectedFace);
//        emit sig_sendEdgeList(mesh.HE_list.at(mesh.HE_list.size()-2));
//        emit sig_sendEdgeList(mesh.HE_list.at(mesh.HE_list.size()-1));
//    }
//}

