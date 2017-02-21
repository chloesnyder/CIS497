#include "voxelizer.h"

Voxelizer::Voxelizer()
{

}

Voxelizer::Voxelizer(img_t* slice, double y)
{
    mSlice = slice;
    mLength = y;
    mVoxelPlane = new std::vector<CVoxel*>();
}

void Voxelizer::voxelizeImageSlice() {
    // Each plane's (0,0) should correspond to (0, y, 0) in world space
    // The "y" value will vary to allow "stacking" of planes

    int height = mSlice->h;
    int width = mSlice->w;
    const int size = height * width;
    int count = 0; // This keeps track of pixel address in 1D array


    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            if(count < size) {
                pixel_t currPixel = mSlice->data[count];

               // float red = (int) currPixel.r;
               // float green = (int) currPixel.g;
               // float blue = (int) currPixel.b;

                float red = currPixel.r / 255.f;
                float green = currPixel.g / 255.f;
                float blue = currPixel.b / 255.f;


                // Black pixels represent empty space
                // If the pixel isn't black, create a voxel
                // Store this voxel in the mVoxelPlane vector
                if(red + green + blue > 0) {
                   // glm::vec4 position = glm::vec4(row, mLength, col, 1);
                    glm::vec4 position = glm::vec4(row, mLength, col, 1);
                    glm::vec4 color = glm::vec4(red, green, blue, 1);
                    CVoxel* currVoxel = new CVoxel(position, color);
                    mVoxelPlane->push_back(currVoxel);
                }
                count++;
            }
        }
    }
}

std::vector<CVoxel *> *Voxelizer::getVoxelPlane(){
    return mVoxelPlane;
}

