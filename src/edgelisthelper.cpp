#include "edgelisthelper.h"


edgelisthelper::edgelisthelper(QWidget *head) : QListWidget(head)
{
    list_head = head;
}

edgelisthelper::~edgelisthelper(){

}

void edgelisthelper::slot_storeEdgeList(QListWidgetItem * el){
    this->addItem(el);
}

void edgelisthelper::slot_storeSelectedEdge(QListWidgetItem *e){
    this->setCurrentItem(e);
}

void edgelisthelper::slot_removeHEList(QListWidgetItem* e) {
    this->removeItemWidget(e);
}
