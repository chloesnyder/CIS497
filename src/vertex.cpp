#include "vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex(){
    edge = NULL;
}

void Vertex::setPos(vec4 &p){
    pos = p;
}

void Vertex::setEdge(HalfEdge *e){
    edge = e;
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
