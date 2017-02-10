#include "voxel.h"

#include <iostream>
#include <la.h>

static const int VOXEL_IDX_COUNT = 36;
static const int VOXEL_VERT_COUNT = 24;

Voxel::Voxel()
  : bufIdx(QOpenGLBuffer::IndexBuffer),
    bufPos(QOpenGLBuffer::VertexBuffer),
    bufNor(QOpenGLBuffer::VertexBuffer),
    bufCol(QOpenGLBuffer::VertexBuffer)
{
}

void Voxel::setTransform(glm::mat4 trans){
    transform = trans;
}

glm::mat4 Voxel::getTransform() {
    return transform;
}

void Voxel::setVoxelColor(glm::vec4 voxelColor){
    voxCol = voxelColor;
}

void Voxel::createVoxelColors(glm::vec4 (&voxel_vert_col)[VOXEL_VERT_COUNT])
{
    for (int i = 0; i < VOXEL_VERT_COUNT; i++) {
        voxel_vert_col[i] = color;
    }
}

void Voxel::set2DAddress(int w, int h, int pos)
{
    rowID = w;
    columnID = h;
    arrayID = pos;
}

void Voxel::setLengthAddress(double l) {
    lengthID = l;
}

int Voxel::getRowID(){
    return rowID;
}

int Voxel::getColumnID(){
    return columnID;
}

int Voxel::getArrayID() {
    return arrayID;
}

