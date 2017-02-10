#include "voxelizer.h"

Voxelizer::Voxelizer()
{

}

Voxelizer::Voxelizer(img_t* slice, double z)
{
    mSlice = slice;
    mLength = z;
    mVoxelPlane = new std::vector<Voxel*>();
}

void Voxelizer::voxelizeImageSlice() {
    // Each plane's (0,0) should correspond to (0, 0, z) in world space
    // The "z" value will vary to allow "stacking" of planes

    int height = mSlice->h;
    int width = mSlice->w;
    const int size = height * width;
    int count = 0; // This keeps track of pixel address in 1D array


    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            if(count < size) {
                pixel_t currPixel = mSlice->data[count];

                float red = (int) currPixel.r;
                float green = (int) currPixel.g;
                float blue = (int) currPixel.b;


                // Black pixels represent empty space
                // If the pixel isn't black, create a voxel
                // Store this voxel in the mVoxelPlane vector
                if(red + green + blue > 0) {
                    Voxel* currVoxel = new Voxel();
                    currVoxel->set2DAddress(row, col, count);
                    currVoxel->setLengthAddress(mLength);
                    float red = (int)currPixel.r / 255.f;
                    float green = (int)currPixel.g / 255.f;
                    float blue = (int)currPixel.b / 255.f;
                    currVoxel->setColor(glm::vec4(red, green, blue, 1));
                    mVoxelPlane->push_back(currVoxel);
                }
                count++;
            }
        }
    }
}

std::vector<Voxel*> *Voxelizer::getVoxelPlane(){
    return mVoxelPlane;
}
