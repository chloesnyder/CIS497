#include "vertexlisthelper.h"

vertexlisthelper::vertexlisthelper(QWidget* head) : QListWidget(head)
{
    list_head = head;
}

vertexlisthelper::~vertexlisthelper(){

}

void vertexlisthelper::slot_storeVertList(QListWidgetItem *vl){
    this->addItem(vl);
}

void vertexlisthelper::slot_storeSelectedVertex(QListWidgetItem *v){
    this->setCurrentItem(v);
}
