#include <QtCore/QThread>
#include "filecollector.h"

filecollector::filecollector() {
    current_directory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
}

filecollector::~filecollector() {
    reset();
}

void filecollector::set_directory(QString path) {
    current_directory.setPath(path);
}

void filecollector::collect_rec() {
    QFileInfoList list = current_directory.entryInfoList();

    for (auto &file_info : list) {
        if (file_info.isFile()) {
            if (file_info.isReadable()) {
                files.push_back(new QFile(file_info.absoluteFilePath()));
            } else {
                if (!file_info.permission(QFile::ReadUser)) {
                    emit addError(
                            QString("don't have permissions to read file : '%1'").arg(file_info.absoluteFilePath()));
                } else {
                    emit addError(QString("file : '%1' is not readable").arg(file_info.absoluteFilePath()));
                }
            }
        }
        if (file_info.isDir()) {
            QDir old_dir = current_directory;
            set_directory(file_info.absoluteFilePath());
            collect_rec();
            current_directory = old_dir;
        }
    }
}

void filecollector::collect() {
    collect_rec();
}

QVector<QFile *> filecollector::get() {
    return files;
}

void filecollector::reset() {
    qDeleteAll(files);
    files.clear();
}
