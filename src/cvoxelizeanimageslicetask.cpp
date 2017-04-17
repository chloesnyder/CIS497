#include "cvoxelizeanimageslicetask.h"

CVoxelizeAnImageSliceTask::CVoxelizeAnImageSliceTask()
{

}

CVoxelizeAnImageSliceTask::CVoxelizeAnImageSliceTask(img_t* slice, int length,
                                                     std::vector<std::vector<CVoxel*>*> *allLayers)
{
    mSlice = slice;
    mLength = length;
    if(mLength == 0)
    {
        mLength = 1;
    }
    mAllLayers = allLayers;

}


// One thread of this loads the content of this task's image slice into a voxel plane
// Then adds the voxel plane to the "allLayers" vector of vocel planes
void CVoxelizeAnImageSliceTask::run()
{

    std::vector<CVoxel*> *voxelPlane = new std::vector<CVoxel*>();
    std::vector<CVoxel*> *bufferVoxelPlane = new std::vector<CVoxel*>();


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


                if(densityThreshold == 0) //default, all. alpha > .25
                {
                    // This option renders everything
                    // Black pixels represent empty space
                    // If the pixel isn't black, create a voxel
                    // Store this voxel in the mVoxelPlane vector

                    glm::vec4 position = glm::vec4(row, mLength, col, 1);
                    glm::vec4 color = glm::vec4(red, green, blue, alpha);
                    CVoxel* currVoxel = new CVoxel(position, color, count);
                    voxelPlane->push_back(currVoxel);


                    // we are at first image, so we need to build our "buffer layer" too
                    if(mLength == 1) {
                        glm::vec4 bufferPos = glm::vec4(row, 0, col, 1);
                        glm::vec4 bufferCol = glm::vec4(red, green, blue, .009);
                        CVoxel* bufferVoxel = new CVoxel(bufferPos, bufferCol, count);
                        bufferVoxelPlane->push_back(bufferVoxel);
                    }


                } else if (densityThreshold == 1) // bone
                {
                    if(alpha > .7225) {
                        glm::vec4 position = glm::vec4(row, mLength, col, 1);
                        glm::vec4 color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);
                    }

                } else if (densityThreshold == 2) // muscle, organs
                {
                    float minMuscleDensity = .45;
                    float maxMuscleDensity = .7;
                    if(alpha > minMuscleDensity && alpha < maxMuscleDensity) {
                        glm::vec4 position = glm::vec4(row, mLength, col, 1);
                        glm::vec4 color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);
                    }

                } else if (densityThreshold == 3) // lung, fat, other low density tissue
                {
                    float minLungDensity = 0.01f;
                    float maxLungDensity = 0.4f;
                    if(alpha > minLungDensity && alpha < maxLungDensity) {
                        glm::vec4 position = glm::vec4(row, mLength, col, 1);

                        glm::vec4 color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);
                    }

                }


                count++;
            }
        }
    }

    if(mLength == 1)
    {
        mAllLayers->push_back(bufferVoxelPlane);
    }

    mAllLayers->push_back(voxelPlane);
}

