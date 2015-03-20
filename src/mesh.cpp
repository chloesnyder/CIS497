#include "mesh.h"



Mesh::Mesh()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer) {
}

//http://stackoverflow.com/questions/10759206/how-to-calculate-vec4-cross-product-with-glm
vec4 Mesh::crossVec4(vec4 _v1, vec4 _v2){
    vec3 vec1 = vec3(_v1[0], _v1[1], _v1[2]);
    vec3 vec2 = vec3(_v2[0], _v2[1], _v2[2]);
    vec3 res = cross(vec1, vec2);
    return vec4(res[0], res[1], res[2], 0);
}

//Use triangle fan method
//create faces. Draw per face
void createMeshVertexPositionsNormalsIndices(vector<vec4>& mesh_vert_pos, vector<vec4>& mesh_vert_nor, vector<vec4>& mesh_vert_col, vector<GLuint>& mesh_idx, list<Face*>& faces){
   for(Face* f : faces) {
       vec4 color = (*f).getColor();
       int idx_count = 1;

       ///Compute Vertex Positions for the face
       //save first start edge pointer
       HalfEdge* start = (*f).getStartEdge();
       mesh_vert_pos.push_back((*(*start).getVert()).getPos());
       HalfEdge* e = (*start).getNext();

       ///Compute vertex normals for the face
       HalfEdge* e1 = f->getStartEdge();
       vec4 v1 = e1->getVert()->getPos();
       HalfEdge* e2 = e1->getNext();
       vec4 v2 = e2->getVert()->getPos();
       HalfEdge* e3 = e2->getNext();
       vec4 v3 = e3->getVert()->getPos();
//       HalfEdge* e4 = e3->getNext();
//       vec4 v4 = e4->getVert()->getPos();


       vec4 first = v2 - v1;
       vec4 second = v3 - v2;
       vec4 cp = Mesh::crossVec4(first, second);
       mesh_vert_nor.push_back(cp);
       mesh_vert_col.push_back(color);


       //loop through each edge until you hit the start edge
       while(e != start) {
           ///store info about vertex positions
            mesh_vert_pos.push_back((*(*e).getVert()).getPos());
            ///store info about vertex normals
            mesh_vert_nor.push_back(cp);

            ///store info about vertex colors
            mesh_vert_col.push_back(color);

            //increment number of indices
            idx_count++;

            e = e->getNext();
       }

       ///Compute indices using triangle fan method
       // Base vertex = 0, the start edge's vertex
       for(int i = 0; i < (idx_count - 2); i++) {
            int i1 = i + 1;
            int i2 = i + 2;
            mesh_idx.push_back(0);
            mesh_idx.push_back(i1);
            mesh_idx.push_back(i2);
       }
   }
}

void Mesh::createCube() {

    ///set up Vertex info
    //back face
    Vertex* nx_ny_pz = new Vertex(); //lower back left
    Vertex* px_ny_pz = new Vertex(); //lower back right
    Vertex* px_py_pz = new Vertex(); //upper back right
    Vertex* nx_py_pz = new Vertex(); //upper back left
    //front face
    Vertex* nx_ny_nz = new Vertex(); //lower front right
    Vertex* px_ny_nz = new Vertex(); //lower front left
    Vertex* px_py_nz = new Vertex(); //upper front right
    Vertex* nx_py_nz = new Vertex(); //upper front left
    //id
    nx_ny_pz->setID(0);
    px_ny_pz->setID(1);
    px_py_pz ->setID(2);
    nx_py_pz->setID(3);
    nx_ny_nz->setID(4);
    px_ny_nz->setID(5);
    px_py_nz->setID(6);
    nx_py_nz->setID(7);
    //pos
    vec4 a = vec4(-.5f, -.5f, .5f, 1);
    vec4 b = vec4(.5f, -.5f, .5f, 1);
    vec4 c = vec4(.5f, .5f, .5f, 1);
    vec4 d = vec4(-.5f, .5f, .5f, 1);

    vec4 e = vec4(-.5f, -.5f, -.5f, 1);
    vec4 f = vec4(.5f, -.5f, -.5f, 1);
    vec4 g = vec4(.5f, .5f, -.5f, 1);
    vec4 h = vec4(-.5f, .5f, -.5f, 1);
    nx_ny_pz->setPos(a);
    px_ny_pz->setPos(b);
    px_py_pz ->setPos(c);
    nx_py_pz->setPos(d);
    nx_ny_nz->setPos(e);
    px_py_nz->setPos(f);
    px_py_nz->setPos(g);
    nx_py_nz->setPos(h);

    Face* front_face = new Face();
    Face* back_face = new Face();
    Face* top_face = new Face();
    Face* bottom_face = new Face();
    Face* left_face = new Face();
    Face* right_face = new Face();


    //TODO: make a real unfolded cube, label EVERYTHING and then program from there.
    //Set Up Half Edge Info

    //Set up Face Info

    //put the face in the global list of faces
    f_list.push_back(front_face);
    f_list.push_back(back_face);
    f_list.push_back(top_face);
    f_list.push_back(bottom_face);
    f_list.push_back(left_face);
    f_list.push_back(right_face);

}

