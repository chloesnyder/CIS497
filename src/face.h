#ifndef FACE_H
#define FACE_H
#include "halfedge.h"
#include <include/glm/glm.hpp>

using namespace glm;

class HalfEdge;

class Face
{
private:
    HalfEdge* start_edge;
    vec4 color;
    int id;
public:
    Face();
    Face(HalfEdge* e, vec4 c, int i);

    void setStartEdge(HalfEdge &e);
    void setColor(vec4 &c);
    void setID(int i);

    HalfEdge* getStartEdge();
    vec4 getColor();
    int getID();
};

#endif // FACE_H
