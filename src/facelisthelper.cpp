#include "facelisthelper.h"

facelisthelper::facelisthelper(QWidget *head) : QListWidget(head)
{
    list_head = head;
}

facelisthelper::~facelisthelper(){

}

void facelisthelper::slot_storeFaceList(QListWidgetItem * fl){
    this->addItem(fl);
}

void facelisthelper::slot_storeSelectedFace(QListWidgetItem *f){
    this->setCurrentItem(f);
}

