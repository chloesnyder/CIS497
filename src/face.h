#ifndef FACE_H
#define FACE_H
#include "halfedge.h"
#include <la.h>
#include <QListWidgetItem>

using namespace glm;

class HalfEdge;

class Face : public QListWidgetItem
{
private:
    HalfEdge* start_edge;
    vec4 color;
    int id;
    QString name;
public:
    Face();
    Face(HalfEdge* e, vec4 c, int i);
    ~Face();

    void setStartEdge(HalfEdge *e);
    void setColor(vec4 &c);
    void setID(int i);

    void addVertex(HalfEdge *e);


    HalfEdge* getStartEdge();
    vec4 getColor();
    int getID();
};

#endif // FACE_H
