#include "cimagereader.h"

CImageReader::CImageReader()
{
    ppm = new PPM();
}

void CImageReader::readPPM(const char *filepath) {
    img_t *img = ppm->read_ppm(filepath);
    mIMG = img;
}

void CImageReader::destroy_mIMG() {
    ppm->destroy_img(&mIMG);
    //delete ppm;
}

img_t *CImageReader::getImageArray() {
    return mIMG;
}

void CImageReader::destroyPPM(){
    delete ppm;
}

