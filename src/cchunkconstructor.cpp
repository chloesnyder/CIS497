#include "cchunkconstructor.h"


// USED CODE FROM HERE: http://paulbourke.net/geometry/polygonise/, credit to Trung Le for showing me!

/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
    0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/


CChunkConstructor::CChunkConstructor(GLWidget277* context, std::vector<std::vector<CVoxel*>*> *mAllLayers)
    : Drawable(context), mAllLayers(mAllLayers)

{

}

float CChunkConstructor::getAlphaAtVoxel(int x, int z, std::vector<CVoxel*>* voxelLayer)
{
    // If this "layer" is below our first layer or above our last layer in the overall structure,
    // then there's nothing there and the density is 0
    // Otherwise, return the alpha value of the color of this voxel
    if(voxelLayer->empty())
    {
        return 1;
    } else  {
        int idx = m_Xmax * x + z;
        if(idx < 0 || idx > voxelLayer->size())
        {
            return 1;
        }
        CVoxel* voxel = voxelLayer->at(idx);
        return voxel->getColor().a;
    }

}

double CChunkConstructor::calculateDensity(glm::vec4 vertex)
{
    // Use trilinear interpolation to determine the density at this vertex
    // fix the x value from the point you sample from
    // 4 times along x (so from each of the 8 corners, lerp along 4 pairs of corners x1->x2 x3->x4 x5->x6 x7->x8)
    // 2 times along y
    // 1 time along z

    // let xd, yd, and zd be the differences between each of the x,y,z, and the
    // smaller coordinate related, where x0 indicates the lattice point below x,
    // and x1 indicates the lattice point above x. similarly for y0,y1,z0,z1
    float x0 = vertex.x - 1; float x1 = vertex.x + 1;
    float y0 = vertex.y - 1; float y1 = vertex.y + 1;
    float z0 = vertex.z - 1; float z1 = vertex.z + 1;

    float xd = (vertex.x - x0)/(x1 - x0);
    float yd = (vertex.y - y0)/(y1 - y0);
    float zd = (vertex.z - z0)/(z1 - z0);

    // convert the floats to the nearest int
    int ix0 = (int) std::round(x0); int iy0 = (int) std::round(y0); int iz0 = (int) std::round(z0);
    int ix1 = (int) std::round(x1); int iy1 = (int) std::round(y1); int iz1 = (int) std::round(z1);
    int ix = (int) std::round(vertex.x); int iy = (int) std::round(vertex.y); int iz = (int) std::round(vertex.z);

    // get the layer that we are currently on, the layer above and the layer below
    // layers are in the y direction
    std::vector<CVoxel*>* iy0Layer = new std::vector<CVoxel*>();
    std::vector<CVoxel*>* iy1Layer = new std::vector<CVoxel*>();

    if(iy0 > 0 && iy0 < mAllLayers->size())
    {
        iy0Layer = mAllLayers->at(iy0);
    }

    if(iy1 < mAllLayers->size() && iy1 > 0)
    {
        iy1Layer = mAllLayers->at(iy1);
    }


    float Vx0y0z0 = getAlphaAtVoxel(ix0, iz0, iy0Layer); //mWorld->voxelAtIsColor(ix0, iy0, iz0).a;
    float Vx1y0z0 = getAlphaAtVoxel(ix1, iz0, iy0Layer); //mWorld->voxelAtIsColor(ix1, iy0, iz0).a;
    float Vx0y0z1 = getAlphaAtVoxel(ix0, iz1, iy0Layer); //mWorld->voxelAtIsColor(ix0, iy0, iz1).a;
    float Vx1y0z1 = getAlphaAtVoxel(ix1, iz1, iy0Layer);//mWorld->voxelAtIsColor(ix1, iy0, iz1).a;
    float Vx0y1z0 = getAlphaAtVoxel(ix0, iz0, iy1Layer);//mWorld->voxelAtIsColor(ix0, iy1, iz0).a;
    float Vx1y1z0 = getAlphaAtVoxel(ix1, iz0, iy1Layer);//mWorld->voxelAtIsColor(ix1, iy1, iz0).a;
    float Vx0y1z1 = getAlphaAtVoxel(ix0, iz1, iy1Layer);//mWorld->voxelAtIsColor(ix0, iy1, iz1).a;
    float Vx1y1z1 = getAlphaAtVoxel(ix1, iz1, iy1Layer);//mWorld->voxelAtIsColor(ix1, iy1, iz1).a;

    float c00 = Vx0y0z0*(1 - xd) + Vx1y0z0*xd;
    float c01 = Vx0y0z1*(1 - xd) + Vx1y0z1*xd;
    float c10 = Vx0y1z0*(1 - xd) + Vx1y1z0*xd;
    float c11 = Vx0y1z1*(1 - xd) + Vx1y1z1*xd;

    float c0 = c00*(1 - yd) + c10*yd;
    float c1 = c01*(1 - yd) + c11*yd;

    float c = c0*(1 - zd) + c1*zd;

    return c;
}

