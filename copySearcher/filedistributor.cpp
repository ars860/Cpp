#include <QtCore/QSet>
#include <set>
#include "filedistributor.h"
#include "filecomparer.h"
#include <QThread>

void filedistributor::sort_by_size() {
    std::set<QPair<qint64, QFile *>> set;

    for (int i = 0; i < files.size(); i++) {
        set.insert({files[i]->size(), files[i]});
    }

    groups.resize(1);
    QPair<qint64, QFile *> prev = {-1, nullptr};
    for (auto size_file : set) {
        if (prev.first != -1) {
            if (prev.first != size_file.first) {
                if (groups.back().size() == 1) {
                    groups.pop_back();
                }

                groups.push_back(QVector<QFile *>());
            }
        }

        groups.back().push_back(size_file.second);
        prev = size_file;
    }

    if (groups.back().size() < 2) {
        groups.pop_back();
    }

    emit sorted();
    emit updateStatsCounter(groups.size());
}

void filedistributor::setFiles(QVector<QFile *> files) {
    this->files = files;
}

void filedistributor::reset() {
    files.clear();
    groups.clear();
}

QVector<QFile *> filedistributor::getFiles() {
    return files;
}

QVector<QVector<QFile *>> filedistributor::getGroups() {
    return groups;
}
