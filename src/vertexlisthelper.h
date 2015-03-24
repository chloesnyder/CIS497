#ifndef VERTEXLISTHELPER_H
#define VERTEXLISTHELPER_H
#include "vertex.h"
#include <QListWidgetItem>

class vertexlisthelper : public QListWidget
{
    Q_OBJECT
private:
    QWidget* list_head;
public:
    vertexlisthelper(QWidget* head);
    ~vertexlisthelper();
public slots:
    void slot_storeVertList(QListWidgetItem*);
    void slot_storeSelectedVertex(QListWidgetItem*);
};

#endif // VERTEXLISTHELPER_H
