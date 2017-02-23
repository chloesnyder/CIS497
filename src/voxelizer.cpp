#include "voxelizer.h"

Voxelizer::Voxelizer()
{

}

Voxelizer::Voxelizer(std::vector<img_t*> *slices) : mSlices(slices)
{

}


// Voxelize each image in the image array, then store that voxelized plane in a vector of all planes
void Voxelizer::voxelizeAllImages() {
    // Each plane's (0,0) should correspond to (0, y, 0) in world space
    // The "y" value will vary to allow "stacking" of planes

    for(int i = 0; i < mSlices->size(); i++) {
        std::vector<CVoxel*> *voxelPlane = new std::vector<CVoxel*>();
        img_t* slice = mSlices->at(i);
        int height = slice->h;
        int width = slice->w;
        const int size = height * width;
        int count = 0; // This keeps track of pixel address in 1D array
        for(int row = 0; row < height; row++) {
            for(int col = 0; col < width; col++) {
                if(count < size) {
                    pixel_t currPixel = slice->data[count];

                    // give value 0-1
                    float red = currPixel.r / 255.f;
                    float green = currPixel.g / 255.f;
                    float blue = currPixel.b / 255.f;

                    float alpha = (red + green + blue) / 3;


                    // Black pixels represent empty space
                    // If the pixel isn't black, create a voxel
                    // Store this voxel in the mVoxelPlane vector
                    if(alpha > .25) {
                        glm::vec4 position = glm::vec4(row, i, col, 1);
                        glm::vec4 color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color);
                        voxelPlane->push_back(currVoxel);
                    }
                    count++;
                }
            }
        }
        mAllLayers->push_back(voxelPlane);
    }
}
