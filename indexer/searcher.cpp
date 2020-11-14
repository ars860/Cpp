#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QVector>
#include <QtCore/QDirIterator>
#include "searcher.h"
#include "settings.h"
#include "infilesearcher.h"
#include <QDebug>

searcher::searcher() {
    connect(&cnter, &counter::counted, this, &searcher::catchAllFounded);
    connect(&cnter, &counter::counted, this, &searcher::hideProgressBar);
}

void searcher::setInput(QString input) {
    this->input = input;
}

QVector<QString> searcher::findInTrigrams() {
    QSet<trigram> extracted = extractTrigrams();

    for (auto &map : *trigrams) {
        if (QThread::currentThread()->isInterruptionRequested()) {
            break;
        }

        for (auto &nameTrigrams : *map.second) {
            if (QThread::currentThread()->isInterruptionRequested()) {
                break;
            }

            bool is_ok = 1;

            auto &fileTrigrams = nameTrigrams.second;

            for (auto &trigram : extracted) {
                if (fileTrigrams.find(trigram) == fileTrigrams.end()) {
                    is_ok = 0;
                    break;
                }
            }

            if (is_ok) {
                suitableFiles.push_back(nameTrigrams.first);
            }
        }
    }

    return suitableFiles;
}

QSet<trigram> searcher::extractTrigrams() {
    QSet<trigram> res;

    if (input.size() > 2) {
        trigram currTrigram(input[0], input[1]);

        for (int i = 2; i < input.size(); i++) {
            currTrigram.next(input[i]);

            res.insert(currTrigram);
        }
    }

    return res;
}

void searcher::catchFounded(int x, int y, QString fileName) {
    emit show_founded(x, y, fileName);

    if (settings::getFindNext()) {
        emit hideShowUI(false);
    }
}

void searcher::doWork() {
    if (!trigrams->empty()) {
        findInTrigrams();
    } else {
        extractAllFiles();
    }

    cnter.setMax(suitableFiles.size());

    emit showHideProgressBar(true);
    emit setProgressBaxMax(suitableFiles.size());

    if (suitableFiles.empty()) {
        catchAllFounded();
        hideProgressBar();
        return;
    }

    QString file = suitableFiles[0];

    if (QThread::currentThread()->isInterruptionRequested()) {
        catchAllFounded();
        hideProgressBar();
        return;
    }

    inFileSearcher *worker = new inFileSearcher(file, input);

    connect(worker, &inFileSearcher::founded, this, &searcher::catchFounded);
    connect(worker, &inFileSearcher::allFounded, worker, &inFileSearcher::deleteLater);

    connect(this, &searcher::find, worker, &inFileSearcher::findNext);
    connect(worker, &inFileSearcher::allFounded, &cnter, &counter::add);
    connect(worker, &inFileSearcher::allFounded, this, &searcher::doWorkWithNextPortion);
}

void searcher::findNext() {
    emit find();
}

void searcher::setTrigrams(std::map<QString, std::unique_ptr<QVector<QPair<QString, QSet<trigram>>>>> *trigrams) {
    this->trigrams = trigrams;
}

void searcher::catchAllFounded() {
    emit allFounded();
    QThread::currentThread()->quit();
}

void searcher::set_dirs(QSet<QString> *dirs) {
    this->dirs = dirs;
}

QVector<QString> searcher::extractAllFiles() {
    for (auto &dirName : *dirs) {
        QDir dir(dirName);

        for (QDirIterator it(dir.absolutePath(), QDir::AllEntries | QDir::NoDotAndDotDot,
                             QDirIterator::Subdirectories); it.hasNext();) {
            it.next();
            if (it.fileInfo().isFile()) {
                suitableFiles.push_back(it.filePath());
            }

            if (QThread::currentThread()->isInterruptionRequested()) {
                break;
            }
        }
    }

    return suitableFiles;
}

void searcher::setProgressBar(QProgressBar *progressBar) {
    this->progressBar = progressBar;
}

void searcher::hideProgressBar() {
    emit showHideProgressBar(false);
}

void searcher::doWorkWithNextPortion(QString) {
    curr++;

    if (curr >= suitableFiles.size()) {
        return;
    }

    if (QThread::currentThread()->isInterruptionRequested()) {
        catchAllFounded();
        hideProgressBar();
        return;
    }


    QString file = suitableFiles[curr];

    inFileSearcher *worker = new inFileSearcher(file, input);

    connect(worker, &inFileSearcher::founded, this, &searcher::catchFounded);
    connect(worker, &inFileSearcher::allFounded, worker, &inFileSearcher::deleteLater);

    connect(this, &searcher::find, worker, &inFileSearcher::findNext);
    connect(worker, &inFileSearcher::allFounded, &cnter, &counter::add);
    connect(worker, &inFileSearcher::allFounded, this, &searcher::doWorkWithNextPortion);

    if (!settings::getFindNext()) {
        findNext();
    }
}
