#ifndef TRIGRAMWORKER_H
#define TRIGRAMWORKER_H


#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QDir>
#include "trigramcalcer.h"
#include <memory>
#include <vector>

class trigramWorker : public QObject {
Q_OBJECT
public:
    trigramWorker();

    trigramWorker(QString dirName, int progressBarId);

    void addCalcer(trigramCalcer *calcer);

    QVector<QPair<QString, QSet<trigram>>> *doWork();

public slots:
    void passError(QString text);

    void calcTrigrams();

    void catch_cant_calc_trigrams();

signals:
    void addError(QString text);

    void cant_calc_trigrams();

    void updateProgress(int val, int id);

    void resetProgressBar(int val, int id);

    void showHideProgressBar(bool val, int id);

    void workDone(QString, QVector<QPair<QString, QSet<trigram>>> *trigrams);

private:
    std::vector<std::unique_ptr<trigramCalcer>> calcers;

    QDir dir;
    int progressBarId;

    int updateStep;
    int curCalcer;
    int prevUpdate;
};

#endif // TRIGRAMWORKER_H