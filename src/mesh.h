#ifndef MESH_H
#define MESH_H
#include "shaderprogram.h"
#include "face.h"
#include "halfedge.h"
#include "vertex.h"
#include <QList>
#include <QMap>
//#include "mygl.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

using namespace std;
using namespace glm;

//class MyGL;

class Mesh : public ShaderProgram::Drawable
{
private:
    int count;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol; // Can be used to pass per-vertex color information to the shader, but is currently unused.

    void createMeshVertexPositionsNormalsIndices(vector<vec4>& mesh_vert_pos, vector<vec4>& mesh_vert_nor, vector<vec4>& mesh_vert_col, vector<GLuint>& mesh_idx, vector<Face*>& faces);

public:
    Mesh();

    static int max_vert_id;
    static int max_edge_id;
    static int max_face_id;

    QMap<Face*, Vertex*> centroid_map;
    QMap<HalfEdge*, Vertex*> midpoint_map;

    Vertex* selectedVertex;
    Face* selectedFace;
    HalfEdge* selectedEdge;

    vector<Face*> f_list;
    QList<HalfEdge*> HE_list;
    vector<Vertex*> v_list;

    static vec4 crossVec4(vec4 _v1, vec4 _v2);

    Vertex *addVertex(HalfEdge* HE1);
    Face *triangulate(Face* FACE1);
    void deleteVertex(Vertex* v, QList<HalfEdge *> &to_remove_from_he_list, QList<Vertex *> &to_remove_from_v_list);

    HalfEdge* getPrev(HalfEdge* e);


    void create();
    void destroy();

    void createSquare();
    void createCube();

    void createFromFile(const QList<Vertex*>& vertices, const QMap<Face *, QList<int> > &faces_and_verts);

    //subdivision
    void subdivide();
    Vertex* calculateCentroid(Face* f);
    Vertex* calculateMidpoint(HalfEdge* e);
    void quadrangulate(Face* f, Vertex* centroid, QMap<Face *, QList<HalfEdge *> > &f_to_mp, QMap<Face *, QList<HalfEdge *> > &f_to_orig_v);

    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();
};

#endif // MESH_H
