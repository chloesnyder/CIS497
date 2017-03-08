#ifndef CVOXEL_H
#define CVOXEL_H
#include <la.h>

class CVoxel
{
public:
    CVoxel();
    CVoxel(glm::vec4 position, glm::vec4 color, int id);

    // Not sure if I will need this, but saving a voxel type?
    // Could be useful for bone vs air vs density?
    enum VTYPE {EMPTY, NONEMPTY};

    void setColor(glm::vec4 col) { color = col; }
    void setPosition(glm::vec4 position) { this->pos = position; }
    glm::vec4 getColor() { return color; }
    glm::vec4 getPosition() { return pos; }
    void setID(int id) {ID = id;}
    int getID() {return ID;}

private:
    glm::vec4 pos;
    glm::vec4 color;
    int ID;

};

#endif // CVOXEL_H
