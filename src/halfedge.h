#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "face.h"
#include "vertex.h"
#include <include/glm/glm.hpp>

using namespace glm;
class Face;
class Vertex;

class HalfEdge
{
private:
    Face* face;
    Vertex* vert;
    HalfEdge* next;
    HalfEdge* sym;
    int id;

public:
    HalfEdge();
    HalfEdge(Face* f, Vertex* v, HalfEdge* n, HalfEdge* s, int i);

    void setFace(Face &f);
    void setVert(Vertex &v);
    void setNext(HalfEdge &n);
    void setSym(HalfEdge &s);
    void setID(int i);

    Face* getFace();
    Vertex* getVert();
    HalfEdge* getNext();
    HalfEdge* getSym();
    int getID();
};

#endif // HALFEDGE_H
