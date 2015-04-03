#ifndef VERTEX_H
#define VERTEX_H
#include "halfedge.h"
#include <la.h>
#include <QListWidgetItem>
#include <shaderprogram.h>


using namespace glm;

class HalfEdge;

class Vertex : public QListWidgetItem, public ShaderProgram::Drawable
{
private:
    vec4 pos;
    HalfEdge* edge;
    int id;

    QString name;

    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol;

public:
    Vertex();
    ~Vertex();

    void setPos(const vec4 &p);
    void setEdge(HalfEdge *e);
    void setID(int i);

    vec4 getPos();
    HalfEdge* getEdge();
    int getID();

    void create();
    void destroy();
    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();

};

#endif // VERTEX_H
