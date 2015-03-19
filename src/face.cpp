#include "face.h"

Face::Face()
{
}

Face::Face(HalfEdge *e, vec4 c, int i) {
    start_edge = e;
    color = c;
    id = i;
}

void Face::setStartEdge(HalfEdge &e){
    start_edge = &e;
}

void Face::setColor(vec4 &c){
    color = c;
}

void Face::setID(int i) {
    id = i;
}

HalfEdge *Face::getStartEdge(){
    return start_edge;
}

vec4 Face::getColor(){
    return color;
}

int Face::getID(){
    return id;
}