/*
 * The normal can be computed easily, by taking the gradient of the density function
 * (the partial derivative, or independent rate of change, in the x, y, and z directions)
 * and then normalizing the resulting vector. This is easily accomplished by sampling the density volume six times.
 * To determine the rate of change in x, we sample the density volume at the next texel in the +x direction,
 * then again at the next texel in the -x direction, and take the difference; this is the rate of change in x.
 *  We repeat this calculation in the y and z directions, for a total of six samples.
 * The three results are put together in a vec4, and then normalized, producing a very
 * high quality surface normal that can later be used for lighting.
 * */
glm::vec4 CChunkConstructor::calculateNormal(glm::vec4 vertex, int totalTris)
{
    glm::vec4 grad;

    //float d = 1.0/(float)voxels_per_block -> so for me 1/(512*512)? thats so close to 0...
    float d = 1.0;//totalTris;

    //   d = 1/totalTris;

    grad.x = calculateDensity(vertex + glm::vec4(d, 0, 0, 1)) -
            calculateDensity(vertex + glm::vec4(-d, 0, 0, 1));
    grad.y = calculateDensity(vertex + glm::vec4(0, d, 0, 1)) -
            calculateDensity(vertex + glm::vec4(0, -d, 0, 1));
    grad.z = calculateDensity(vertex + glm::vec4(0, 0, d, 1)) -
            calculateDensity(vertex + glm::vec4(0, 0, -d, 1));
    grad.w = 0;

    return -glm::normalize(grad);
}

glm::vec4 CChunkConstructor::VertexInterp(double isolevel, glm::vec4 p1, glm::vec4 p2, double valp1, double valp2)
{
    double mu;
    glm::vec4 p;

    if (std::fabs(isolevel-valp1) < 0.00001)
        return(p1);
    if (std::fabs(isolevel-valp2) < 0.00001)
        return(p2);
    if (std::fabs(valp1-valp2) < 0.00001)
        return(p1);
    mu = (isolevel - valp1) / (valp2 - valp1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);
    p.a = 1;

    return(p);
}

