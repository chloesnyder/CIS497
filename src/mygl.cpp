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

    mesh.createCube();
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
    if(selectedEdge != NULL) {
        emit sig_sendNextEdge(selectedEdge->getNext());
        emit sig_sendFace(selectedEdge->getFace());
        emit sig_sendSymEdge(selectedEdge->getSym());
        emit sig_sendVert(selectedEdge->getVert());
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

void MyGL::SelectNextHE() {
    if (selectedEdge != NULL) {
        selectedEdge = selectedEdge->getNext();
        update();
    }
}

void MyGL::SelectSymHE() {
    if (selectedEdge != NULL) {
        selectedEdge = selectedEdge->getSym();
        update();
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
    } else if(e->key() == Qt::Key_N){
        //make currently selected HE become its next
        SelectNextHE();
    } else if(e->key() == Qt::Key_S){
        //become its sym
        SelectSymHE();
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
        selectedVertex = mesh.addVertex(selectedEdge);

        emit sig_SendVertList(mesh.v_list.at(total_vertices));
        emit sig_sendEdgeList(mesh.HE_list.at(total_edges));
        emit sig_sendEdgeList(mesh.HE_list.at(total_edges+1));
        update();
    }
}


void MyGL::slot_triangulate() {
    if(selectedFace != NULL) {
        total_edges = mesh.HE_list.size();
        total_faces = mesh.f_list.size();
        selectedFace = mesh.triangulate(selectedFace);
        if(selectedFace != NULL) {
            mesh.selectedFace = selectedFace;

            emit sig_SendFaceList(mesh.f_list.at(total_faces));
            emit sig_sendEdgeList(mesh.HE_list.at(total_edges));
            emit sig_sendEdgeList(mesh.HE_list.at(total_edges+1));
            update();
        }
    }
}

void MyGL::slot_deleteVertex(){
    if(selectedVertex != NULL) {
        QList<HalfEdge*> to_remove_from_he_list;
        QList<Vertex*> to_remove_from_v_list;
        mesh.deleteVertex(selectedVertex, to_remove_from_he_list, to_remove_from_v_list);

        for(HalfEdge* e : to_remove_from_he_list){
            emit sig_removeHEList(e);
            for(int i = 0; i < mesh.HE_list.size(); i++) {
                 mesh.HE_list.erase(mesh.HE_list.begin() + i);

                 --mesh.max_edge_id;
            }
            delete e;
        }
        for(Vertex* v : to_remove_from_v_list){
            emit sig_removeVertList(v);
        }


        selectedVertex = NULL;
        selectedEdge = NULL;
        update();

    }
}

void MyGL::slot_receiveR(double r){
    if(selectedFace != NULL) {
        vec4 new_col = selectedFace->getColor();
        new_col = vec4(r, new_col.y, new_col.z, 1);
        selectedFace->setColor(new_col);
        update();
    }
}

void MyGL::slot_receiveG(double g){
    if(selectedFace != NULL) {
        vec4 new_col = selectedFace->getColor();
        new_col = vec4(new_col.x, g, new_col.z, 1);
        selectedFace->setColor(new_col);
        update();
    }
}

void MyGL::slot_receiveB(double b){
    if(selectedFace != NULL) {
        vec4 new_col = selectedFace->getColor();
        new_col = vec4(new_col.x, new_col.y, b, 1);
        selectedFace->setColor(new_col);
        update();
    }
}

void MyGL::slot_receiveX(double x){
    if(selectedVertex != NULL) {
        vec4 new_pos = selectedVertex->getPos();
        new_pos = vec4(x, new_pos.y, new_pos.z, 1);
        selectedVertex->setPos(new_pos);
        selectedVertex->create();
        update();
    }
}

void MyGL::slot_receiveY(double y) {
    if(selectedVertex != NULL) {
        vec4 new_pos = selectedVertex->getPos();
        new_pos = vec4(new_pos.x, y, new_pos.z, 1);
        selectedVertex->setPos(new_pos);
        selectedVertex->create();
        update();
    }
}

void MyGL::slot_receiveZ(double z){
    if(selectedVertex != NULL) {
        vec4 new_pos = selectedVertex->getPos();
        new_pos = vec4(new_pos.x, new_pos.y, z, 1);
        selectedVertex->setPos(new_pos);
        selectedVertex->create();
        update();
    }
}

