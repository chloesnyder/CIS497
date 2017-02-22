#include "cchunk.h"

int offset = 0;

CChunk::CChunk(GLWidget277 *context)
    : Drawable(context), mWorld(nullptr), m_Xmin(0), m_Xmax(0), m_Ymin(0), m_Ymax(0), m_Zmin(0), m_Zmax(0)
{
}

CChunk::CChunk(GLWidget277* context, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) :
    Drawable(context), mWorld(nullptr), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax)
{

}

CChunk::CChunk(GLWidget277* context, CWorld* w, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
    : Drawable(context), mWorld(w), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax)
{

}

void CChunk::pushBackVertData(glm::vec4 *a, glm::vec4 *b, glm::vec4 *c, glm::vec4 *d, glm::vec4 normal, glm::vec4 color,
                              std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices)
{
    vertices->push_back(*a);
    vertices->push_back(color);
    vertices->push_back(normal);

    vertices->push_back(*b);
    vertices->push_back(color);
    vertices->push_back(normal);

    vertices->push_back(*c);
    vertices->push_back(color);
    vertices->push_back(normal);

    vertices->push_back(*d);
    vertices->push_back(color);
    vertices->push_back(normal);

    indices->push_back(offset);
    indices->push_back(offset + 1);
    indices->push_back(offset + 2);
    indices->push_back(offset);
    indices->push_back(offset + 2);
    indices->push_back(offset + 3);
    offset += 4;

}

void CChunk::checkFace(glm::vec4 *v000, glm::vec4 *v001, glm::vec4 *v010,
                       glm::vec4 *v100, glm::vec4 *v011, glm::vec4 *v101,
                       glm::vec4 *v110, glm::vec4 *v111, int face, int x, int y, int z,
                       glm::vec4 color,
                       std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices)
{
    // Want to check all of my 6 faces to see whether or not I should render it
    // If there is a voxel next to face x, I do not render face x. If there is nothing there, then I render face x.
    glm::vec4 posXNor = glm::vec4(1,0,0,0);
    glm::vec4 negXNor = glm::vec4(-1,0,0,0);
    glm::vec4 posYNor = glm::vec4(0,1,0,0);
    glm::vec4 negYNor = glm::vec4(0,-1,0,0);
    glm::vec4 posZNor = glm::vec4(0,0,1,0);
    glm::vec4 negZNor = glm::vec4(0,0,-1,0);


    if(face == 0) {
        if(!mWorld->hasVoxelAt(x, y, z + 1))
        {
          //  if(glm::dot(posZNor, mCameraForward) > 0) {
                pushBackVertData(v111, v101, v001, v011, posZNor, color, vertices, indices);
          //  }
        }

    } else if (face == 1) {
        if(!mWorld->hasVoxelAt(x + 1, y, z))
        {
          //  if(glm::dot(posXNor, mCameraForward) > 0) {
                pushBackVertData(v110, v100, v101, v111, posXNor, color, vertices, indices);
           // }
        }

    } else if (face == 2) {
        if(!mWorld->hasVoxelAt(x - 1, y, z))
        {
           // if(glm::dot(negXNor, mCameraForward) > 0) {
                pushBackVertData(v011, v001, v000, v010, negXNor, color, vertices, indices);
           // }
        }

    } else if (face == 3) {
        if(!mWorld->hasVoxelAt(x, y, z - 1))
        {
           // if(glm::dot(negZNor, mCameraForward) > 0) {
                pushBackVertData(v010, v000, v100, v110, negZNor, color, vertices, indices);
           // }
        }

    } else if (face == 4) {
        if(!mWorld->hasVoxelAt(x, y + 1, z))
        {
           // if(glm::dot(posYNor, mCameraForward) > 0) {
                pushBackVertData(v110, v111, v011, v010, posYNor, color, vertices, indices);
           // }
        }

    } else if (face == 5) {
        if(!mWorld->hasVoxelAt(x, y - 1, z))
        {
            //if(glm::dot(negYNor, mCameraForward) > 0) {
                pushBackVertData(v101, v100, v000, v001, negYNor, color, vertices, indices);
           // }
        }
    }
}

void CChunk::createVoxelBuffer(std::vector<glm::vec4> *vertices,
                               std::vector<GLuint> *indices)
{

    // iterate over all the existing blocks in the environment
    for(int i = m_Xmin; i < m_Xmax; i++) {
        for(int j = m_Ymin; j < m_Ymax; j++) {
            for(int k = m_Zmin; k < m_Zmax; k++) {

                if(mWorld->hasVoxelAt(i, j, k)) {
                    if(mWorld->voxelAtIsType(i, j, k) == CVoxel::NONEMPTY)
                    {

                        glm::vec4 color = mWorld->voxelAtIsColor(i, j, k);

                        // define voxel vertices
                        glm::vec4 v000 = glm::vec4(i, j, k, 1);
                        glm::vec4 v001 = glm::vec4(i, j, k+1, 1);
                        glm::vec4 v010 = glm::vec4(i, j+1, k, 1);
                        glm::vec4 v100 = glm::vec4(i+1, j, k, 1);
                        glm::vec4 v011 = glm::vec4(i, j+1, k+1, 1);
                        glm::vec4 v101 = glm::vec4(i+1, j, k+1, 1);
                        glm::vec4 v110 = glm::vec4(i+1, j+1, k, 1);
                        glm::vec4 v111 = glm::vec4(i+1, j+1, k+1, 1);

                        // check if each face should be drawn
                        for(int f = 0; f < 6; f++)
                        {
                            checkFace(&v000, &v001, &v010, &v100, &v011, &v101, &v110, &v111, f, i, j, k, color, vertices, indices);
                        }

                    }
                }

            }
        }
    }

}

void CChunk::create()
{
    std::vector<glm::vec4> vertices;
    std::vector<GLuint> indices;

    //populates the vertices and indices buffers
    createVoxelBuffer(&vertices, &indices);

    count = indices.size();
    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
                          sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generateVertData();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufVertData);
    context->glBufferData(GL_ARRAY_BUFFER, vertices.size() *
                          sizeof(glm::vec4), vertices.data(), GL_STATIC_DRAW);


}
