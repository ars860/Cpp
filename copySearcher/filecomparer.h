#ifndef FILECOMPARER_H
#define FILECOMPARER_H


#include <QtCore/QVector>
#include <QtCore/QFile>
#include "file_hash.h"

class filecomparer : public QObject {
Q_OBJECT
public:
    filecomparer() = default;

    filecomparer(QVector<QFile *> files);

    void set_files(QVector<QFile *> files);

    void setId(int id);

    void clear();

    QVector<QVector<QFile *>> getGroups();

public slots:

    void find_equal();

signals:

    void compared(int id);

    void addError(QString);
private:
    QVector<file_hash> files;
    QVector<QVector<QFile *>> res;
    int id;
};

#endif // FILECOMPARER_H