int CChunkConstructor::Polygonise(GRIDCELL grid, double isolevel, std::vector<TRIANGLE> &triangles)
{

    int i, ntriang;
    int cubeindex;
    glm::vec4 vertlist[12];

    /*
         Determine the index into the edge table which
         tells us which vertices are inside of the surface
      */
    cubeindex = 0;
    if (grid.val[0] < isolevel) cubeindex |= 1;
    if (grid.val[1] < isolevel) cubeindex |= 2;
    if (grid.val[2] < isolevel) cubeindex |= 4;
    if (grid.val[3] < isolevel) cubeindex |= 8;
    if (grid.val[4] < isolevel) cubeindex |= 16;
    if (grid.val[5] < isolevel) cubeindex |= 32;
    if (grid.val[6] < isolevel) cubeindex |= 64;
    if (grid.val[7] < isolevel) cubeindex |= 128;

    /* Cube is entirely in/out of the surface */
    if (edgeTable[cubeindex] == 0)
        return(0);

    int edge = edgeTable[cubeindex];

    /* Find the vertices where the surface intersects the cube */
    if (edgeTable[cubeindex] & 1)
        vertlist[0] =
                VertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
    if (edgeTable[cubeindex] & 2)
        vertlist[1] =
                VertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
    if (edgeTable[cubeindex] & 4)
        vertlist[2] =
                VertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
    if (edgeTable[cubeindex] & 8)
        vertlist[3] =
                VertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
    if (edgeTable[cubeindex] & 16)
        vertlist[4] =
                VertexInterp(isolevel,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
    if (edgeTable[cubeindex] & 32)
        vertlist[5] =
                VertexInterp(isolevel,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
    if (edgeTable[cubeindex] & 64)
        vertlist[6] =
                VertexInterp(isolevel,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
    if (edgeTable[cubeindex] & 128)
        vertlist[7] =
                VertexInterp(isolevel,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
    if (edgeTable[cubeindex] & 256)
        vertlist[8] =
                VertexInterp(isolevel,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
    if (edgeTable[cubeindex] & 512)
        vertlist[9] =
                VertexInterp(isolevel,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
    if (edgeTable[cubeindex] & 1024)
        vertlist[10] =
                VertexInterp(isolevel,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
    if (edgeTable[cubeindex] & 2048)
        vertlist[11] =
                VertexInterp(isolevel,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

    /* Create the triangle */
    ntriang = 0;
    for (i=0;triTable[cubeindex][i]!=-1;i+=3) {
        TRIANGLE t;

        int triT = triTable[cubeindex][i];
        int triT2 = triTable[cubeindex][i+1];
        int triT3 = triTable[cubeindex][i+2];

        t.p[0] = vertlist[triTable[cubeindex][i  ]];
        t.p[1] = vertlist[triTable[cubeindex][i+1]];
        t.p[2] = vertlist[triTable[cubeindex][i+2]];
        triangles.push_back(t);
        ntriang++;
    }

    return(ntriang);

}

void CChunkConstructor::create()
{
    count = mIndices.size();
    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() *
                          sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    generateVertData();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufVertData);
    context->glBufferData(GL_ARRAY_BUFFER, mVertices.size() *
                          sizeof(glm::vec4), mVertices.data(), GL_STATIC_DRAW);
}


void CChunkConstructor::populateVoxelBuffer()
{
    //populates the vertices and indices buffers
    createVoxelBuffer(&mVertices, &mIndices);
}

void CChunkConstructor::createVoxelBuffer(std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices)
{

    int totalNumVoxels = 0;
    // iterate over all the existing blocks in the environment
    for(int i = m_Xmin; i < m_Xmax; i++) {
        // <= or <?
        for(int j = m_Ymin; j < m_Ymax; j++) {


            // get the layer that we are currently on, the layer above and the layer below
            // layers are in the y direction
            std::vector<CVoxel*>* currVoxelLayer = mAllLayers->at(j);
            std::vector<CVoxel*>* layerBelow = new std::vector<CVoxel*>();
            std::vector<CVoxel*>* layerAbove = new std::vector<CVoxel*>();

            if(j - 1 > 0)
            {
                layerBelow = mAllLayers->at(j - 1);
            }

            if(j + 1 < mAllLayers->size())
            {
                layerAbove = mAllLayers->at(j + 1);
            }

            for(int k = m_Zmin; k < m_Zmax; k++) {

                // The current voxel layer represents one image slice
                // It is a 1D vector of a 2D image array, so indexing is as follows: width * row + column
                int index = m_Xmax * i + k;
                CVoxel* currVoxel = currVoxelLayer->at(index);
                glm::vec4 color = currVoxel->getColor();

                //glm::vec4 position = currVoxel->getPosition();

                //For each voxel, polygonise it
                // define voxel verticies
                glm::vec4 v000 = glm::vec4(i, j, k, 1);
                glm::vec4 v001 = glm::vec4(i+1, j, k, 1);
                glm::vec4 v010 = glm::vec4(i+1, j, k+1, 1);
                glm::vec4 v100 = glm::vec4(i, j, k+1, 1);
                glm::vec4 v011 = glm::vec4(i, j+1, k, 1);
                glm::vec4 v101 = glm::vec4(i+1, j+1, k, 1);
                glm::vec4 v110 = glm::vec4(i+1, j+1, k+1, 1);
                glm::vec4 v111 = glm::vec4(i, j+1, k+1, 1);

                // assign vertices to the 8 corners of this grid cell
                GRIDCELL currCell = GRIDCELL();
                currCell.p[0] = v000; currCell.p[1] = v001; currCell.p[2] = v010; currCell.p[3] = v100;
                currCell.p[4] = v011; currCell.p[5] = v101; currCell.p[6] = v110; currCell.p[7] = v111;

                int hasDensity = false;

                // define densities at each vertex as the alpha value of the voxel at that vertex location
                for(int corner = 0; corner < 8; corner++) {

                    // sample the corners as a lerped value between neighboring voxels
                    int x = currCell.p[corner].x; int y = currCell.p[corner].y; int z = currCell.p[corner].z;
                    int cornerIdx = m_Xmax * x + z;

                    float density;
                    CVoxel* voxelAtCorner;


                    // check the value of y so we know which layer to look in to get the voxel at this corner
                    if(y == j) {
                        if(cornerIdx > 0 && cornerIdx < currVoxelLayer->size()){
                            voxelAtCorner = currVoxelLayer->at(cornerIdx);
                        }
                    } else if (y == j - 1) {
                        if(!layerBelow->empty())
                        {
                            if(cornerIdx > 0 && cornerIdx < layerBelow->size()){
                                voxelAtCorner = layerBelow->at(cornerIdx);
                            }
                        }
                    } else if (y == j + 1) {
                        if(!layerAbove->empty())
                        {
                            if(cornerIdx > 0 && cornerIdx < layerAbove->size()){
                                voxelAtCorner = layerAbove->at(cornerIdx);
                            }
                        }
                    }

                    // get the color from the voxel at this corner, and from that the alpha value which is density
                    if(voxelAtCorner != nullptr)
                    {
                        density = voxelAtCorner->getColor().a;
                    } else {
                        density = 1;
                    }

                    currCell.val[corner] = density;

                    if(density > 0)
                    {
                        hasDensity = true;
                    }
                }

                if(hasDensity){

                    std::vector<TRIANGLE> currTriangles = std::vector<TRIANGLE>();
                    double currIsolevel = .01; //.01; // TODO: PLAY WITH THIS!!! what's a good value for my isolevel?

                    int totalTris = Polygonise(currCell, currIsolevel, currTriangles);

                    unsigned int indexCount = offset;

                    // push back vertex and index data
                    for(int u = 0; u < totalTris; u++)
                    {

                        for(int v = 0; v < 3; v++) {

                            glm::vec4 normal = calculateNormal(currTriangles[u].p[v], totalTris);

                            vertices->push_back(currTriangles.at(u).p[v]); // push back first vertex of this triangle
                            vertices->push_back(color); // then the color
                            vertices->push_back(normal); // then the normal

                            indices->push_back(indexCount);
                            indexCount++;
                        }
                        offset += 3;
                    }

                }
                totalNumVoxels++;
            }
        }
    }
}
