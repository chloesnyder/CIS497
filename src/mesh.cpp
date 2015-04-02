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

//calculates the centroid of a face
Vertex* Mesh::calculateCentroid(Face* f){
    QList<Vertex*> visited;
    HalfEdge* e = f->getStartEdge();
    Vertex* v = e->getVert();
    //find all the vertices in the face
    while(!visited.contains(v)) {
        visited.append(v);
        e = e->getNext();
        v = e->getVert();
    }

    vec4 centroid = vec4(0,0,0,1);
    int size = visited.size();

    //add up the positions of all the vertices in the face
    for(Vertex* vert : visited) {
        centroid += vert->getPos();
    }
    //divide by the size to get the avg position for centroid
    centroid.x /= size;
    centroid.y /= size;
    centroid.z /= size;
    centroid.w = 1;
    Vertex* centroid_vert = new Vertex();
    centroid_vert->setPos(centroid);
    max_vert_id++;
    centroid_vert->setID(max_vert_id);
    return centroid_vert;
}

void Mesh::quadrangulate(Face* f, Vertex* centroid, QMap<Face*, QList<HalfEdge*>>& f_to_mp, QMap<Face*, QList<HalfEdge*>>& f_to_orig_v){

    QList<HalfEdge*> to_centroid;
    QList<HalfEdge*> from_centroid;

    //get the vector of all half edges pointing to midpoints
    QList<HalfEdge*> mid_edges = f_to_mp[f];
    //get the vector of all half edges pointing to orig vertices
    QList<HalfEdge*> orig_edges = f_to_orig_v[f];

    QList<HalfEdge*> point_to_mp;
    QList<HalfEdge*> point_to_vert;

    //find number of vertices in orig face
    HalfEdge* start = f->getStartEdge();
    HalfEdge* e = start;
    do {
        e = e->getNext();
        //see if the edge points to a midpoint, if so, add to points to midpoint list. Else, add to point to vertex list
        if(mid_edges.contains(e)) {
            point_to_mp.append(e);
        } else {
            point_to_vert.append(e);
        }
    } while (e != start);

    int n = point_to_vert.size();

    //need to make n new faces
    int orig_f_list_size = f_list.size(); //use this to compare later
    QList<Face*> new_faces;

    //now will have a list of all the pointing to midpoints and pointing to verts
    //point_to_vert[i].next = point_to_mp[i] //this should be done already
    //point_to_mp[i].next = to_centroid[i];
    //to_centroid[i].next = from_centroid[i]
    //from_centroid[i].next = point_to_vert[i];
    for(int i = 0; i < n; i++) {
        Face* new_face = new Face();
        new_face->setID(++max_face_id);
        new_faces.append(new_face);
        f_list.push_back(new_face);


        //update face pointer; next pointer should be done already
        point_to_vert[i]->setFace(new_face);
        point_to_vert[i]->getNext()->setFace(new_face);

        //create new edge to centroid
        HalfEdge* new_to_centroid = new HalfEdge();
        HE_list.push_back(new_to_centroid);
        new_to_centroid->setID(++max_edge_id);
        new_to_centroid->setVert(centroid);
        new_to_centroid->setFace(new_face);
        to_centroid.append(new_to_centroid);

        point_to_vert[i]->getNext()->setNext(to_centroid[i]);

        //create new edge from centroid
        HalfEdge* new_from_centroid = new HalfEdge();
        new_from_centroid->setID(++max_edge_id);
        HE_list.push_back(new_from_centroid);
        Vertex* p = point_to_vert[i]->getSym()->getVert(); //this gives the midpoint the edge should point to
        new_from_centroid->setVert(p);
        new_from_centroid->setFace(new_face);
        from_centroid.append(new_from_centroid);

        to_centroid[i]->setNext(from_centroid[i]);

        from_centroid[i]->setNext(point_to_vert[i]);

//        if(i != 0) {
//            point_to_vert[i]->setNext(point_to_mp[i-1]);
//        } else {
//            point_to_vert[i]->setNext(point_to_mp[point_to_mp.size() - 1]);
//        }

        new_face->setStartEdge(point_to_vert[i]);

    }
    //set the syms of the new half edges
    //first from and last to need to be hardcoded b/c special case
    to_centroid[to_centroid.size() - 1]->setSym(from_centroid[0]);
    from_centroid[0]->setSym(to_centroid[to_centroid.size() - 1]);

    for(int i = 1; i < from_centroid.size(); i++) {
        from_centroid[i]->setSym(to_centroid[i-1]);
        to_centroid[i-1]->setSym(from_centroid[i]);
    }

}