double Voxel::getLengthID(){
    return lengthID;
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createVoxelVertexPositions(glm::vec4 (&voxel_vert_pos)[VOXEL_VERT_COUNT]) {
    //6 faces
    //4 sided shapes

    //Back Square (IDX 0-3)
    glm::vec4 v00 = glm::vec4(.5f, -.5f, .5f, 1);
    glm::vec4 v01 = glm::vec4(.5, .5f, .5f, 1);
    glm::vec4 v02 = glm::vec4(.5, .5f, -.5f, 1);
    glm::vec4 v03 = glm::vec4(.5f, -.5f, -.5f, 1);
    voxel_vert_pos[0] = v00;
    voxel_vert_pos[1] = v01;
    voxel_vert_pos[2] = v02;
    voxel_vert_pos[3] = v03;

    //Left Square (IDX 4-7)
    glm::vec4 v10 = glm::vec4(-.5f, -.5f, .5f, 1);
    glm::vec4 v11 = glm::vec4(-.5, .5f, .5f, 1);
    glm::vec4 v12 = glm::vec4(.5, .5f, .5f, 1);
    glm::vec4 v13 = glm::vec4(.5f, -.5f, .5f, 1);
    voxel_vert_pos[4] = v10;
    voxel_vert_pos[5] = v11;
    voxel_vert_pos[6] = v12;
    voxel_vert_pos[7] = v13;

    //Right Square (IDX 8-11)
    glm::vec4 v20 = glm::vec4(-.5f, -.5f, -.5f, 1);
    glm::vec4 v21 = glm::vec4(-.5f, .5f, -.5f, 1);
    glm::vec4 v22 = glm::vec4(.5f, .5f, -.5f, 1);
    glm::vec4 v23 = glm::vec4(.5f, -.5f, -.5f, 1);
    voxel_vert_pos[8] = v20;
    voxel_vert_pos[9] = v21;
    voxel_vert_pos[10] = v22;
    voxel_vert_pos[11] = v23;

    //Bottom Square (IDX 12-15)
    glm::vec4 v30 = glm::vec4(.5f, -.5f, .5f, 1);
    glm::vec4 v31 = glm::vec4(-.5f, -.5f, .5f, 1);
    glm::vec4 v32 = glm::vec4(-.5f, -.5f, -.5f, 1);
    glm::vec4 v33 = glm::vec4(.5f, -.5f, -.5f, 1);
    voxel_vert_pos[12] = v30;
    voxel_vert_pos[13] = v31;
    voxel_vert_pos[14] = v32;
    voxel_vert_pos[15] = v33;

    //Top Square (IDX 16-19)
    glm::vec4 v40 = glm::vec4(.5f, .5f, .5f, 1);
    glm::vec4 v41 = glm::vec4(-.5f, .5f, .5f, 1);
    glm::vec4 v42 = glm::vec4(-.5f, .5f, -.5f, 1);
    glm::vec4 v43 = glm::vec4(.5f, .5f, -.5f, 1);
    voxel_vert_pos[16] = v40;
    voxel_vert_pos[17] = v41;
    voxel_vert_pos[18] = v42;
    voxel_vert_pos[19] = v43;

    //Front Square (IDX 20-23)
    glm::vec4 v50 = glm::vec4(-.5f, -.5f, -.5f, 1);
    glm::vec4 v51 = glm::vec4(-.5f, .5f, -.5f, 1);
    glm::vec4 v52 = glm::vec4(-.5f, .5f, .5f, 1);
    glm::vec4 v53 = glm::vec4(-.5f, -.5f, .5f, 1);
    voxel_vert_pos[20] = v50;
    voxel_vert_pos[21] = v51;
    voxel_vert_pos[22] = v52;
    voxel_vert_pos[23] = v53;

}


void createVoxelVertexNormals(glm::vec4 (&voxel_vert_nor)[VOXEL_VERT_COUNT]){

     glm::vec4 pos_z = glm::vec4(0, 0, 1, 0);
     glm::vec4 neg_z = glm::vec4(0, 0, -1, 0);
     glm::vec4 pos_x = glm::vec4(1, 0, 0, 0);
     glm::vec4 neg_x = glm::vec4(-1, 0, 0, 0);
     glm::vec4 pos_y = glm::vec4(0, 1, 0, 0);
     glm::vec4 neg_y = glm::vec4(0, -1, 0, 0);

    //Back Square (IDX 0-3) use -x vector
     for(int i = 0; i < 4; i++) {
         voxel_vert_nor[i] = neg_x;
     }

    //Left Square (IDX 4-7) use +z
     for(int i = 4; i < 8; i++) {
         voxel_vert_nor[i] = pos_z;
     }

   //Right Square (IDX 8-11) use -z
     for(int i = 8; i < 12; i++) {
         voxel_vert_nor[i] = neg_z;
     }

   //Bottom Square (IDX 12-15) use -y
     for(int i = 12; i < 16; i++) {
         voxel_vert_nor[i] = neg_y;
     }

   //Top Square (IDX 16-19) use +y
     for(int i = 16; i < 20; i++) {
         voxel_vert_nor[i] = pos_y;
     }

    //Front Square (IDX 20-24) use +x
     for(int i = 20; i < 24; i++) {
         voxel_vert_nor[i] = pos_x;
     }

}


void createVoxelIndices(GLuint (&voxel_idx)[VOXEL_IDX_COUNT]){
    int idx = 0;
    for(int i = 0; i < 6; i++) {
        voxel_idx[idx++] = 4*i;
        voxel_idx[idx++] = (4*i) + 1;
        voxel_idx[idx++] = (4*i) + 2;
        voxel_idx[idx++] = 4*i;
        voxel_idx[idx++] = (4*i) + 2;
        voxel_idx[idx++] = (4*i) + 3;

    }
}

void Voxel::create()
{
  GLuint voxel_idx[VOXEL_IDX_COUNT];
  glm::vec4 voxel_vert_pos[VOXEL_VERT_COUNT];
  glm::vec4 voxel_vert_nor[VOXEL_VERT_COUNT];
  glm::vec4 voxel_vert_col[VOXEL_VERT_COUNT];

  createVoxelVertexPositions(voxel_vert_pos);
  createVoxelVertexNormals(voxel_vert_nor);
  createVoxelIndices(voxel_idx);
  createVoxelColors(voxel_vert_col);

  count = VOXEL_IDX_COUNT;

  bufIdx.create();
  bufIdx.bind();
  bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufIdx.allocate(voxel_idx, VOXEL_IDX_COUNT * sizeof(GLuint));

  bufPos.create();
  bufPos.bind();
  bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufPos.allocate(voxel_vert_pos,VOXEL_VERT_COUNT * sizeof(glm::vec4));

  bufNor.create();
  bufNor.bind();
  bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufNor.allocate(voxel_vert_nor, VOXEL_VERT_COUNT * sizeof(glm::vec4));

  bufCol.create();
  bufCol.bind();
  bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufCol.allocate(voxel_vert_col, VOXEL_VERT_COUNT * sizeof(glm::vec4));
}

//ShaderProgram::Drawable Voxel::getDrawable() {
  //  return this;
//}

void Voxel::destroy()
{
  bufIdx.destroy();
  bufPos.destroy();
  bufNor.destroy();
  bufCol.destroy();
}

GLenum Voxel::drawMode()
{
  return GL_TRIANGLES;
}

int Voxel::elemCount()
{
  return count;
}

bool Voxel::bindIdx()
{
  return bufIdx.bind();
}

bool Voxel::bindPos()
{
  return bufPos.bind();
}

bool Voxel::bindNor()
{
  return bufNor.bind();
}

bool Voxel::bindCol()
{
  return bufCol.bind();
}
