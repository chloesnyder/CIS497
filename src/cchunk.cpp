//#include "cchunk.h"

//int offset = 0;


//// USED CODE FROM HERE: http://paulbourke.net/geometry/polygonise/, credit to Trung Le for showing me!

///*
//   Given a grid cell and an isolevel, calculate the triangular
//   facets required to represent the isosurface through the cell.
//   Return the number of triangular facets, the array "triangles"
//   will be loaded up with the vertices at most 5 triangular facets.
//    0 will be returned if the grid cell is either totally above
//   of totally below the isolevel.
//*/

//int CChunk::Polygonise(GRIDCELL grid, double isolevel, std::vector<TRIANGLE> &triangles)
//{
//    int i, ntriang;
//    int cubeindex;
//    glm::vec4 vertlist[12];

//    int edgeTable[256]={
//        0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
//        0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
//        0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
//        0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
//        0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
//        0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
//        0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
//        0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
//        0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
//        0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
//        0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
//        0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
//        0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
//        0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
//        0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
//        0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
//        0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
//        0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
//        0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
//        0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
//        0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
//        0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
//        0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
//        0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
//        0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
//        0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
//        0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
//        0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
//        0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
//        0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
//        0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
//        0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };
//    int triTable[256][16] =
//    {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
//     {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
//     {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
//     {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
//     {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
//     {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
//     {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
//     {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
//     {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
//     {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
//     {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
//     {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
//     {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
//     {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
//     {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
//     {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
//     {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
//     {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
//     {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
//     {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
//     {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
//     {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
//     {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
//     {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
//     {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
//     {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
//     {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
//     {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
//     {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
//     {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
//     {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
//     {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
//     {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
//     {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
//     {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
//     {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
//     {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
//     {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
//     {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
//     {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
//     {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
//     {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
//     {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
//     {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
//     {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
//     {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
//     {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
//     {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
//     {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
//     {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
//     {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
//     {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
//     {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
//     {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
//     {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
//     {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
//     {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
//     {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
//     {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
//     {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
//     {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
//     {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
//     {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
//     {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
//     {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
//     {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
//     {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
//     {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
//     {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
//     {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
//     {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
//     {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
//     {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
//     {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
//     {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
//     {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
//     {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
//     {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
//     {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
//     {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
//     {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
//     {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
//     {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
//     {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
//     {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
//     {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
//     {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
//     {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
//     {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
//     {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
//     {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
//     {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
//     {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
//     {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
//     {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
//     {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
//     {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
//     {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
//     {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
//     {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
//     {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
//     {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
//     {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
//     {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
//     {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
//     {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
//     {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
//     {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
//     {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
//     {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
//     {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
//     {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
//     {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
//     {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
//     {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
//     {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
//     {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
//     {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
//     {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
//     {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
//     {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
//     {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
//     {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
//     {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
//     {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
//     {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
//     {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
//     {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
//     {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
//     {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
//     {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
//     {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
//     {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
//     {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
//     {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
//     {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
//     {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
//     {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
//     {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
//     {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
//     {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
//     {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
//     {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
//     {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
//     {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
//     {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
//     {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
//     {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
//     {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
//     {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
//     {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
//     {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
//     {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
//     {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
//     {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
//     {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
//     {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
//     {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
//     {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
//     {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
//     {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
//     {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
//     {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
//     {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
//     {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
//     {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
//     {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
//     {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
//     {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
//     {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
//     {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
//     {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
//     {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
//     {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
//     {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
//     {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
//     {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
//     {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};


//    /*
//         Determine the index into the edge table which
//         tells us which vertices are inside of the surface
//      */
//    cubeindex = 0;
//    if (grid.val[0] < isolevel) cubeindex |= 1;
//    if (grid.val[1] < isolevel) cubeindex |= 2;
//    if (grid.val[2] < isolevel) cubeindex |= 4;
//    if (grid.val[3] < isolevel) cubeindex |= 8;
//    if (grid.val[4] < isolevel) cubeindex |= 16;
//    if (grid.val[5] < isolevel) cubeindex |= 32;
//    if (grid.val[6] < isolevel) cubeindex |= 64;
//    if (grid.val[7] < isolevel) cubeindex |= 128;

