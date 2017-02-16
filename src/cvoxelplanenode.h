#ifndef CVOXELPLANENODE_H
#define CVOXELPLANENODE_H

#include "la.h"
#include "voxelizer.h"
/*
class CVoxelPlaneNode
{
public:
    CVoxelPlaneNode();
    CVoxelPlaneNode(float tx, float ty, float tz,
                    float rx, float ry, float rz,
                    float sx, float sy, float sz);
    //~CVoxelPlaneNode();

    void setVoxelPlane(std::vector<Voxel*> *vp);
    void setParent(CVoxelPlaneNode* p);
    void setChild(CVoxelPlaneNode* c);
    glm::mat4 getTransform();
    glm::mat4 getTransformForVoxel(Voxel* v, Voxel *prevVoxel);
    std::vector<Voxel*>* getVoxelPlane();

private:
    CVoxelPlaneNode *parent; //the plane above
    CVoxelPlaneNode *child; //the plane below
    glm::mat4 trans; //translation matrix

    std::vector<Voxel*>* mVoxelPlane;
};
*/
#endif // CVOXELPLANENODE_H
