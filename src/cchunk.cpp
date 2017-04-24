#include "cchunk.h"




// USED CODE FROM HERE: http://paulbourke.net/geometry/polygonise/, credit to Trung Le for showing me!

/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
    0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/

int CChunk::Polygonise(GRIDCELL grid, double isolevel, std::vector<TRIANGLE> &triangles)
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

glm::vec4 CChunk::VertexInterp(double isolevel, glm::vec4 p1, glm::vec4 p2, double valp1, double valp2)
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





CChunk::CChunk(GLWidget277 *context)
    : Drawable(context), mWorld(nullptr), m_Xmin(0), m_Xmax(0), m_Ymin(0), m_Ymax(0), m_Zmin(0), m_Zmax(0), offset(0)
{
}

CChunk::CChunk(GLWidget277* context, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) :
    Drawable(context), mWorld(nullptr), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax), offset(0)
{

}

CChunk::CChunk(GLWidget277* context, CWorldArray* w, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
    : Drawable(context), mWorld(w), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax), offset(0)
{

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
glm::vec4 CChunk::calculateNormal(glm::vec4 vertex)
{

    glm::vec4 grad;

    //float d = 1.0/(float)voxels_per_block -> so for me 1/(512*512)? thats so close to 0...
    float d = 1.5;

    grad.x = calculateDensity(vertex + glm::vec4(d, 0, 0, 0)) -
            calculateDensity(vertex + glm::vec4(-d, 0, 0, 0));
    grad.y = calculateDensity(vertex + glm::vec4(0, d, 0, 0)) -
            calculateDensity(vertex + glm::vec4(0, -d, 0, 0));
    grad.z = calculateDensity(vertex + glm::vec4(0, 0, d, 0)) -
            calculateDensity(vertex + glm::vec4(0, 0, -d, 0));
    grad.w = 0;

    return -glm::normalize(grad);


}


double CChunk::calculateDensity(glm::vec4 vertex)
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


    float Vx0y0z0 = mWorld->voxelAtIsColor(ix0, iy0, iz0).a;
    float Vx1y0z0 = mWorld->voxelAtIsColor(ix1, iy0, iz0).a;
    float Vx0y0z1 = mWorld->voxelAtIsColor(ix0, iy0, iz1).a;
    float Vx1y0z1 = mWorld->voxelAtIsColor(ix1, iy0, iz1).a;
    float Vx0y1z0 = mWorld->voxelAtIsColor(ix0, iy1, iz0).a;
    float Vx1y1z0 = mWorld->voxelAtIsColor(ix1, iy1, iz0).a;
    float Vx0y1z1 = mWorld->voxelAtIsColor(ix0, iy1, iz1).a;
    float Vx1y1z1 = mWorld->voxelAtIsColor(ix1, iy1, iz1).a;

    float c00 = Vx0y0z0*(1 - xd) + Vx1y0z0*xd;
    float c01 = Vx0y0z1*(1 - xd) + Vx1y0z1*xd;
    float c10 = Vx0y1z0*(1 - xd) + Vx1y1z0*xd;
    float c11 = Vx0y1z1*(1 - xd) + Vx1y1z1*xd;

    float c0 = c00*(1 - yd) + c10*yd;
    float c1 = c01*(1 - yd) + c11*yd;

    float c = c0*(1 - zd) + c1*zd;

    return c;

}

void CChunk::createVoxelBuffer()
{

    /// TODO: Try to convert from map to array again. This didn't work well the first time
    /// but this may improve speed to access from an 1D array as opposed to a map



    int totalNumVoxels = 0;
    // iterate over all the existing blocks in the environment

   // for(int idx = 0; idx < mWorld->getSize(); idx++) {
    int maxIdxForChunk = mWorld->calculateIndex(m_Xmax, m_Ymax, m_Zmax);
    int minIdxForChunk = mWorld->calculateIndex(m_Xmin, m_Ymin, m_Zmin);
    for(int idx = minIdxForChunk; idx < maxIdxForChunk; idx++) {
        glm::vec3 idxIn3D = mWorld->to3D(idx);
        int i = idxIn3D.x;
        int j = idxIn3D.y;
        int k = idxIn3D.z;

        glm::vec4 color = mWorld->voxelAtIsColor(i, j, k);

        // For each voxel, polygonise it
        // define voxel vertices
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


        bool hasDensity = false;
        // define densities at each vertex as the alpha value of the voxel at that vertex location
        for(int corner = 0; corner < 8; corner++) {
            // sample the corners as a lerped value between neighboring voxels
            int x = currCell.p[corner].x; int y = currCell.p[corner].y; int z = currCell.p[corner].z;

            if(mWorld->hasVoxelAt(x,y,z)) {
                currCell.val[corner] = mWorld->voxelAtIsColor(x,y,z).a;
                if(currCell.val[corner] > .01) hasDensity = true;
            } else {
                currCell.val[corner] = 0;
            }

        }

        if(hasDensity){

            std::vector<TRIANGLE> currTriangles = std::vector<TRIANGLE>();
            double currIsolevel = mIsolevel;

            int totalTris = Polygonise(currCell, currIsolevel, currTriangles);


            unsigned int indexCount = offset;

            // push back vertex and index data
            for(int u = 0; u < totalTris; u++)
            {

                for(int v = 0; v < 3; v++) {

                    //glm::vec4 normal = glm::vec4();

                    // calculate the normal for this vertex based on xyz gradient
                    // if(j != m_Ymax - 1) {
                    //      normal = calculateNormal(currTriangles[u].p[v]);
                    //  } else {
                    //     glm::vec4 normal = -glm::vec4(glm::cross(glm::vec3(currTriangles[u].p[1] - currTriangles[u].p[2]),
                    //           glm::vec3(currTriangles[u].p[3] - currTriangles[u].p[2])), 0);
                    // }


                    glm::vec4 normal = calculateNormal(currTriangles[u].p[v]);
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
    // }

}



void CChunk::create()
{


    count = indices->size();
    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() *
                          sizeof(GLuint), indices->data(), GL_STATIC_DRAW);

    generateVertData();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufVertData);
    context->glBufferData(GL_ARRAY_BUFFER, vertices->size() *
                          sizeof(glm::vec4), vertices->data(), GL_STATIC_DRAW);

     mCreated = true;
}

void CChunk::exportVerticesAndIndicesToFile()
{

    QString filename = newFileName.append(".txt");
    QFile file(filename);

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        // load the file path to the ct scans associated with this
        stream << ctScans << endl;

        // load the max number of layers
        stream << m_Ymax << endl;

        // load in all vertices
        // first, load a "v" to denote start of vertices
        stream << "v" << endl;
        for(glm::vec4 vertex : *vertices)
        {
            std::string vert = std::to_string(vertex[0]) + ", " +  std::to_string(vertex[1]) + ", " +
                    std::to_string(vertex[2]) + ", " +  std::to_string(vertex[3]);
            stream << vert.c_str() << endl;
        }

        // load a "i" to denote start of indices
        stream << "i" << endl;
        for(GLuint index : *indices)
        {
            stream << index << endl;
        }
    }

    file.close();

}


