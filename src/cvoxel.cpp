#include "cvoxel.h"

CVoxel::CVoxel() : pos(glm::vec4(0.f, 0.f, 0.f, 1.f)), color(glm::vec4(.5f, .5f, .5f, 1.f)), ID(-1)
{
}

CVoxel::CVoxel(glm::vec4 position, glm::vec4 color, int id) : pos(position), color(color), ID(id)
{
}
