#include "csquareplane.h"

CSquarePlane::CSquarePlane(GLWidget277 *context)
    : Drawable(context)
{

    glm::vec4 v00 = glm::vec4(0, 0, 0, 1);
    glm::vec4 v01 = glm::vec4(512, 0, 0, 1);
    glm::vec4 v02 = glm::vec4(512, 0, 512, 1);
    glm::vec4 v03 = glm::vec4(0, 0, 512, 1);

    glm::vec4 red = glm::vec4(1, 0, 0, 1);
     glm::vec4 up = glm::vec4(0, 1, 0, 1);

    mVertices.push_back(v00);
//    mVertices.push_back(red);
//    mVertices.push_back(up);

    mVertices.push_back(v01);
//    mVertices.push_back(red);
//    mVertices.push_back(up);

    mVertices.push_back(v02);
//    mVertices.push_back(red);
//    mVertices.push_back(up);

    mVertices.push_back(v03);
//    mVertices.push_back(red);
//    mVertices.push_back(up);

    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(3);

}

GLenum CSquarePlane::drawMode()
{
    return GL_LINES;
}

void CSquarePlane::create()
{
    int count = mIndices.size();
    generateIdx();

    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() *
                          sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    generateVertData();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufVertData);
    context->glBufferData(GL_ARRAY_BUFFER, mVertices.size() *
                          sizeof(glm::vec4), mVertices.data(), GL_STATIC_DRAW);
}
