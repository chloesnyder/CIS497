#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "face.h"
#include "vertex.h"
#include <QListWidgetItem>
#include <shaderprogram.h>

using namespace glm;
class Face;
class Vertex;

class HalfEdge : public QListWidgetItem, public ShaderProgram::Drawable
{
private:

    Face* face;
    Vertex* vert;
    HalfEdge* next;
    HalfEdge* sym;
    int id;
    vec4 color;
    QString name;

    int count;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol;

    void createEdgeVertexIndices(GLuint he_idx[2], vec4 he_vert_pos[2],  vec4 he_vert_col[2]);

public:

    HalfEdge* selectedEdge;

    HalfEdge();
    HalfEdge(Face* f, Vertex* v, HalfEdge* n, HalfEdge* s, int i);
    ~HalfEdge();

    void setFace(Face *f);
    void setVert(Vertex *v);
    void setNext(HalfEdge *n);
    void setSym(HalfEdge *s);
    void setID(int i);
    void setColor(vec4 c);

    Face* getFace();
    Vertex* getVert();
    HalfEdge* getNext();
    HalfEdge* getSym();
    vec4 getColor();
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

#endif // HALFEDGE_H
