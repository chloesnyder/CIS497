#include "cvoxelizeanimageslicetask.h"

CVoxelizeAnImageSliceTask::CVoxelizeAnImageSliceTask()
{

}

CVoxelizeAnImageSliceTask::CVoxelizeAnImageSliceTask(img_t* slice, int length,
                                                     std::vector<std::vector<CVoxel*>*> *allLayers)
{
    mSlice = slice;
    mLength = length;
    mAllLayers = allLayers;

}


// One thread of this loads the content of this task's image slice into a voxel plane
// Then adds the voxel plane to the "allLayers" vector of vocel planes
// allLayers is a pointer that all the concurrently running threads will be populating

// Question - can multiple threads push to the same vector if provided the same memory address?
void CVoxelizeAnImageSliceTask::run()
{

    std::vector<CVoxel*> *voxelPlane = new std::vector<CVoxel*>();

    int height = mSlice->h;
    int width = mSlice->w;
    const int size = height * width;
    int count = 0; // This keeps track of pixel address in 1D array
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            if(count < size) {

                pixel_t currPixel = mSlice->data[count];

                // give value 0-1
                float red = currPixel.r / 255.f;
                float green = currPixel.g / 255.f;
                float blue = currPixel.b / 255.f;

                float alpha = (red + green + blue) / 3;


                // Black pixels represent empty space
                // If the pixel isn't black, create a voxel
                // Store this voxel in the mVoxelPlane vector
                if(alpha > .25) {
                    glm::vec4 position = glm::vec4(row, mLength, col, 1);
                    glm::vec4 color = glm::vec4(red, green, blue, alpha);
                    CVoxel* currVoxel = new CVoxel(position, color, count);
                    voxelPlane->push_back(currVoxel);
                }
                count++;
            }
        }
    }
    mAllLayers->push_back(voxelPlane);
}

