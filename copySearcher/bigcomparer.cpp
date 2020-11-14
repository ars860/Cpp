#include <QtCore/QThread>
#include "bigcomparer.h"

bigComparer::bigComparer() = default;

bigComparer::~bigComparer(){
    reset();
}

void bigComparer::reset() {
    qDeleteAll(comparers);
    comparers.clear();
}

void bigComparer::addComparer(filecomparer *comparer) {
    comparers.push_back(comparer);
}

void bigComparer::compareAll() {
    for (auto &comparer : comparers) {
        comparer->find_equal();
    }

    QThread::currentThread()->quit();
}