void Mesh::subdivide() {

    QMap<Face*, QList<HalfEdge*>> mp_he_set; //a map that maps a face to the set of half edges that point to the midpoints on the face
    QMap<Face*, QList<HalfEdge*>> v_he_set; //a map that maps a face to the set of half edges that point to the original vertices on the face

    QList<Vertex*> orig_mesh_data_verts;
    QList<Face*> orig_mesh_data_faces;
    for(Vertex* orig_vert : v_list) {
        orig_mesh_data_verts.append(orig_vert);
    }
    for(Face* orig_face : f_list) {
        orig_mesh_data_faces.append(orig_face);
    }

    QList<HalfEdge*> visited = QList<HalfEdge*>();

    //go through each face in the mesh, find the centroids
    for(Face* f : f_list) {
        Vertex* centroid = calculateCentroid(f);
        centroid_map.insert(f, centroid);

        centroid->setID(++max_vert_id);
        v_list.push_back(centroid);
    }

    QList<HalfEdge*> to_split;
    //go through all the edges in the mesh, calculate midpoint
    for(HalfEdge* e : HE_list){
        if(!to_split.contains(e->getSym())){
            to_split.append(e);
        }
    }

    for(HalfEdge* e : to_split){
        //make sure you only calculate 1 midpoint per edge, not 2 per half edge

            vec4 pos = vec4(0,0,0,1);

            //call split edge function to get midpoint of edge
            Vertex* mp = addVertex(e);

            //average mp with face centroids of incident faces to get overall midpoint
            Face* f = e->getFace();
            Vertex* f1 = centroid_map[f];
            Vertex* f2 = new Vertex();
            //if f2 exists, i.e. edge incident to 2 faces, not a border edge
            if(e->getSym()->getFace() != NULL) {
                f2 = centroid_map[e->getSym()->getFace()];
                pos = 2.0f * mp->getPos() + f1->getPos() + f2->getPos();
                pos.x /= 4;
                pos.y /= 4;
                pos.z /= 4;
                pos.w = 1;

            } else {
                pos = 2.0f * mp->getPos() + f1->getPos();
                pos.x /= 3;
                pos.y /= 3;
                pos.z /= 3;
                pos.w = 1;
            }

            //update midpoint so that it is the smoothed midpoint
            mp->setPos(pos);
            midpoint_map.insert(e, mp);
            midpoint_map.insert(e->getSym(), mp);
    }


    //smooth the original vertices
    for(Vertex* v : orig_mesh_data_verts) {
        vector<HalfEdge*> adjacent_edges;

        //get faces incident to vertex
        vector<Face*> incident_faces;
        HalfEdge* e1 = v->getEdge();
        Face* startFace = e1->getFace();
        Face* face = startFace;
        do{
            if(face != NULL) {
                //add to list of faces
                incident_faces.push_back(face);
            }

            e1 = e1->getSym();

            //it's entering an infinite loop here
            while(e1->getVert() != v) {
                e1 = e1->getNext();
            }

            face = e1->getFace();
        } while(e1->getFace() != startFace);

        vec4 centroid_sum = vec4(0,0,0,1);
        vec4 adjacent_mp_sum = vec4(0,0,0,1);

        //sum the incident face centroids and adjacent midpoints
        for(Face* fce : incident_faces) {
            //sum face centroids
            Vertex* cent = centroid_map[fce];
            centroid_sum += cent->getPos();

            //find the edges adjacent to the vertex
            HalfEdge* edge = fce->getStartEdge();
            while(edge->getVert() != v) {
                edge = edge->getNext();
            }

            adjacent_edges.push_back(edge);
        }

        //sum the adjacent midpoints
        for(HalfEdge* e2 : adjacent_edges) {
            Vertex* v2 = e2->getSym()->getVert();
            //create a vector of all this vertex's adjacent midpoints

            adjacent_mp_sum.x += v2->getPos().x;
            adjacent_mp_sum.y += v2->getPos().y;
            adjacent_mp_sum.z += v2->getPos().z;
            adjacent_mp_sum.w = 1;
        }

        float n = adjacent_edges.size();
        adjacent_mp_sum.x /= (n*n);
        adjacent_mp_sum.y /= (n*n);
        adjacent_mp_sum.z /= (n*n);

        centroid_sum.x /= (n*n);
        centroid_sum.y /= (n*n);
        centroid_sum.z /= (n*n);

        vec4 new_pos = v->getPos();
        new_pos.x *= (n-2);
        new_pos.x /= n;
        new_pos.y /= n;
        new_pos.y *= (n-2);
        new_pos.z /= n;
        new_pos.z *= (n-2);

        new_pos += (adjacent_mp_sum + centroid_sum);
        new_pos.w = 1;
        v->setPos(new_pos);

    }

    //populate map with the set of half edges that point to a midpoint in the face
    for(Face* fc : f_list) {
        QList<HalfEdge*> midpoints_on_face;
        QList<HalfEdge*> orig_verts_on_face;
        HalfEdge* start = fc->getStartEdge();
        HalfEdge* e = start;
        do {
            //if the edge points to a vertex not in the original list of vertices, then we know
            //that it points to a midpoint. Add it to the vector of midpoints on the face
            if(!orig_mesh_data_verts.contains(e->getVert())){
                midpoints_on_face.push_back(e);
            } else {
                orig_verts_on_face.push_back(e);
            }
            e = e->getNext();
        } while (e != start);
        mp_he_set.insert(fc, midpoints_on_face);
        v_he_set.insert(fc, orig_verts_on_face);
    }

    while(!orig_mesh_data_faces.empty()) {
        quadrangulate(orig_mesh_data_faces[0], centroid_map[orig_mesh_data_faces[0]], mp_he_set, v_he_set);
        for(int i = 0; i < f_list.size(); i++){
            if(f_list[i] == orig_mesh_data_faces[0]) {
                f_list.erase(f_list.begin() + i);
            }
        }
        delete orig_mesh_data_faces[0];
        orig_mesh_data_faces.erase(orig_mesh_data_faces.begin());
    }

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
        HE_list.push_back(v7v4);
        HE_list.push_back(v4v7);
        //check later
        v7v4->setID(22);
        v4v7->setID(23);
        v7v4->setVert(v4);
        v4v7->setVert(v7);
        v4v7->setSym(v7v4);
        v7v4->setSym(v4v7);
        v4v7->setFace(back_face);
        v7v4->setFace(bottom_face);

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
    HalfEdge* e = HE_0->getNext();
    while(e != HE_0){
        count_edges++;
        e = e->getNext();
    }

    if(count_edges == 3) {

        //create 2 new half edges
        HalfEdge* HE_A = new HalfEdge();
        HalfEdge* HE_B = new HalfEdge();

        //set vertices
        HE_A->setVert(HE_0->getVert());
        HE_B->setVert((((HE_0->getNext())->getNext())->getVert()));

        //set sym edgs
        HE_A->setSym(HE_B);
        HE_B->setSym(HE_A);

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
    } else {
        return NULL;
    }
}

