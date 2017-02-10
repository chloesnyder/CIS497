#include "cdicomreader.h"



CDicomReader::CDicomReader()
{

}

void CDicomReader::readFile(QString filePath)
{
    const char *path = filePath.toStdString().c_str();
    const char *hardcodePath = "/Users/chloebrownsnyder/Desktop/Spring2017/CIS497/testimage.dcm";

//  OFFile dicomFile = OFFile::fopen(hardcodePath, "r");
 // DcmFileFormat fileformat;
 // OFCondition status = fileformat.loadFile(hardcodePath);

  //DcmDataset *dataset = fileformat.getDataset();


    //DicomImage *img = new DicomImage(hardcodePath);
    //int width = img->getWidth();
    //int height = img->getHeight();
    //unsigned long imgSize = img->getOutputDataSize();

}
