#ifndef EDGELISTHELPER_H
#define EDGELISTHELPER_H
#include "halfedge.h"
#include <QListWidget>

class edgelisthelper : public QListWidget
{
    Q_OBJECT
private:
    QWidget* list_head;
public:
    edgelisthelper(QWidget* head);
    ~edgelisthelper();

public slots:
    void slot_storeEdgeList(QListWidgetItem*);
    void slot_storeSelectedEdge(QListWidgetItem*);
    void slot_removeHEList(QListWidgetItem*);
};

#endif // EDGELISTHELPER_H
