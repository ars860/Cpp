#ifndef FILEDISTRIBUTOR_H
#define FILEDISTRIBUTOR_H


#include <QtCore/QVector>
#include <QtCore/QFile>

class filedistributor : public QObject {
Q_OBJECT
public:
public slots:

    void sort_by_size();

    void setFiles(QVector<QFile *> files);

    QVector<QFile *> getFiles();

    QVector<QVector<QFile *>> getGroups();

    void reset();

signals:

    void sorted();

    void updateStatsCounter(int);

private:
    QVector<QFile *> files;
    QVector<QVector<QFile *>> groups;
};

#endif // FILEDISTRIBUTOR_H