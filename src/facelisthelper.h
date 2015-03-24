#ifndef FACELISTHELPER_H
#define FACELISTHELPER_H
#include "face.h"
#include <QListWidget>

class facelisthelper : public QListWidget
{
        Q_OBJECT
private:
    QWidget* list_head;
public:
    facelisthelper(QWidget* head);
    ~facelisthelper();

public slots:
    void slot_storeFaceList(QListWidgetItem*);
    void slot_storeSelectedFace(QListWidgetItem*);
};

#endif // FACELISTHELPER_H
