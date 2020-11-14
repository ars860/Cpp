#include <QtCore/QThread>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QDirIterator>
#include "trigramworker.h"

trigramWorker::trigramWorker() {

}

trigramWorker::trigramWorker(QString dirName, int progressBarId) : dir(dirName), progressBarId(progressBarId) {

}

void trigramWorker::addCalcer(trigramCalcer *calcer) {
    calcers.push_back(std::unique_ptr<trigramCalcer>(calcer));
    connect(calcer, &trigramCalcer::cant_calc_trigrams, this, &trigramWorker::catch_cant_calc_trigrams);
    connect(calcer, &trigramCalcer::addError, this, &trigramWorker::passError);
}

QVector<QPair<QString, QSet<trigram>>> *trigramWorker::doWork() {
    auto *trigrams = new QVector<QPair<QString, QSet<trigram>>>();

    for (auto &calcer : calcers) {

        if (QThread::currentThread()->isInterruptionRequested()) {
            break;
        }

        QPair<QString, QSet<trigram>> newPortion;
        try {
            newPortion = calcer->calcTrigrams();
        } catch (std::bad_alloc &) {
            emit cant_calc_trigrams();

            delete trigrams;
            return new QVector<QPair<QString, QSet<trigram>>>();
        }

        if (prevUpdate + updateStep == curCalcer || (prevUpdate == 0 && curCalcer == 0)) {
            emit updateProgress(curCalcer / updateStep, progressBarId);
            prevUpdate = curCalcer;
        }
        calcers[curCalcer].reset();
        curCalcer++;

        if (!newPortion.second.empty()) {
            trigrams->push_back(newPortion);
        }
    }

    return trigrams;
}

void trigramWorker::calcTrigrams() {
    emit showHideProgressBar(true, progressBarId);
    int calcersAmount = 0;

    for (QDirIterator it(dir.absolutePath(), QDir::AllEntries | QDir::NoDotAndDotDot,
                         QDirIterator::Subdirectories); it.hasNext();) {
        it.next();
        if (it.fileInfo().isFile()) {
            trigramCalcer *newCalcer = new trigramCalcer(it.filePath());
            addCalcer(newCalcer);
            calcersAmount++;
        }

        if (QThread::currentThread()->isInterruptionRequested()) {
            break;
        }
    }

    emit resetProgressBar(std::min(100, calcersAmount), progressBarId);

    updateStep = std::max(calcersAmount / 100, 1);
    curCalcer = 0;
    prevUpdate = 0;

    QVector<QPair<QString, QSet<trigram>>> *trigrams = doWork();

    emit workDone(dir.absolutePath(), trigrams);
    emit showHideProgressBar(false, progressBarId);
    QThread::currentThread()->quit();
}

void trigramWorker::catch_cant_calc_trigrams() {
    emit cant_calc_trigrams();
}

void trigramWorker::passError(QString text) {
    emit addError(text);
}
