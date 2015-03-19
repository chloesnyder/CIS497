#ifndef MESH_H
#define MESH_H
#include "shaderprogram.h"
#include "face.h"
#include "halfedge.h"
#include "vertex.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

using namespace std;
class Mesh : public ShaderProgram::Drawable
{
private:
    int count;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol; // Can be used to pass per-vertex color information to the shader, but is currently unused.



public:
    Mesh();

    list<Face*> f_list;
    list<HalfEdge*> HE_list;
    list<Vertex*> v_list;

    static vec4 crossVec4(vec4 _v1, vec4 _v2);

    void create();
    void destroy();

    void createSquare();

    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();
};

#endif // MESH_H
