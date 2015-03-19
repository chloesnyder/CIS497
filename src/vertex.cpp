#include "vertex.h"

Vertex::Vertex()
{
}

//Vertex::Vertex(vec4 p, HalfEdge *e, int i){
//    pos = p;
//    edge = e;
//    id = i;
//}

void Vertex::setPos(vec4 &p){
    pos = p;
}

void Vertex::setEdge(HalfEdge &e){
    edge = &e;
}

void Vertex::setID(int i){
    id = i;
}

vec4 Vertex::getPos() {
    return pos;
}

HalfEdge *Vertex::getEdge(){
    return edge;
}

int Vertex::getID(){
    return id;
}