//    /* Cube is entirely in/out of the surface */
//    if (edgeTable[cubeindex] == 0)
//        return(0);

//    int edge = edgeTable[cubeindex];

//    /* Find the vertices where the surface intersects the cube */
//    if (edgeTable[cubeindex] & 1)
//        vertlist[0] =
//                VertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
//    if (edgeTable[cubeindex] & 2)
//        vertlist[1] =
//                VertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
//    if (edgeTable[cubeindex] & 4)
//        vertlist[2] =
//                VertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
//    if (edgeTable[cubeindex] & 8)
//        vertlist[3] =
//                VertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
//    if (edgeTable[cubeindex] & 16)
//        vertlist[4] =
//                VertexInterp(isolevel,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
//    if (edgeTable[cubeindex] & 32)
//        vertlist[5] =
//                VertexInterp(isolevel,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
//    if (edgeTable[cubeindex] & 64)
//        vertlist[6] =
//                VertexInterp(isolevel,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
//    if (edgeTable[cubeindex] & 128)
//        vertlist[7] =
//                VertexInterp(isolevel,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
//    if (edgeTable[cubeindex] & 256)
//        vertlist[8] =
//                VertexInterp(isolevel,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
//    if (edgeTable[cubeindex] & 512)
//        vertlist[9] =
//                VertexInterp(isolevel,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
//    if (edgeTable[cubeindex] & 1024)
//        vertlist[10] =
//                VertexInterp(isolevel,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
//    if (edgeTable[cubeindex] & 2048)
//        vertlist[11] =
//                VertexInterp(isolevel,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

//    /* Create the triangle */
//    ntriang = 0;
//    for (i=0;triTable[cubeindex][i]!=-1;i+=3) {
//        TRIANGLE t;

//        int triT = triTable[cubeindex][i];
//        int triT2 = triTable[cubeindex][i+1];
//        int triT3 = triTable[cubeindex][i+2];

//        t.p[0] = vertlist[triTable[cubeindex][i  ]];
//        t.p[1] = vertlist[triTable[cubeindex][i+1]];
//        t.p[2] = vertlist[triTable[cubeindex][i+2]];
//        triangles.push_back(t);
//        ntriang++;
//    }

//    return(ntriang);
//}

//glm::vec4 CChunk::VertexInterp(double isolevel, glm::vec4 p1, glm::vec4 p2, double valp1, double valp2)
//{
//    double mu;
//    glm::vec4 p;

//    if (std::fabs(isolevel-valp1) < 0.00001)
//        return(p1);
//    if (std::fabs(isolevel-valp2) < 0.00001)
//        return(p2);
//    if (std::fabs(valp1-valp2) < 0.00001)
//        return(p1);
//    mu = (isolevel - valp1) / (valp2 - valp1);
//    p.x = p1.x + mu * (p2.x - p1.x);
//    p.y = p1.y + mu * (p2.y - p1.y);
//    p.z = p1.z + mu * (p2.z - p1.z);
//    p.a = 1;

//    return(p);
//}





//CChunk::CChunk(GLWidget277 *context)
//    : Drawable(context), mWorld(nullptr), m_Xmin(0), m_Xmax(0), m_Ymin(0), m_Ymax(0), m_Zmin(0), m_Zmax(0)
//{
//}

//CChunk::CChunk(GLWidget277* context, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) :
//    Drawable(context), mWorld(nullptr), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax)
//{

//}

//CChunk::CChunk(GLWidget277* context, CWorld* w, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
//    : Drawable(context), mWorld(w), m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax), m_Zmin(zmin), m_Zmax(zmax)
//{

//}

//void CChunk::pushBackVertData(glm::vec4 *a, glm::vec4 *b, glm::vec4 *c, glm::vec4 *d, glm::vec4 normal, glm::vec4 color,
//                              std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices)
//{
//    vertices->push_back(*a);
//    vertices->push_back(color);
//    vertices->push_back(normal);

//    vertices->push_back(*b);
//    vertices->push_back(color);
//    vertices->push_back(normal);

