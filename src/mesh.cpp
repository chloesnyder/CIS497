#include "mesh.h"

#include <iostream>

int Mesh::max_vert_id = 0;
int Mesh::max_edge_id = 0;
int Mesh::max_face_id = 0;

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
void Mesh::createMeshVertexPositionsNormalsIndices(vector<vec4>& mesh_vert_pos, vector<vec4>& mesh_vert_nor, vector<vec4>& mesh_vert_col, vector<GLuint>& mesh_idx, vector<Face*>& faces){
   GLuint base_idx = 0;
    for(Face* f : faces) {
       GLuint temp_idx = base_idx;
       vec4 color = (*f).getColor();
       if(f == selectedFace) {
           color = vec4(1, 1, 1, 1);
       }
       int idx_count = 1;

       ///Compute Vertex Positions for the face
       //save first start edge pointer
       HalfEdge* start = (*f).getStartEdge();
       mesh_vert_pos.push_back(start->getVert()->getPos());
       base_idx++;
       HalfEdge* e = (*start).getNext();

       ///Compute vertex normals for the face
       HalfEdge* e1 = f->getStartEdge();
       vec4 v1 = e1->getVert()->getPos();
       HalfEdge* e2 = e1->getNext();
       vec4 v2 = e2->getVert()->getPos();
       HalfEdge* e3 = e2->getNext();
       vec4 v3 = e3->getVert()->getPos();


       vec4 first = v2 - v1;
       vec4 second = v3 - v2;
       vec4 cp = Mesh::crossVec4(first, second);
       mesh_vert_nor.push_back(cp);
       mesh_vert_col.push_back(color);


       //loop through each edge until you hit the start edge
       while(e != start) {
           ///store info about vertex positions
            mesh_vert_pos.push_back(e->getVert()->getPos());

            base_idx++;
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
       for(GLuint i = 0; i < (idx_count - 2); i++) {
            GLuint i1 = i + 1;
            GLuint i2 = i + 2;
            mesh_idx.push_back(temp_idx);
            mesh_idx.push_back(i1 + temp_idx);
            mesh_idx.push_back(i2 + temp_idx);
       }
   }
}

//void Mesh::storeMyGL(MyGL *m){
//    mygl = m;
//}


void Mesh::createCube() {

        Mesh::max_vert_id = 7;
        Mesh::max_edge_id = 23;
        Mesh::max_face_id = 5;

        //back face
        Vertex* v0 = new Vertex(); //lower back left
        Vertex* v1 = new Vertex(); //lower back right
        Vertex* v2 = new Vertex(); //upper back right
        Vertex* v3 = new Vertex(); //upper back left
        //front face
        Vertex* v4 = new Vertex(); //lower front right
        Vertex* v5 = new Vertex(); //lower front left
        Vertex* v6 = new Vertex(); //upper front right
        Vertex* v7 = new Vertex(); //upper front left
        //assign ids for vertices
        v0->setID(0);
        v1->setID(1);
        v2 ->setID(2);
        v3->setID(3);
        v4->setID(4);
        v5->setID(5);
        v6->setID(6);
        v7->setID(7);
        //assign vertice positions
        vec4 a = vec4(.5f, .5f, .5f, 1); //0
        vec4 b = vec4(-.5f, .5f, .5f, 1); //1
        vec4 c = vec4(-.5f, -.5f, .5f, 1); //2
        vec4 d = vec4(.5f, -.5f, .5f, 1); //3
        vec4 e = vec4(.5f, -.5f, -.5f, 1); //4
        vec4 f = vec4(.5f, .5f, -.5f, 1); //5
        vec4 g = vec4(-.5f, .5f, -.5f, 1); //6
        vec4 h = vec4(-.5f, -.5f, -.5f, 1); //7
        v0->setPos(a);
        v1->setPos(b);
        v2 ->setPos(c);
        v3->setPos(d);
        v4->setPos(e);
        v5->setPos(f);
        v6->setPos(g);
        v7->setPos(h);
        v_list.push_back(v0);
        v_list.push_back(v1);
        v_list.push_back(v2);
        v_list.push_back(v3);
        v_list.push_back(v4);
        v_list.push_back(v5);
        v_list.push_back(v6);
        v_list.push_back(v7);


        ///Create faces (id, color)
        vec4 col = vec4(0,0,1,0);
        Face* front_face = new Face();
        front_face->setID(0);
        front_face->setColor(col);
        Face* back_face = new Face();
        back_face->setID(1);
        back_face->setColor(c);
        Face* top_face = new Face();
        top_face->setID(2);
        top_face->setColor(c);
        Face* bottom_face = new Face();
        bottom_face->setID(3);
        bottom_face->setColor(c);
        Face* left_face = new Face();
        left_face->setID(4);
        left_face->setColor(c);
        Face* right_face = new Face();
        right_face->setID(5);
        right_face->setColor(c);

        //create half edges
        //NAMING CONVENTION: second part is the vert it points to (indicates direction), second part is where it points from
        HalfEdge* v0v1 = new HalfEdge(); //front face top edge (1 edge)
        HalfEdge* v1v0 = new HalfEdge(); //top face bottom edge (1 edge)
        HE_list.push_back(v0v1);
        HE_list.push_back(v1v0);
        v0v1->setID(0);
        v1v0->setID(1);
        v0v1->setVert(v1);
        v1v0->setVert(v0);
        v0v1->setSym(v1v0);
        v1v0->setSym(v0v1);
        v0v1->setFace(front_face);
        v1v0->setFace(top_face);

        HalfEdge* v1v2 = new HalfEdge();//front face left edge (2e)
        HalfEdge* v2v1 = new HalfEdge();//left face right edge (1 edge)
        HE_list.push_back(v1v2);
        HE_list.push_back(v2v1);
        v1v2->setID(2);
        v2v1->setID(3);
        v1v2->setVert(v2);
        v2v1->setVert(v1);
        v1v2->setSym(v2v1);
        v2v1->setSym(v1v2);
        v1v2->setFace(front_face);
        v2v1->setFace(left_face);

        HalfEdge* v2v3 = new HalfEdge(); //front face bottom edge (3e)
        HalfEdge* v3v2 = new HalfEdge(); //bottom face top edge (1e)
        HE_list.push_back(v2v3);
        HE_list.push_back(v3v2);
        v2v3->setID(4);
        v3v2->setID(5);
        v2v3->setVert(v3);
        v3v2->setVert(v2);
        v2v3->setSym(v3v2);
        v3v2->setSym(v2v3);
        v2v3->setFace(front_face);
        v3v2->setFace(bottom_face);

        HalfEdge* v0v5 = new HalfEdge(); //top face right edge (2e)
        HalfEdge* v5v0 = new HalfEdge(); //right face top edge (1e)
        HE_list.push_back(v0v5);
        HE_list.push_back(v5v0);
        v0v5->setID(6);
        v5v0->setID(7);
        v0v5->setVert(v5);
        v5v0->setVert(v0);
        v0v5->setSym(v5v0);
        v5v0->setSym(v0v5);
        v0v5->setFace(top_face);
        v5v0->setFace(right_face);

        HalfEdge* v5v6 = new HalfEdge(); //top face top edge (3e)
        HalfEdge* v6v5 = new HalfEdge(); //back face top edge (1e)
        HE_list.push_back(v5v6);
        HE_list.push_back(v6v5);
        v5v6->setID(8);
        v6v5->setID(9);
        v5v6->setVert(v6);
        v6v5->setVert(v5);
        v5v6->setSym(v6v5);
        v6v5->setSym(v5v6);
        v5v6->setFace(top_face);
        v6v5->setFace(back_face);

        HalfEdge* v6v1 = new HalfEdge(); //top face left edge (4e DONE TOP FACE)
        HalfEdge* v1v6 = new HalfEdge(); //left face top edge (2e)
        HE_list.push_back(v6v1);
        HE_list.push_back(v1v6);
        v6v1->setID(10);
        v1v6->setID(11);
        v6v1->setVert(v1);
        v1v6->setVert(v6);
        v1v6->setSym(v6v1);
        v6v1->setSym(v1v6);
        v1v6->setFace(left_face);
        v6v1->setFace(top_face);

        HalfEdge* v6v7 = new HalfEdge(); //left face left edge (3e)
        HalfEdge* v7v6 = new HalfEdge(); //back face right edge (2e)
        HE_list.push_back(v6v7);
        HE_list.push_back(v7v6);
        v6v7->setID(12);
        v7v6->setID(13);
        v6v7->setVert(v7);
        v7v6->setVert(v6);
        v6v7->setSym(v7v6);
        v7v6->setSym(v6v7);
        v6v7->setFace(left_face);
        v7v6->setFace(back_face);

        HalfEdge* v7v2 = new HalfEdge();//left face bottom edge (4e DONE LEFT FACE)
        HalfEdge* v2v7 = new HalfEdge();//bottom face left edge (2e)
        HE_list.push_back(v7v2);
        HE_list.push_back(v2v7);
        v7v2->setID(14);
        v2v7->setID(15);
        v2v7->setVert(v7);
        v7v2->setVert(v2);
        v7v2->setSym(v2v7);
        v2v7->setSym(v7v2);
        v2v7->setFace(bottom_face);
        v7v2->setFace(left_face);

        HalfEdge* v3v0 = new HalfEdge(); //front face right edge (4e DONE FRONT FACE)
        HalfEdge* v0v3 = new HalfEdge(); //right face left edge (2e)
        HE_list.push_back(v3v0);
        HE_list.push_back(v0v3);
        v3v0->setID(16);
        v0v3->setID(17);
        v0v3->setVert(v3);
        v3v0->setVert(v0);
        v0v3->setSym(v3v0);
        v3v0->setSym(v0v3);
        v0v3->setFace(right_face);
        v3v0->setFace(front_face);

        HalfEdge* v3v4 = new HalfEdge(); //right face bottom edge (3e)
        HalfEdge* v4v3 = new HalfEdge(); //bottom face right edge (3e)
        HE_list.push_back(v3v4);
        HE_list.push_back(v4v3);
        v3v4->setID(18);
        v4v3->setID(19);
        v3v4->setVert(v4);
        v4v3->setVert(v3);
        v3v4->setSym(v4v3);
        v4v3->setSym(v3v4);
        v3v4->setFace(right_face);
        v4v3->setFace(bottom_face);

        HalfEdge* v4v5 = new HalfEdge(); // right face top edge (4e) DONE RIGHT FACE
        HalfEdge* v5v4 = new HalfEdge(); //back face left edge (3e)
        HE_list.push_back(v5v4);
        HE_list.push_back(v4v5);
        v5v4->setID(20);
        v4v5->setID(21);
        v5v4->setVert(v4);
        v4v5->setVert(v5);
        v5v4->setSym(v4v5);
        v4v5->setSym(v5v4);
        v4v5->setFace(right_face);
        v5v4->setFace(back_face);

        HalfEdge* v7v4 = new HalfEdge(); //back face bottom edge (4e) DONE BACK FACE
        HalfEdge* v4v7 = new HalfEdge(); //bottom face bottom edge (4e) DONE BOTTOM FACE
        HE_list.push_back(v4v7);
        HE_list.push_back(v7v4);
        v7v4->setID(22);
        v4v7->setID(23);
        v7v4->setVert(v4);
        v4v7->setVert(v7);
        v4v7->setSym(v7v4);
        v7v4->setSym(v4v7);
        v4v7->setFace(bottom_face);
        v7v4->setFace(back_face);

        //set face start edge and next loops
        front_face->setStartEdge(v0v1);
        v0v1->setNext(v1v2);
        v1v2->setNext(v2v3);
        v2v3->setNext(v3v0);
        v3v0->setNext(v0v1);

        top_face->setStartEdge(v1v0);
        v1v0->setNext(v0v5);
        v0v5->setNext(v5v6);
        v5v6->setNext(v6v1);
        v6v1->setNext(v1v0);

        left_face->setStartEdge(v1v6);
        v1v6->setNext(v6v7);
        v6v7->setNext(v7v2);
        v7v2->setNext(v2v1);
        v2v1->setNext(v1v6);

        back_face->setStartEdge(v6v5);
        v6v5->setNext(v5v4);
        v5v4->setNext(v4v7);
        v4v7->setNext(v7v6);
        v7v6->setNext(v6v5);

        right_face->setStartEdge(v5v0);
        v5v0->setNext(v0v3);
        v0v3->setNext(v3v4);
        v3v4->setNext(v4v5);
        v4v5->setNext(v5v0);

        bottom_face->setStartEdge(v3v2);
        v3v2->setNext(v2v7);
        v2v7->setNext(v7v4);
        v7v4->setNext(v4v3);
        v4v3->setNext(v3v2);

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

Vertex* Mesh::addVertex(HalfEdge *HE1) {

    HalfEdge* HE2 = HE1->getSym();
    Face* f1 = HE1->getFace();
    Face* f2 = HE2->getFace();

    //create new vertex
    Vertex* v3 = new Vertex();
    Vertex* v1 = HE1->getVert();
    Vertex* v2 = HE2->getVert();
    //get the avg position of v1 v2
    vec4 new_pos = vec4
            ((v1->getPos().x + v2->getPos().x)/2,
             (v1->getPos().y + v2->getPos().y)/2,
             (v1->getPos().z + v2->getPos().z)/2,
             1.0f
             );
    v3->setPos(new_pos);

    //set id and add to global list
    v3->setID(++max_vert_id);
    v_list.push_back(v3);


    //create 2 new half edges
    HalfEdge* HE1B = new HalfEdge();
    HalfEdge* HE2B = new HalfEdge();
    int edge_id = HE_list.size();
    HE1B->setID(++max_edge_id);
    HE2B->setID(++max_edge_id);
    HE_list.push_back(HE1B);
    HE_list.push_back(HE2B);

    //set faces
    HE1B->setFace(f1);
    HE2B->setFace(f2);

    //adjust sym pointers
    HE1B->setSym(HE2);
    HE2->setSym(HE1B);
    HE2B->setSym(HE1);
    HE1->setSym(HE2B);

    //adjust next pointers
    HE1B->setNext(HE1->getNext());
    HE2B->setNext(HE2->getNext());
    HE1->setNext(HE1B);
    HE2->setNext(HE2B);

    //adjust vert pointers
    HE1B->setVert(v1);
    HE2B->setVert(v2);
    HE2->setVert(v3);
    HE1->setVert(v3);

    return v3;
}

Face* Mesh::triangulate(Face* FACE1){
    //quad or ngon?
    int count_edges = 0;
    HalfEdge* HE_0 = FACE1->getStartEdge();
    std::cout<< "here a" << std::endl;
    HalfEdge* e = HE_0->getNext();
    while(e->getNext() != HE_0){
        count_edges++;
        std::cout<< "here b" << std::endl;
        e = e->getNext();
    }

    if(count_edges == 3) {

        std::cout<< "here c" << std::endl;
        //create 2 new half edges
        HalfEdge* HE_A = new HalfEdge();
        HalfEdge* HE_B = new HalfEdge();

        //set vertices
        HE_A->setVert(HE_0->getVert());
        std::cout<< "here d" << std::endl;
        HE_B->setVert((((HE_0->getNext())->getNext())->getVert()));
        std::cout<< "here e" << std::endl;

        //set sym edgs
        HE_A->setSym(HE_B);
        std::cout<< "here f" << std::endl;
        HE_B->setSym(HE_A);
        std::cout<< "here g" << std::endl;


        //assign new ids
        HE_A->setID(++max_edge_id);
        HE_B->setID(++max_edge_id);

        //create new face
        Face* FACE2 = new Face();
        HE_A->setFace(FACE2);
        (HE_0->getNext())->setFace(FACE2);
        ((HE_0->getNext())->getNext())->setFace(FACE2);
        HE_B->setFace(FACE1);
        FACE2->setStartEdge(HE_A);
        FACE2->setID(++max_face_id);

        HE_B->setNext(((HE_0->getNext())->getNext())->getNext());

        ((HE_0->getNext())->getNext())->setNext(HE_A);
        HE_A->setNext((HE_0->getNext()));
        HE_0->setNext(HE_B);

        f_list.push_back(FACE2);
        HE_list.push_back(HE_A);
        HE_list.push_back(HE_B);

        return FACE2;
    }
}


void Mesh::create()
{

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
