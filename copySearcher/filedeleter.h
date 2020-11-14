#ifndef FILEDELETER_H
#define FILEDELETER_H


#include <QtCore/QObject>
#include <QtWidgets/QTreeWidgetItem>
#include <QtCore/QFile>
#include <set>
#include "filecollector.h"

class filedeleter : public QObject {
Q_OBJECT
public:
    filedeleter();

    void reset();

public slots:

    void addFile(QTreeWidgetItem *item);

    void deleteFiles();

signals:

    void toggleChecked(QTreeWidgetItem *item);

    void hideRow(QTreeWidgetItem *item);

    void filesDeleted();

    void addError(QString);
private:
    QSet<QTreeWidgetItem *> items;
};

#endif // FILEDELETER_H