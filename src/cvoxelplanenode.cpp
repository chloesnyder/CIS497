#include "cvoxelplanenode.h"

// A voxel plane node stores a vector of voxels (created in the voxelizer class)
// and assigns the position info in world space
// The (x,y,z) will be the position of the first voxel in the vector,
// and all other voxels will be positioned in relation to this voxel
/*
CVoxelPlaneNode::CVoxelPlaneNode()
{

}

CVoxelPlaneNode::CVoxelPlaneNode(float tx, float ty, float tz,
                                 float rx, float ry, float rz,
                                 float sx, float sy, float sz)
{
    trans[0][0] = tx; trans[0][1] = ty; trans[0][2] = tz;
    trans[1][0] = rx; trans[1][1] = ry; trans[1][2] = rz;
    trans[2][0] = sx; trans[2][1] = sy; trans[2][2] = sz;
}

//CVoxelPlaneNode::~CVoxelPlaneNode()
//{
//    mVoxelPlane->erase(mVoxelPlane->begin(), mVoxelPlane->end());
//}

std::vector<Voxel*>* CVoxelPlaneNode::getVoxelPlane()
{
    return mVoxelPlane;
}

void CVoxelPlaneNode::setVoxelPlane(std::vector<Voxel *> *vp)
{
    mVoxelPlane = vp;
}

void CVoxelPlaneNode::setParent(CVoxelPlaneNode *p)
{
    parent = p;
}

void CVoxelPlaneNode::setChild(CVoxelPlaneNode *c)
{
    child = c;
}

glm::mat4 CVoxelPlaneNode::getTransform()
{
    glm::mat4 ident = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(ident, glm::vec3(trans[0][0], trans[0][1], trans[0][2]));
    glm::mat4 rotate_x = glm::rotate(ident, trans[1][0], glm::vec3(1, 0, 0));
    glm::mat4 rotate_y = glm::rotate(ident, trans[1][1], glm::vec3(0, 1, 0));
    glm::mat4 rotate_z = glm::rotate(ident, trans[1][2], glm::vec3(0, 0, 1));
    glm::mat4 scale = glm::scale(ident, glm::vec3(trans[2][0], trans[2][1], trans[2][2]));

    return translate * rotate_x * rotate_y * rotate_z * scale;
}

glm::mat4 CVoxelPlaneNode::getTransformForVoxel(Voxel *v, Voxel* prevVoxel)
{
    float tx = v->getRowID();
    float ty = v->getColumnID();
    glm::mat4 ident;
    if(prevVoxel == nullptr) {
        ident = glm::mat4(1.0f);
    } else {
        ident = prevVoxel->getTransform();
    }

    // Translate should take the node's translation, then move it over by tx and up by ty
    // trying to simulate "grid"

    glm::mat4 translate = glm::translate(ident, glm::vec3(trans[0][0] + ty, trans[0][1] + tx, trans[0][2]));
    glm::mat4 rotate_x = glm::rotate(ident, trans[1][0], glm::vec3(1, 0, 0));
    glm::mat4 rotate_y = glm::rotate(ident, trans[1][1], glm::vec3(0, 1, 0));
    glm::mat4 rotate_z = glm::rotate(ident, trans[1][2], glm::vec3(0, 0, 1));
    glm::mat4 scale = glm::scale(ident, glm::vec3(trans[2][0], trans[2][1], trans[2][2]));

    glm::mat4 transform = translate * rotate_x * rotate_y * rotate_z * scale;
    v->setTransform(transform); //save this as the transformation for this voxel
    return transform;
}
*/