//    vertices->push_back(*c);
//    vertices->push_back(color);
//    vertices->push_back(normal);

//    vertices->push_back(*d);
//    vertices->push_back(color);
//    vertices->push_back(normal);

//    indices->push_back(offset);
//    indices->push_back(offset + 1);
//    indices->push_back(offset + 2);
//    indices->push_back(offset);
//    indices->push_back(offset + 2);
//    indices->push_back(offset + 3);
//    offset += 4;

//}

//void CChunk::checkFace(glm::vec4 *v000, glm::vec4 *v001, glm::vec4 *v010,
//                       glm::vec4 *v100, glm::vec4 *v011, glm::vec4 *v101,
//                       glm::vec4 *v110, glm::vec4 *v111, int face, int x, int y, int z,
//                       glm::vec4 color,
//                       std::vector<glm::vec4> *vertices, std::vector<GLuint> *indices)
//{
//    // Want to check all of my 6 faces to see whether or not I should render it
//    // If there is a voxel next to face x, I do not render face x. If there is nothing there, then I render face x.
//    glm::vec4 posXNor = glm::vec4(1,0,0,0);
//    glm::vec4 negXNor = glm::vec4(-1,0,0,0);
//    glm::vec4 posYNor = glm::vec4(0,1,0,0);
//    glm::vec4 negYNor = glm::vec4(0,-1,0,0);
//    glm::vec4 posZNor = glm::vec4(0,0,1,0);
//    glm::vec4 negZNor = glm::vec4(0,0,-1,0);


//    if(face == 0) {
//        if(!mWorld->hasVoxelAt(x, y, z + 1))
//        {
//            //  if(glm::dot(posZNor, mCameraForward) > 0) {
//            pushBackVertData(v111, v101, v001, v011, posZNor, color, vertices, indices);
//            //  }
//        }

//    } else if (face == 1) {
//        if(!mWorld->hasVoxelAt(x + 1, y, z))
//        {
//            //  if(glm::dot(posXNor, mCameraForward) > 0) {
//            pushBackVertData(v110, v100, v101, v111, posXNor, color, vertices, indices);
//            // }
//        }

//    } else if (face == 2) {
//        if(!mWorld->hasVoxelAt(x - 1, y, z))
//        {
//            // if(glm::dot(negXNor, mCameraForward) > 0) {
//            pushBackVertData(v011, v001, v000, v010, negXNor, color, vertices, indices);
//            // }
//        }

//    } else if (face == 3) {
//        if(!mWorld->hasVoxelAt(x, y, z - 1))
//        {
//            // if(glm::dot(negZNor, mCameraForward) > 0) {
//            pushBackVertData(v010, v000, v100, v110, negZNor, color, vertices, indices);
//            // }
//        }

//    } else if (face == 4) {
//        if(!mWorld->hasVoxelAt(x, y + 1, z))
//        {
//            // if(glm::dot(posYNor, mCameraForward) > 0) {
//            pushBackVertData(v110, v111, v011, v010, posYNor, color, vertices, indices);
//            // }
//        }

//    } else if (face == 5) {
//        if(!mWorld->hasVoxelAt(x, y - 1, z))
//        {
//            //if(glm::dot(negYNor, mCameraForward) > 0) {
//            pushBackVertData(v101, v100, v000, v001, negYNor, color, vertices, indices);
//            // }
//        }
//    }
//}



///*
// * The normal can be computed easily, by taking the gradient of the density function
// * (the partial derivative, or independent rate of change, in the x, y, and z directions)
// * and then normalizing the resulting vector. This is easily accomplished by sampling the density volume six times.
// * To determine the rate of change in x, we sample the density volume at the next texel in the +x direction,
// * then again at the next texel in the -x direction, and take the difference; this is the rate of change in x.
// *  We repeat this calculation in the y and z directions, for a total of six samples.
// * The three results are put together in a vec4, and then normalized, producing a very
// * high quality surface normal that can later be used for lighting.
// * */
//glm::vec4 CChunk::calculateNormal(glm::vec4 vertex, int totalTris)
//{

//    /*Essentially, the normal of the vertex would be the average of the adjacent faces' (triangles) normals.

