#include "halfedge.h"
#include <QDebug>
using namespace std;

static const int HE_IDX_COUNT = 2;
static const int HE_VERT_COUNT = 2;
static const int HE_COL_COUNT = 2;

HalfEdge::HalfEdge()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
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
    name = QString::number(id);
    this->setText(name);
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

void HalfEdge::setColor(vec4 c){
    color = c;
}

vec4 HalfEdge::getColor(){
    return color;
}


void HalfEdge::createEdgeVertexIndices(GLuint he_idx[HE_IDX_COUNT], vec4 he_vert_pos[HE_VERT_COUNT],  vec4 he_vert_col[HE_COL_COUNT]){

     he_vert_pos[0] = this->getVert()->getPos();
     he_vert_pos[1] = this->getSym()->getVert()->getPos();

     vec4 color = vec4(1,1,0,1);

     he_vert_col[0] = color;
     he_vert_col[1] = color;

     he_idx[0] = 0;
     he_idx[1] = 1;
 }

void HalfEdge::create(){

    GLuint he_idx[HE_IDX_COUNT];
    vec4 he_vert_pos[HE_VERT_COUNT];
    vec4 he_vert_col[HE_COL_COUNT];

    createEdgeVertexIndices(he_idx, he_vert_pos, he_vert_col);

    count = HE_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(he_idx, HE_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(he_vert_pos, HE_VERT_COUNT * sizeof(vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(he_vert_col, HE_COL_COUNT * sizeof(vec4));
}

void HalfEdge::destroy(){
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum HalfEdge::drawMode(){
    return GL_LINES;
}

int HalfEdge::elemCount(){
    return count;
}

bool HalfEdge::bindIdx(){
    return bufIdx.bind();
}

bool HalfEdge::bindPos(){
    return bufPos.bind();
}

bool HalfEdge::bindNor(){
    return bufNor.bind();
}

bool HalfEdge::bindCol(){
    return bufCol.bind();
}
