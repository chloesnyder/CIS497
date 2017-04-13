#include "cvoxelizeanimageslicetask.h"

CVoxelizeAnImageSliceTask::CVoxelizeAnImageSliceTask()
{

}

CVoxelizeAnImageSliceTask::CVoxelizeAnImageSliceTask(img_t* slice, int length,
                                                     std::vector<std::vector<CVoxel*>*> *allLayers,
                                                     std::vector<int> *mMinXs, std::vector<int> *mMinZs,
                                                     std::vector<int> *mMaxXs, std::vector<int> *mMaxZs)
{
    mSlice = slice;
    mLength = length;
    mAllLayers = allLayers;
    minXForEachLayer = mMinXs;
    minZForEachLayer = mMinZs;
    maxXForEachLayer = mMaxXs;
    maxZForEachLayer = mMaxZs;

}

void CVoxelizeAnImageSliceTask::checkMaxAndMinXAndZ(int minXForThisLayer, int maxXForThisLayer, int minZForThisLayer, int maxZForThisLayer, int row, int column)
{
    if(minXForThisLayer > row)
    {
        minXForThisLayer = row;
    }

    if(minZForThisLayer > column)
    {
        minZForThisLayer = column;
    }

    if(maxXForThisLayer < row)
    {
        maxXForThisLayer = row;
    }

    if(maxZForThisLayer < column)
    {
        maxZForThisLayer = column;
    }
}


// One thread of this loads the content of this task's image slice into a voxel plane
// Then adds the voxel plane to the "allLayers" vector of vocel planes
void CVoxelizeAnImageSliceTask::run()
{

    std::vector<CVoxel*> *voxelPlane = new std::vector<CVoxel*>();

    int height = mSlice->h;
    int width = mSlice->w;
    const int size = height * width;
    int count = 0; // This keeps track of pixel address in 1D array

    // min x, max x
    int minXForThisLayer = 512;
    int maxXForThisLayer = 0;

    // min z, max z
    int minZForThisLayer = 512;
    int maxZForThisLayer = 0;


    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            if(count < size) {

                pixel_t currPixel = mSlice->data[count];

                // give value 0-1
                float red = currPixel.r / 255.f;
                float green = currPixel.g / 255.f;
                float blue = currPixel.b / 255.f;

                float alpha = (red + green + blue) / 3;

                glm::vec4 position;
                glm::vec4 color;

                if(densityThreshold == 0) //default, all. alpha > .25
                {
                    // This option renders everything
                    // Black pixels represent empty space
                    // If the pixel isn't black, create a voxel
                    // Store this voxel in the mVoxelPlane vector
                    if(alpha > .25) {
                        position = glm::vec4(row, mLength, col, 1);
                        color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);

                        minXForThisLayer = std::min(minXForThisLayer, row);
                        minZForThisLayer = std::min(minZForThisLayer, col);
                        maxXForThisLayer = std::max(maxXForThisLayer, row);
                        maxZForThisLayer = std::max(maxZForThisLayer, col);

                    }

                } else if (densityThreshold == 1) // bone
                {
                    if(alpha > .7225) {
                        position = glm::vec4(row, mLength, col, 1);
                        color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);

                        minXForThisLayer = std::min(minXForThisLayer, row);
                        minZForThisLayer = std::min(minZForThisLayer, col);
                        maxXForThisLayer = std::max(maxXForThisLayer, row);
                        maxZForThisLayer = std::max(maxZForThisLayer, col);

                    }

                } else if (densityThreshold == 2) // muscle, organs
                {
                    float minMuscleDensity = .45;
                    float maxMuscleDensity = .7;
                    if(alpha > minMuscleDensity && alpha < maxMuscleDensity) {
                        position = glm::vec4(row, mLength, col, 1);
                        color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);

                        minXForThisLayer = std::min(minXForThisLayer, row);
                        minZForThisLayer = std::min(minZForThisLayer, col);
                        maxXForThisLayer = std::max(maxXForThisLayer, row);
                        maxZForThisLayer = std::max(maxZForThisLayer, col);

                    }

                } else if (densityThreshold == 3) // lung, fat, other low density tissue
                {
                    float minLungDensity = 0.01f;
                    float maxLungDensity = 0.4f;
                    if(alpha > minLungDensity && alpha < maxLungDensity) {
                        position = glm::vec4(row, mLength, col, 1);
                        color = glm::vec4(red, green, blue, alpha);
                        CVoxel* currVoxel = new CVoxel(position, color, count);
                        voxelPlane->push_back(currVoxel);

                        minXForThisLayer = std::min(minXForThisLayer, row);
                        minZForThisLayer = std::min(minZForThisLayer, col);
                        maxXForThisLayer = std::max(maxXForThisLayer, row);
                        maxZForThisLayer = std::max(maxZForThisLayer, col);
                    }

                }

                count++;
            }
        }
    }

    mAllLayers->push_back(voxelPlane);

    minXForEachLayer->push_back(minXForThisLayer);
    minZForEachLayer->push_back(minZForThisLayer);
    maxXForEachLayer->push_back(maxXForThisLayer);
    maxZForEachLayer->push_back(maxZForThisLayer);
}

