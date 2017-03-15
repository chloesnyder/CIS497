#ifndef CCHUNK_H
#define CCHUNK_H
#include <drawable.h>
#include "cvoxel.h"
#include "cworld.h"

typedef struct {
    glm::vec4 p[3]; // XYZ
} TRIANGLE;

typedef struct {
    glm::vec4 p[8]; // 8 points in a cube
    double val[8];
} GRIDCELL;

class CChunk : public Drawable
{
public:
    CChunk(GLWidget277* context);
    CChunk(GLWidget277* context, float, float, float, float, float, float);
    CChunk(GLWidget277* context, CWorld*, float, float, float, float, float, float);

    virtual void create();

    float getXMin() { return m_Xmin;}
    float getXMax() { return m_Xmax;}
    float getYMin() { return m_Ymin;}
    float getYMax() { return m_Ymax;}
    float getZMin() { return m_Zmin;}
    float getZMax() { return m_Zmax;}

    void setXMin(float x) { m_Xmin = x;}
    void setXMax(float x) { m_Xmax = x;}
    void setYMin(float y) { m_Ymin = y;}
    void setYMax(float y) { m_Ymax = y;}
    void setZMin(float z) { m_Zmin = z;}
    void setZMax(float z) { m_Zmax = z;}

    CWorld *getWorld() { return mWorld; }
    void setWorld(CWorld *w) { mWorld = w; }

    void setColor(glm::vec4 col) { mColor = col; }
    glm::vec4 getColor() { return mColor; }

    void recomputeAttributes() {this->create();}
    void setCameraForward(glm::vec4 look) { mCameraForward = look; }

    int Polygonise(GRIDCELL grid, double isolevel, std::vector<TRIANGLE> &triangles);
    glm::vec4 VertexInterp(double isolevel, glm::vec4 p1, glm::vec4 p2, double valp1, double valp2);

    void populateVoxelBuffer();

private:
    // coords to define the absolute min and max coordinates of chunk's volume
    float m_Xmin, m_Xmax, m_Ymin, m_Ymax, m_Zmin, m_Zmax;

    std::vector<glm::vec4> mVertices;
    std::vector<GLuint> mIndices;

    glm::vec4 mColor;

    //pointer to our world which gives info about hwere blocks are
    CWorld *mWorld;
    void checkFace(glm::vec4 *v000, glm::vec4 *v001, glm::vec4 *v010,
                   glm::vec4 *v100, glm::vec4 *v011, glm::vec4 *v101,
                   glm::vec4 *v110, glm::vec4 *v111, int face, int x, int y, int z, glm::vec4 color,
                   std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices);
    void createVoxelBuffer(std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices);
    void pushBackVertData(glm::vec4 *a, glm::vec4 *b, glm::vec4 *c, glm::vec4 *d, glm::vec4 normal, glm::vec4 color, std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices);

    double calculateDensity(glm::vec4 vertex);
    glm::vec4 calculateNormal(glm::vec4 vertex);

    glm::vec4 mCameraForward;
};

#endif // CCHUNK_H
