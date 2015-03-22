#ifndef VERTEX_H
#define VERTEX_H
#include "halfedge.h"
#include <la.h>

using namespace glm;

class HalfEdge;

class Vertex
{
private:
    vec4 pos;
    HalfEdge* edge;
    int id;

public:
    Vertex();
    ~Vertex();

    void setPos(vec4 &p);
    void setEdge(HalfEdge *e);
    void setID(int i);

    vec4 getPos();
    HalfEdge* getEdge();
    int getID();
};

#endif // VERTEX_H
