#ifndef CCHUNKARR_H
#define CCHUNKARR_H
#pragma once
#include <drawable.h>
#include "cvoxel.h"
#include "cworldarr.h"

#include "structs.h"

class CChunkArr : public Drawable
{
public:
    CChunkArr(GLWidget277* context);
    CChunkArr(GLWidget277* context, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    CChunkArr(GLWidget277* context, CWorldArr*, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

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

    CWorldArr *getWorld() { return mWorld; }
    void setWorld(CWorldArr *w) { mWorld = w; }

    int Polygonise(GRIDCELL grid, double isolevel, std::vector<TRIANGLE> &triangles);
    glm::vec4 VertexInterp(double isolevel, glm::vec4 p1, glm::vec4 p2, double valp1, double valp2);

    void populateVoxelBuffer();

private:
    // coords to define the absolute min and max coordinates of chunk's volume
    float m_Xmin, m_Xmax, m_Ymin, m_Ymax, m_Zmin, m_Zmax;

    std::vector<glm::vec4> mVertices;
    std::vector<GLuint> mIndices;

    //pointer to our world which gives info about hwere blocks are
    CWorldArr *mWorld;

    void createVoxelBuffer(std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices);

    double calculateDensity(glm::vec4 vertex);
    glm::vec4 calculateNormal(glm::vec4 vertex, int totalTris);

    int offset = 0;




};

#endif
