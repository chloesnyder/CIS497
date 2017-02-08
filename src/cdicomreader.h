#ifndef CDICOMREADER_H
#define CDICOMREADER_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>

#include <stdlib.h>
#include <stdio.h>

#include "dcmtk_dcmimgle.h"


class CDicomReader
{
public:
    CDicomReader();

    // FOR NOW: Hardcode a path to a single file
    // IN THE FUTURE: Will want a way to iterate through a folder of files, grabbing each file path
    // and generating files for each file.
    void readFile(QString filePath);


};

#endif // CDICOMREADER_H
