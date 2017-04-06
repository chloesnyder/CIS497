#include "cube.h"
#include <la.h>
#include <iostream>

static const int SQUARE_IDX_COUNT = 8;
static const int SQUARE_VERT_COUNT = 4;


GLenum SquarePlane::drawMode()
{
    return GL_LINES;
}


//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createSquareVertexPositions(glm::vec4 (&square_vert_pos)[SQUARE_VERT_COUNT])
{
    int idx = 0;
    square_vert_pos[idx++] = glm::vec4(0, 0, 0, 1);
    square_vert_pos[idx++] = glm::vec4(512, 0, 0, 1);
    square_vert_pos[idx++] = glm::vec4(512, 0, 512, 1);
    square_vert_pos[idx++] = glm::vec4(0, 0, 512, 1);
}


void createCubeVertexNormals(glm::vec4 (&square_vert_nor)[SQUARE_VERT_COUNT])
{
    int idx = 0;
    //Top
    for(int i = 0; i < SQUARE_VERT_COUNT; i++){
        square_vert_nor[idx++] = glm::vec4(0,1,0,0);
    }

}

void createSquareIndices(GLuint (&square_idx)[SQUARE_IDX_COUNT])
{
    int idx = 0;

    square_idx[idx++] = 0;
    square_idx[idx++] = 1;

    square_idx[idx++] = 1;
    square_idx[idx++] = 2;

    square_idx[idx++] = 2;
    square_idx[idx++] = 3;

    square_idx[idx++] = 3;
    square_idx[idx++] = 0;

}

void SquarePlane::create()
{
    GLuint square_idx[SQUARE_IDX_COUNT];
    glm::vec4 square_vert_pos[SQUARE_VERT_COUNT];
    glm::vec4 square_vert_nor[SQUARE_VERT_COUNT];
    glm::vec4 square_vert_col[SQUARE_VERT_COUNT];

    createSquareVertexPositions(square_vert_pos);
    createCubeVertexNormals(square_vert_nor);
    createSquareIndices(square_idx);


    for(int i = 0; i < SQUARE_VERT_COUNT; i++){
        square_vert_col[i] = glm::vec4(1, 1, 1, 1);
    }

    count = SQUARE_IDX_COUNT;


    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, SQUARE_IDX_COUNT * sizeof(GLuint), square_idx, GL_STATIC_DRAW);

    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, SQUARE_VERT_COUNT * sizeof(glm::vec4), square_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, SQUARE_VERT_COUNT * sizeof(glm::vec4), square_vert_nor, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, SQUARE_VERT_COUNT * sizeof(glm::vec4), square_vert_col, GL_STATIC_DRAW);
}
