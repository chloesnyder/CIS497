#include "halfedge.h"

HalfEdge::HalfEdge()
{
}

HalfEdge::HalfEdge(Face* f, Vertex* v, HalfEdge* n, HalfEdge* s, int i){
    face = f;
    vert = v;
    next = n;
    sym = s;
    id = i;
}

HalfEdge::~HalfEdge(){
    face = NULL;
    next = NULL;
    sym = NULL;
    Vertex v = *vert;
    vert = NULL;
    v.~Vertex();
}

void HalfEdge::setFace(Face *f){
    face = f;
}

void HalfEdge::setVert(Vertex *v){
    vert = v;
    v->setEdge(this);
}

void HalfEdge::setNext(HalfEdge* n){
    next = n;
}

void HalfEdge::setSym(HalfEdge *s){
    sym = s;
}

void HalfEdge::setID(int i){
    id = i;
}

Face* HalfEdge::getFace(){
    return face;
}

Vertex* HalfEdge::getVert(){
    return vert;
}

HalfEdge* HalfEdge::getNext(){
    return next;
}

HalfEdge* HalfEdge::getSym(){
    return sym;
}

int HalfEdge::getID(){
    return id;
}
