#include "face.h"

Face::Face()
{
}

Face::Face(HalfEdge *e, vec4 c, int i) {
    start_edge = e;
    color = c;
    id = i;
    name = "" + i;
}

Face::~Face(){
    HalfEdge* start = this->getStartEdge();
    HalfEdge* e = this->getStartEdge()->getNext();
    start_edge = NULL;
    while(e != start) {
        HalfEdge temp = *e;
        e = e->getNext();
        temp.~HalfEdge();
    }
}

void Face::setStartEdge(HalfEdge *e){
    start_edge = e;
}

void Face::setColor(vec4 &c){
    color = c;
}

void Face::setID(int i) {
    id = i;
    name = QString::number(id);
    this->setText(name);
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

void Face::addVertex(HalfEdge *e) {

//    Vertex* v1 = e->getVert();
//    Vertex* v2;
//    if(e->getSym() != NULL) {
//       v2 = e->getSym()->getVert();
//    } else {
//        HalfEdge* temp = e;
//        while(temp->getNext() != e) {
//            temp = temp->getNext();
//        }
//        v2 = temp->getVert();
//    }

//    //get the avg position of v1 v2
//    vec4 new_pos = ((v1->getPos()[0] + v2->getPos()[0])/2,(v1->getPos()[1] + v2->getPos()[1])/2, (v1->getPos()[2] + v2->getPos()[2])/2, 1);
//    //assign to new vert
//    Vertex* new_vert = Vertex();
//    new_vert->setPos(new_pos);
//    //set id
//    //create 4 new edges
}

