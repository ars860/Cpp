#ifndef BIGCOMPARER_H
#define BIGCOMPARER_H


#include <QtCore/QObject>
#include "filecomparer.h"

class bigComparer : public QObject {
Q_OBJECT
public:
    bigComparer();

    ~bigComparer();

    void reset();

    void addComparer(filecomparer *comparer);

public slots:

    void compareAll();

private:
    QVector<filecomparer *> comparers;
};

#endif // BIGCOMPARER_H