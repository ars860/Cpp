#include <QtCore/QThread>
#include "fileworker.h"

fileworker::fileworker() = default;

void fileworker::setCollector(filecollector *collector) {
    this->collector = collector;
}

void fileworker::setDistributor(filedistributor *distributor) {
    this->distributor = distributor;
}

void fileworker::reset() {
    collector->reset();
    distributor->reset();
}

void fileworker::doWork() {
    collector->collect();

    distributor->setFiles(collector->get());
    distributor->sort_by_size();

    QThread::currentThread()->quit();
}
