#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    glm::vec4 p[3]; // XYZ
} TRIANGLE;

typedef struct {
    glm::vec4 p[8]; // 8 points in a cube
    double val[8];
} GRIDCELL;

#endif // STRUCTS_H