///create a square to test mesh with.
void Mesh::createSquare() {

    ///set up Vertex info
    Vertex* v0 = new Vertex();
    Vertex* v1 = new Vertex();
    Vertex* v2 = new Vertex();
    Vertex* v3 = new Vertex();
    //id
    v0->setID(0);
    v1->setID(1);
    v2->setID(2);
    v3->setID(3);
    //pos
    vec4 a = vec4(-.5f, -.5f, 0, 1);
    vec4 b = vec4(.5f, -.5f, 0, 1);
    vec4 c = vec4(.5f, .5f, 0, 1);
    vec4 d = vec4(-.5f, .5f, 0, 1);
    v0->setPos(a);
    v1->setPos(b);
    v2->setPos(c);
    v3->setPos(d);

    Face* square = new Face();

    ///set up edge info
    HalfEdge* he_0 = new HalfEdge();
    HalfEdge* he_1 = new HalfEdge();
    HalfEdge* he_2 = new HalfEdge();
    HalfEdge* he_3 = new HalfEdge();
    //face
    he_0->setFace(square);
    he_1->setFace(square);
    he_2->setFace(square);
    he_3->setFace(square);
    //next
    he_0->setNext(he_1);
    he_1->setNext(he_2);
    he_2->setNext(he_3);
    he_3->setNext(he_0);
    //id
    he_0->setID(0);
    he_1->setID(1);
    he_2->setID(2);
    he_3->setID(3);
    //sym
    he_0->setSym(NULL);
    he_1->setSym(NULL);
    he_2->setSym(NULL);
    he_3->setSym(NULL);
    //vertex
    he_0->setVert(v0);
    v0->setEdge(he_0);
    he_1->setVert(v1);
    v1->setEdge(he_1);
    he_2->setVert(v2);
    v2->setEdge(he_2);
    he_3->setVert(v3);
    v3->setEdge(he_3);

    ///set up face
    square->setStartEdge(he_0);
    square->setID(0);
    vec4 color = vec4(1,0,0,1);
    square->setColor(color);

    //put the face in the global list of faces
    f_list.push_back(square);

}


void Mesh::create()
{

  createSquare();

  vector<GLuint> mesh_idx;
  vector<vec4> mesh_vert_pos;
  vector<vec4> mesh_vert_nor;
  vector<vec4> mesh_vert_col;

  createMeshVertexPositionsNormalsIndices(mesh_vert_pos, mesh_vert_nor, mesh_vert_col, mesh_idx, Mesh::f_list);

  count = mesh_idx.size();

  bufIdx.create();
  bufIdx.bind();
  bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufIdx.allocate(mesh_idx.data(), mesh_idx.size() * sizeof(GLuint));

  bufPos.create();
  bufPos.bind();
  bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufPos.allocate(mesh_vert_pos.data(), mesh_vert_pos.size() * sizeof(vec4));

  bufNor.create();
  bufNor.bind();
  bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufNor.allocate(mesh_vert_nor.data(), mesh_vert_nor.size() * sizeof(vec4));

  bufCol.create();
  bufCol.bind();
  bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufCol.allocate(mesh_vert_col.data(), mesh_vert_col.size() * sizeof(vec4));
}

void Mesh::destroy()
{
  bufIdx.destroy();
  bufPos.destroy();
  bufNor.destroy();
  bufCol.destroy();
}

GLenum Mesh::drawMode()
{
  return GL_TRIANGLES;
}

int Mesh::elemCount()
{
  return count;
}

bool Mesh::bindIdx()
{
  return bufIdx.bind();
}

bool Mesh::bindPos()
{
  return bufPos.bind();
}

bool Mesh::bindNor()
{
  return bufNor.bind();
}

bool Mesh::bindCol()
{
  return bufCol.bind();
}
