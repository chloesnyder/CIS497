#include "mygl.h"
#include <la.h>
#include<QFileDialog>
#include<QFile>
#include<QStringList>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

using namespace glm;



MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent)
{
    setFocusPolicy(Qt::ClickFocus);

}

MyGL::~MyGL()
{
    makeCurrent();

    vao.destroy();
    mesh.destroy();
    voxel.destroy();
}

void MyGL::drawVoxels() {
    ///TODO:
    // Go through each voxel plane in the chain of voxel planes (i.e. traverse parent ->child until
    // child is null)
    // Then, go through each voxel in the vector of voxesl (the plane)
    // Transform each voxel as appropriate and draw

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
    voxel.create();


    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl", ":/glsl/wire.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();

    // Read in the test image
    mImageReader = CImageReader();
    /// TODO: In future, this needs to iterate over multiple file paths to get multiple images
    // For now, only 1 image array, with height 0
    const char* filepath = "/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/CIS497_SD/testMore.ppm";
    mImageReader.readPPM(filepath);
    img_t* img = mImageReader.getImageArray();
    mVoxelizer = Voxelizer(img, 0);
    mVoxelizer.voxelizeImageSlice();

    // The node is at (0, 0, 0) with no rotation, scale of 1
    mVoxelNode = new CVoxelPlaneNode(0, 0, 0, 0, 0, 0, 1, 1, 1);
    mVoxelNode->setVoxelPlane(mVoxelizer.getVoxelPlane());
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

    voxel.destroy();
    voxel.create();

    mat4 model = mat4(1.0f);
    prog_lambert.setModelMatrix(model);
    prog_lambert.draw(*this, mesh);

    std::vector<Voxel*> *voxelPlane = mVoxelNode->getVoxelPlane();
    Voxel* vPrev = nullptr;
    int count = 0;
    for(Voxel* v : *voxelPlane) {
        //if(count != 0){
        mat4 trans = mVoxelNode->getTransformForVoxel(v, vPrev);
        voxel.setColor(v->getColor());
        prog_lambert.setModelMatrix(trans);
        //prog_lambert.draw(*this, *v);

        prog_lambert.draw(*this, voxel);
       // }
        vPrev = v;
        count++;
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
        camera.zoom -= 0.5f;
    } else if (e->key() == Qt::Key_O) {
        camera.zoom += 0.5f;
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= 5.0f * DEG2RAD;
    }
    camera.RecomputeEye();
    update();  // Calls paintGL, among other things
}


void MyGL::slot_importDICOMFile(){


    //ASSSUMES WELL FORMED
    //QString filename = QString("/Users/chloesnyder/Desktop/CIS277/hw06/objs/cube.obj");//QFileDialog::getOpenFileName(0, QString("Load OBJ"), QString("../"), QString("*.obj"));
    QString filename = QFileDialog::getOpenFileName(0, QString("Load OBJ"), QString("../"), QString("*.obj"));
    QFile* file = new QFile(filename);
    file->open(QIODevice::ReadOnly);
    QString line;

    QList<Vertex*> vertices;

    QMap<Face*, QList<int>> faces_and_verts;

    if(file->exists()) {
        QTextStream stream(file);
        line = stream.readLine();
        do {
            if(line.startsWith("v ")) {
                //split the line up, read in the vector positions, add to list
                QStringList positions = line.split(" ");
                vec4 v_pos = vec4(0,0,0,1);
                //skip 0 because 0 will be "v"
                v_pos.x = positions[1].toFloat();
                v_pos.y = positions[2].toFloat();
                v_pos.z = positions[3].toFloat();

                //make new vertex
                Vertex* v = new Vertex();
                v->setPos(v_pos);
                v->setID(++mesh.max_vert_id);
                vertices.push_back(v);
                mesh.v_list.push_back(v);

            } else if (line.startsWith("f")) {
                Face* f = new Face();
                f->setID(++mesh.max_face_id);
                float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                vec4 color = vec4(r1, r2, r3, 1);
                f->setColor(color);
                mesh.f_list.push_back(f);

                //parse to get the first number of each word
                //this is a vertex
                QStringList info = line.split(" ");
                QList<int> face_vert_info;
                //get each vertex belonging to a single face
                for(int i = 1; i < info.size(); i++) {
                    //split each word up with the slash being the delimiter
                    QStringList numbers = info[i].split("/");
                    //the first char in the word is the vertex
                    int vert = numbers[0].toInt() - 1;
                    face_vert_info.append(vert);
                }
                //map the face to its list of vertex info
                faces_and_verts.insert(f, face_vert_info);

            }
            line = stream.readLine();
        } while (!line.isNull());
    }

    file->close();


    mesh.createFromFile(vertices, faces_and_verts);
    mesh.create();
    mesh.selectedFace = NULL;

    update();
}
