#ifndef FILEWORKER_H
#define FILEWORKER_H


#include <QtCore/QObject>
#include "filecollector.h"
#include "filedistributor.h"

class fileworker : public QObject {
Q_OBJECT
public:
    fileworker();

    void setCollector(filecollector *collector);

    void setDistributor(filedistributor *distributor);

    void reset();
public slots:

    void doWork();
private:
    filecollector *collector;
    filedistributor *distributor;
};

#endif // FILEWORKER_H