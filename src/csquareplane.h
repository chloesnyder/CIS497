#ifndef CSQUAREPLANE_H
#define CSQUAREPLANE_H

#include <drawable.h>

class CSquarePlane : public Drawable
{
public:
    CSquarePlane(GLWidget277* context);
    void create();

    GLenum drawMode() override;
private:
    std::vector<glm::vec4> mVertices;
    std::vector<GLuint> mIndices;

};

#endif // CSQUAREPLANE_H