//In pseudocode:

//for each face adjacent to vertex[n]
//    sum = sum + face.normal
//normal[n] = NORMALIZE(sum / COUNT(adjacent faces))
//Repeat for each vertex.

//*/

//    glm::vec4 grad;

//    //float d = 1.0/(float)voxels_per_block -> so for me 1/(512*512)? thats so close to 0...
//    float d = 1.0;//totalTris;

// //   d = 1/totalTris;

//    grad.x = calculateDensity(vertex + glm::vec4(d, 0, 0, 1)) -
//            calculateDensity(vertex + glm::vec4(-d, 0, 0, 1));
//    grad.y = calculateDensity(vertex + glm::vec4(0, d, 0, 1)) -
//            calculateDensity(vertex + glm::vec4(0, -d, 0, 1));
//    grad.z = calculateDensity(vertex + glm::vec4(0, 0, d, 1)) -
//            calculateDensity(vertex + glm::vec4(0, 0, -d, 1));
//    grad.w = 0;

//    return -glm::normalize(grad);


//}


//double CChunk::calculateDensity(glm::vec4 vertex)
//{
//    // Use trilinear interpolation to determine the density at this vertex
//    // fix the x value from the point you sample from
//    // 4 times along x (so from each of the 8 corners, lerp along 4 pairs of corners x1->x2 x3->x4 x5->x6 x7->x8)
//    // 2 times along y
//    // 1 time along z

//    // let xd, yd, and zd be the differences between each of the x,y,z, and the
//    // smaller coordinate related, where x0 indicates the lattice point below x,
//    // and x1 indicates the lattice point above x. similarly for y0,y1,z0,z1
//    float x0 = vertex.x - 1; float x1 = vertex.x + 1;
//    float y0 = vertex.y - 1; float y1 = vertex.y + 1;
//    float z0 = vertex.z - 1; float z1 = vertex.z + 1;

//    float xd = (vertex.x - x0)/(x1 - x0);
//    float yd = (vertex.y - y0)/(y1 - y0);
//    float zd = (vertex.z - z0)/(z1 - z0);

//    // convert the floats to the nearest int
//    int ix0 = (int) std::round(x0); int iy0 = (int) std::round(y0); int iz0 = (int) std::round(z0);
//    int ix1 = (int) std::round(x1); int iy1 = (int) std::round(y1); int iz1 = (int) std::round(z1);
//    int ix = (int) std::round(vertex.x); int iy = (int) std::round(vertex.y); int iz = (int) std::round(vertex.z);


//    float Vx0y0z0 = mWorld->voxelAtIsColor(ix0, iy0, iz0).a;
//    float Vx1y0z0 = mWorld->voxelAtIsColor(ix1, iy0, iz0).a;
//    float Vx0y0z1 = mWorld->voxelAtIsColor(ix0, iy0, iz1).a;
//    float Vx1y0z1 = mWorld->voxelAtIsColor(ix1, iy0, iz1).a;
//    float Vx0y1z0 = mWorld->voxelAtIsColor(ix0, iy1, iz0).a;
//    float Vx1y1z0 = mWorld->voxelAtIsColor(ix1, iy1, iz0).a;
//    float Vx0y1z1 = mWorld->voxelAtIsColor(ix0, iy1, iz1).a;
//    float Vx1y1z1 = mWorld->voxelAtIsColor(ix1, iy1, iz1).a;

//    float c00 = Vx0y0z0*(1 - xd) + Vx1y0z0*xd;
//    float c01 = Vx0y0z1*(1 - xd) + Vx1y0z1*xd;
//    float c10 = Vx0y1z0*(1 - xd) + Vx1y1z0*xd;
//    float c11 = Vx0y1z1*(1 - xd) + Vx1y1z1*xd;

//    float c0 = c00*(1 - yd) + c10*yd;
//    float c1 = c01*(1 - yd) + c11*yd;

//    float c = c0*(1 - zd) + c1*zd;

//    return c;

//}

//void CChunk::createVoxelBuffer(std::vector<glm::vec4> *vertices,
//                               std::vector<GLuint> *indices)
//{

