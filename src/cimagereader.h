#ifndef CIMAGEREADER_H
#define CIMAGEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>

#include "ppm.h"


// This class takes in an image file and stores it in a 2D array of pixels

class CImageReader
{
public:

    CImageReader();
    void readPPM(const char *filepath);
    img_t *getImageArray();

    void destroyPPM();

private:
    PPM *ppm;
    img_t *mIMG;

    void destroy_mIMG();



};

#endif // CIMAGEREADER_H
