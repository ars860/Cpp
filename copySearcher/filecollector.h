#ifndef FILECOLLECTOR_H
#define FILECOLLECTOR_H

#include <QDir>
#include <QFile>
#include <QVector>

class filecollector : public QObject {
Q_OBJECT
public:
    filecollector();

    ~filecollector();

    void reset();

    void set_directory(QString directory_name);

    QVector<QFile *> get();

    void collect_rec();

public slots:

    void collect();

signals:

    void addError(QString);

private:
    QDir current_directory;
    QVector<QFile*> files;
};

#endif // FILECOLLECTOR_H