//    int totalNumVoxels = 0;
//    // iterate over all the existing blocks in the environment
//    for(int i = m_Xmin; i < m_Xmax; i++) {
//        for(int j = m_Ymin; j <= m_Ymax; j++) {
//            for(int k = m_Zmin; k < m_Zmax; k++) {

//                glm::vec4 color = mWorld->voxelAtIsColor(i, j, k);

//                // For each voxel, polygonise it
//                // define voxel vertices
//                glm::vec4 v000 = glm::vec4(i, j, k, 1);
//                glm::vec4 v001 = glm::vec4(i+1, j, k, 1);
//                glm::vec4 v010 = glm::vec4(i+1, j, k+1, 1);
//                glm::vec4 v100 = glm::vec4(i, j, k+1, 1);
//                glm::vec4 v011 = glm::vec4(i, j+1, k, 1);
//                glm::vec4 v101 = glm::vec4(i+1, j+1, k, 1);
//                glm::vec4 v110 = glm::vec4(i+1, j+1, k+1, 1);
//                glm::vec4 v111 = glm::vec4(i, j+1, k+1, 1);

//                // assign vertices to the 8 corners of this grid cell
//                GRIDCELL currCell = GRIDCELL();
//                currCell.p[0] = v000; currCell.p[1] = v001; currCell.p[2] = v010; currCell.p[3] = v100;
//                currCell.p[4] = v011; currCell.p[5] = v101; currCell.p[6] = v110; currCell.p[7] = v111;


//                bool hasDensity = false;
//                // define densities at each vertex as the alpha value of the voxel at that vertex location
//                for(int corner = 0; corner < 8; corner++) {
//                    // sample the corners as a lerped value between neighboring voxels
//                    int x = currCell.p[corner].x; int y = currCell.p[corner].y; int z = currCell.p[corner].z;

//                    if(mWorld->hasVoxelAt(x,y,z)) {
//                        currCell.val[corner] = mWorld->voxelAtIsColor(x,y,z).a;
//                        hasDensity = true;
//                    } else {
//                        currCell.val[corner] = 0;
//                    }
//                }

//                if(hasDensity){

//                    std::vector<TRIANGLE> currTriangles = std::vector<TRIANGLE>();
//                    double currIsolevel = .25; //.01; // TODO: PLAY WITH THIS!!! what's a good value for my isolevel?

//                    int totalTris = Polygonise(currCell, currIsolevel, currTriangles);

//                    unsigned int indexCount = offset;

//                    // push back vertex and index data
//                    for(int u = 0; u < totalTris; u++)
//                    {

//                        for(int v = 0; v < 3; v++) {

//                            // calculate the normal for this vertex based on xyz gradient
//                            glm::vec4 v1 = currTriangles[u].p[1] - currTriangles[u].p[2];
//                            glm::vec4 v2 = currTriangles[u].p[0] - currTriangles[u].p[2];


//                            glm::vec4 normal = /*glm::vec4(glm::cross(glm::vec3(v1), glm::vec3(v2)),0) + */calculateNormal(currTriangles[u].p[v], totalTris);

//                            vertices->push_back(currTriangles.at(u).p[v]); // push back first vertex of this triangle
//                            vertices->push_back(color); // then the color
//                            vertices->push_back(normal); // then the normal

//                            indices->push_back(indexCount);
//                            indexCount++;
//                        }
//                        offset += 3;
//                    }

//                }
//                totalNumVoxels++;
//            }
//        }
//    }

//    //std::cout << "Total number of voxels: " + totalNumVoxels << std::endl;
//}


//void CChunk::populateVoxelBuffer()
//{
//    //populates the vertices and indices buffers
//    createVoxelBuffer(&mVertices, &mIndices);
//}


//void CChunk::create()
//{

//    count = mIndices.size();
//    generateIdx();
//    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
//    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() *
//                          sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

//    generateVertData();
//    context->glBindBuffer(GL_ARRAY_BUFFER, bufVertData);
//    context->glBufferData(GL_ARRAY_BUFFER, mVertices.size() *
//                          sizeof(glm::vec4), mVertices.data(), GL_STATIC_DRAW);


//}

//*/
