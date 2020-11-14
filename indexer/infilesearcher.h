#ifndef INFILESEARCHER_H
#define INFILESEARCHER_H

#include <QObject>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <algorithm>
#include <functional>

class inFileSearcher : public QObject {
Q_OBJECT
public:
    inFileSearcher();

    inFileSearcher(QString fileName, QString input);

public slots:

    void findNext();

signals:

    void founded(int x, int y, QString fileName);

    void allFounded(QString fileName);
private:
    QString input;

    int x ;
    int y;
    int pos;
    QTextStream stream;
    QString buffer;
    QFile file;
};


#endif // INFILESEARCHER_H