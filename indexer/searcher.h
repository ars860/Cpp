#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include "trigram.h"
#include "counter.h"
#include "infilesearcher.h"
#include <memory>
#include <QMap>
#include <QSet>
#include <QThread>
#include <QtWidgets/QProgressBar>

class searcher : public QObject {
Q_OBJECT
public:
    searcher();

    void setTrigrams(std::map<QString, std::unique_ptr<QVector<QPair<QString, QSet<trigram>>>>> *trigrams);

    void setInput(QString input);

    void set_dirs(QSet<QString> *dirs);

    void setProgressBar(QProgressBar *progressBar);

public slots:
    void doWork();

    void findNext();

    void catchFounded(int x, int y, QString fileName);

    void catchAllFounded();

    void hideProgressBar();

    void doWorkWithNextPortion(QString);
signals:

    void founded(int x, int y);

    void find();

    void allFounded();

    void hideShowUI(bool);

    void show_founded(int x, int y, QString fileName);

    void setProgressBaxMax(int);

    void showHideProgressBar(bool);
private:
    counter cnter;
    std::map<QString, std::unique_ptr<QVector<QPair<QString, QSet<trigram>>>>> *trigrams;

    QVector<QString> findInTrigrams();

    QSet<trigram> extractTrigrams();

    QVector<QString> extractAllFiles();

    QString input;
    QSet<QString> *dirs;
    QProgressBar *progressBar;
    QVector<QString> suitableFiles;
    int curr = 0;
};

#endif // SEARCHER_H