HalfEdge* Mesh::getPrev(HalfEdge *e) {
    HalfEdge* e2 = e;
    while(e2->getNext() != e) {
        e2 = e2->getNext();
    }
    return e2;
}

void Mesh::deleteVertex(Vertex *v, QList<HalfEdge*>& to_remove_from_he_list, QList<Vertex*>& to_remove_from_v_list) {

    //find all faces incident to vertex
    //start deleting each of those faces but when you delete a face, set all of its half edge face pointers to null
    // then for every half edge in the face in the loop, check to see if that half edge's sym pointer's face pointer is also null
    //if so, we are looking a a degenerate edge that's not bounded by any face. if that's the case, delete both that half edge and the sym.

    //find all faces incident to v
    HalfEdge* e1 = v->getEdge();
    //update
    v->setEdge(NULL);
    QList<Face*> incident_faces;

    //keep track of a list of "previous edges"
    QList<HalfEdge*> previousEdges;

    Face* f = e1->getFace();
    Face* startFace = f;

    do{
        if(f != NULL) {
            //add to list of faces
            incident_faces.push_back(f);
        }

        e1 = e1->getSym();
       // if(e1->getVert() != NULL) {
            while(e1->getVert() != v) {
                e1 = e1->getNext();
            }
      //  }
        f = e1->getFace();
    } while(e1->getFace() != startFace);

    //start deleting faces
    while(!incident_faces.empty()) {

        //set all its half edge pointers to null
        Face* f2 = incident_faces.front();
        HalfEdge* start = f2->getStartEdge();
        HalfEdge* e = start;

        //traverse through all of the face's edges
        do{

//          for the edge pointing to the vertex you delete, store the previous edge in a list to update
//          later, update so that its next = this.sym.prev.sym
            if(e->getVert() == v) {
                //get the edge previous to current edge, store in list to update the next later
                HalfEdge* p = getPrev(e);
                if(p != NULL && !previousEdges.contains(p)) {
                  previousEdges.push_back(p);
                }
            }
            else if(e->getSym()->getVert() == v){
                HalfEdge* p = getPrev(e->getSym());
                if(p != NULL && !previousEdges.contains(p)) {
                  previousEdges.push_back(p);
                }
            }

            //set all of the half edge face pointers pointing to the face to null
            e->setFace(NULL);

            //check if the half edge's sym pointer's face pointer is also null
            if(e->getSym() != NULL) {
                if(e->getSym()->getFace() == NULL && !to_remove_from_he_list.contains(e)) {
                    //store this edge and it's sym in list of edges to be deleted later
                    to_remove_from_he_list.append(e->getSym());
                    to_remove_from_he_list.append(e);
                    e = e->getNext();
                } else {
                //update e
                    e = e->getNext();
                }
            }
        } while(e != start);

        //remove face from global list of faces
        //reduce max size      
        incident_faces.removeAt(0); //remove f2 from list
        for(int i = 0; i < f_list.size(); i++) {
            if(f_list[i] == f2) {
                f_list.erase(f_list.begin() + i);
                --max_face_id;
            }
        }
        delete f2; //delete it
    }

    //update the pointers to maintain "empty" HalfEdges on boundary of structure
    // prev.next = prev.sym.prev.sym

    if(f_list.size() > 0){
        for(HalfEdge* prev : previousEdges) {
            if(prev->getNext() != NULL) {
                if(prev->getNext()->getSym() != NULL) {
                    HalfEdge* s1 = prev->getSym();
                    HalfEdge* p = getPrev(s1);
                    HalfEdge* s2 = p->getSym();
                    prev->setNext(s2);
                }
            }
        }
    }

    for(int i = 0; i < v_list.size(); i++){
        if(v_list[i] == v) {
            v_list.erase(v_list.begin() + i);
            to_remove_from_v_list.append(v);
            --max_vert_id;
        }
    }
    delete v;
}

void Mesh::create()
{

  vector<GLuint> mesh_idx;
  vector<vec4> mesh_vert_pos;
  vector<vec4> mesh_vert_nor;
  vector<vec4> mesh_vert_col;

  createMeshVertexPositionsNormalsIndices(mesh_vert_pos, mesh_vert_nor, mesh_vert_col, mesh_idx, f_list);

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
