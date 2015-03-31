#include "vertex.h"
#include <stdlib.h>

using namespace std;

Vertex::Vertex()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
{
}

Vertex::~Vertex(){
}

void Vertex::setPos(vec4 &p){
    pos = p;
}

void Vertex::setEdge(HalfEdge *e){
    edge = e;
}

void Vertex::setID(int i){
    id = i;
    name = QString::number(id);
    this->setText(name);
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
void Vertex::create(){
    GLuint vert_idx[1];
    vec4 vert_pos[1];
    vec4 vert_col[1];

    vert_pos[0] = this->getPos();
    vert_col[0] = vec4(0,1,0,1);
    vert_idx[0] = 0;


    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(vert_idx, 1 * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(vert_pos, 1* sizeof(vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(vert_col, 1 * sizeof(vec4));
}

void Vertex::destroy(){
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum Vertex::drawMode(){
    return GL_POINTS;
}

int Vertex::elemCount(){
    return 1;
}

bool Vertex::bindIdx(){
    return bufIdx.bind();
}

bool Vertex::bindPos(){
    return bufPos.bind();
}

bool Vertex::bindNor(){
    return bufNor.bind();
}

bool Vertex::bindCol(){
    return bufCol.bind();
}
