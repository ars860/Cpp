#include <QtCore/QThread>
#include <QtCore/QFileInfo>
#include "filecomparer.h"

filecomparer::filecomparer(QVector<QFile *> files) {
    for (int i = 0; i < files.size(); i++) {
        this->files.push_back(file_hash(files[i]));
    }
}

void filecomparer::set_files(QVector<QFile *> files) {
    clear();
    for (int i = 0; i < files.size(); i++) {
        this->files.push_back(file_hash(files[i]));
    }
}

void filecomparer::setId(int id) {
    this->id = id;
}

void filecomparer::find_equal() {
    QVector<bool> used(files.size(), 0);
    for (int i = 0; i < files.size(); i++) {
        if (!used[i]) {
            res.push_back(QVector<QFile *>());
            used[i] = 1;
            file_hash current_file = files[i];
            QFileInfo currentFileInfo(*current_file.get_file());

            if (!currentFileInfo.isReadable()) {
                if (!currentFileInfo.permission(QFile::ReadUser)) {
                    emit addError(QString("don't have permissions to read file : '%1'").arg(
                            current_file.get_file()->fileName()));
                } else {
                    emit addError(QString("file : '%1' is not readable").arg(current_file.get_file()->fileName()));
                }
                continue;
            }

            res.back().push_back(current_file.get_file());

            for (int j = i; j < files.size(); j++) {
                if (!used[j]) {
                    QFileInfo filesJInfo(*files[j].get_file());


                    if (filesJInfo.isReadable()) {
                        if (files[j].compare(current_file)) {
                            res.back().push_back(files[j].get_file());
                            used[j] = 1;
                        }
                    } else {
                        if (!filesJInfo.permission(QFile::ReadUser)) {
                            emit addError(QString("don't have permissions to read file : '%1'").arg(
                                    files[j].get_file()->fileName()));
                        } else {
                            emit addError(QString("file : '%1' is not readable").arg(files[j].get_file()->fileName()));
                        }
                    }
                }
            }
            if (res.back().size() == 1) {
                res.pop_back();
            }
        }
    }

    emit compared(id);
}

QVector<QVector<QFile *>> filecomparer::getGroups() {
    return res;
}

void filecomparer::clear() {
    files.clear();
            foreach(QVector<QFile *> vec, res) {
            qDeleteAll(vec);
        }
}
