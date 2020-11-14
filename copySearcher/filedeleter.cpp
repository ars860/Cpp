#include <QtCore/QThread>
#include "filedeleter.h"

filedeleter::filedeleter() = default;

void filedeleter::reset() {
    qDeleteAll(items);
    items.clear();
}

void filedeleter::addFile(QTreeWidgetItem *item) {
    if (item->childCount() > 0) {
        for (size_t i = 0; i < item->childCount(); i++) {
            QTreeWidgetItem *child = item->child(i);

            if (!child->isHidden()) {
                if (items.find(item) != items.end() && items.find(child) != items.end()) {
                    emit toggleChecked(child);
                    items.erase(items.find(child));
                } else if (items.find(item) == items.end() && items.find(child) == items.end()) {
                    emit toggleChecked(child);
                    items.insert(child);
                }
            }
        }
    }

    if (items.find(item) != items.end()) {
        items.erase(items.find(item));
    } else {
        items.insert(item);
    }

    if (item->childCount() == 0 && items.find(item) == items.end() &&
        items.find(item->parent()) != items.end()) {
        items.erase(items.find(item->parent()));
        emit toggleChecked(item->parent());
    }

    emit toggleChecked(item);
}

void filedeleter::deleteFiles() {
    QVector<QTreeWidgetItem *> toDelete;
    QSet<QTreeWidgetItem *> notDelete;
    QVector<QTreeWidgetItem *> parents;
    for (auto &item : items) {
        if (item->childCount() == 0) {
            QFile fileToDelete(item->text(0));
            QFileInfo info(fileToDelete);
            if (info.permission(QFile::WriteUser)) {
//              fileToDelete.remove();
                emit hideRow(item);
                toDelete.push_back(item);
            } else {
                emit addError(QString("don't have permissions to remove file : '%1'").arg(fileToDelete.fileName()));
                notDelete.insert(item->parent());
            }
        } else {
            parents.push_back(item);
        }
    }

    for (auto &item : toDelete) {
        items.erase(items.find(item));
    }

    for(auto &item : parents){
        if(!notDelete.contains(item)){
            emit hideRow(item);
            items.erase(items.find(item));
        }
    }

    emit filesDeleted();
}
