#include <QtWidgets/QFileDialog>
#include <QtWidgets/QAction>
#include <QtWidgets/QListWidgetItem>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDirIterator>
#include <QtCore/QThread>
#include <QtWidgets/QGroupBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trigramworker.h"
#include "searcher.h"
#include "settingswindow.h"
#include <memory>
#include <vector>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->progressBar->reset();
    ui->progressBar->setHidden(true);
    ui->errors->setHidden(true);

    connect(ui->actionAdd_directory, &QAction::triggered, this, &MainWindow::addDirectory);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::removeItem);
    connect(ui->findButton, &QPushButton::pressed, this, &MainWindow::evaluateTrigrams);
    connect(ui->actionshowSettings, &QAction::triggered, this, &MainWindow::showSettings);
    connect(ui->cancelButton, &QPushButton::pressed, this, &MainWindow::cancel);

    connect(&cnter, &counter::counted, this, &MainWindow::recoverAfterCancel);
    connect(&cnter, &counter::counted, this, &MainWindow::search);

    connect(ui->showHideErrors, &QPushButton::pressed, this, &MainWindow::toggleErrorsVisisbility);

    ui->cancelButton->setVisible(false);

    watcher.moveToThread(&watcherThread);
    watcherThread.start();
    connect(this, &MainWindow::addToWatcher, &watcher, &QFileSystemWatcher::addPaths);
    connect(this, &MainWindow::removeFromWatcher, &watcher, &QFileSystemWatcher::removePaths);
//    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::addDirectoryChangedMark);
    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::addDirectoryChangedMark);
}

int MainWindow::execAlertWindow(QString text, QMessageBox::StandardButtons buttons) {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setStandardButtons(buttons);
    return msgBox.exec();
}

void MainWindow::addDirectory() {
    QString newDir = QFileDialog::getExistingDirectory(this, "Select Directory to add",
                                                       QString(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (newDir == "") {
        return;
    }

    bool alreadyIncluded = 0, removeItems = 0;
    QVector<QString> toErase;
    for (QString const &dir : dirs) {
        if (dir == newDir) {
            alreadyIncluded = 1;
        } else if (dir.left(newDir.size()) == newDir && dir[newDir.size()] == '/') {
            switch (execAlertWindow(QString("Some childs (%1) of your directory already included.\n "
                                            "Do you want to discard them/include only parent?").arg(dir),
                                    QMessageBox::Yes | QMessageBox::No)) {
                case QMessageBox::Yes:
                    removeItems = 1;
                    break;
                case QMessageBox::No:
                    alreadyIncluded = 1;
                    break;
            }

            toErase.push_back(dir);
        } else if (newDir.left(dir.size()) == dir && newDir[dir.size()] == '/') {
            switch (execAlertWindow(QString("Parent (%1) of your directory already included.\n "
                                            "Do you want to discard him/include only child?").arg(dir),
                                    QMessageBox::Yes | QMessageBox::No)) {
                case QMessageBox::Yes:
                    toErase.push_back(dir);
                    removeItems = 1;
                    break;
                case QMessageBox::No:
                    alreadyIncluded = 1;
                    break;
            }
        }
    }

    if (!alreadyIncluded) {
        trigramsCalced = 0;
        if (removeItems) {
            for (QString const &item : toErase) {
                removeItem(ui->listWidget->findItems(item, Qt::MatchFixedString).front());
            }
        }

        dirs.insert(newDir);

        addDirToWatcher(newDir);

        QListWidgetItem *itm = new QListWidgetItem();
        itm->setText(newDir);
        itm->setHidden(false);
        itm->setIcon(QIcon());

        ui->listWidget->addItem(itm);
    }
}

void MainWindow::removeItem(QListWidgetItem *item) {
    if (!inProgress) {
        auto text = item->text();
        if (text.endsWith(" !!!dir changed!!!")) {
            text = text.remove(text.size() - QString(" !!!dir changed!!!").size(),
                               QString(" !!!dir changed!!!").size());
        }

        trigrams.erase(text);
        isTrigramsCalced.erase(text);

        removeDirFromWatcher(text);

        dirs.erase(dirs.find(text));
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
}

void MainWindow::evaluateTrigrams() {
    ui->statusLabel->setText("Calcing trigrams");
    QString input = ui->lineEdit->text();

    if (input == "") {
        execAlertWindow("Input is empty", QMessageBox::Ok);
        return;
    }
    if (dirs.empty()) {
        execAlertWindow("Directories not selected", QMessageBox::Ok);
        return;
    }
    if (input.size() <= 2) {
        if (settings::getFormats().isEmpty()) {
            execAlertWindow("Specify formats for searching strings with length <3\n", QMessageBox::Ok);
            return;
        }
        cant_calc_trigrams = 1;
    }


    inProgress = 1;

    clearTrigrams();
    is_cancelled = 0;
    if (trigramsCalced || cant_calc_trigrams) {
        search();
        return;
    }

    hideShowUI(true);
    showHideCancelButton(true);

    cnter.setMax(dirs.size() - trigrams.size());
    for (auto &dir : dirs) {
        if (trigrams.find(dir) == trigrams.end()) {
            QThread *thread = new QThread();

            progressBars.push_back(std::unique_ptr<QProgressBar>(new QProgressBar()));

            progressBars.back()->reset();
            progressBars.back()->setVisible(false);

            //<kostyl area>

            QWidget *newWidget = new QWidget;
            QLayout *l = new QHBoxLayout;
            QLabel *label = new QLabel();

            QFont font("Times", 12);
            label->setFont(font);
            label->setAlignment(Qt::AlignCenter);
            label->setFixedWidth(QFontMetrics(font).width(dir));

            l->addWidget(label);
            l->addWidget(&*progressBars.back());
            newWidget->setLayout(l);

            //</kostyl area>

            auto item = ui->listWidget->findItems(dir, Qt::MatchExactly).back();
            item->setSizeHint(newWidget->sizeHint());
            ui->listWidget->setItemWidget(item, newWidget);

            trigramWorker *worker = new trigramWorker(dir, progressBars.size() - 1);
            worker->moveToThread(thread);

            connect(thread, &QThread::started, worker, &trigramWorker::calcTrigrams);
            connect(thread, &QThread::finished, worker, &trigramWorker::deleteLater);
            connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            connect(this, &MainWindow::interrupt, thread, &QThread::requestInterruption);

            connect(worker, &trigramWorker::updateProgress, this, &MainWindow::updateProgessBar);
            connect(worker, &trigramWorker::showHideProgressBar, this, &MainWindow::showHideProgressBar);
            connect(worker, &trigramWorker::workDone, this, &MainWindow::addNewTrigramsPortion);
            connect(worker, &trigramWorker::cant_calc_trigrams, this, &MainWindow::catch_cant_calc_trigrams);

            connect(worker, &trigramWorker::addError, this, &MainWindow::addError);

            thread->start();
        }
    }
}

MainWindow::~MainWindow() {
    watcherThread.quit();
}

void MainWindow::resetProgressBar(int value, int id) {
    progressBars[id]->reset();
    progressBars[id]->setMaximum(value);
}

void MainWindow::showHideProgressBar(bool show, int id) {
    if (!is_cancelled) {
        progressBars[id]->setVisible(show);
    }
}

void MainWindow::updateProgessBar(int value, int id) {
    progressBars[id]->setValue(value);
}

void MainWindow::showSettings() {
    auto old_formats = settings::getFormats();
    settingsWindow window;
    window.exec();

    if (window.result() && settings::getFormats() != old_formats) {
        clearAllTrigrams();
    }
}

void MainWindow::addNewTrigramsPortion(QString dirName, QVector<QPair<QString, QSet<trigram>>> *trigrams) {
    if (!cant_calc_trigrams) {
        this->trigrams[dirName] = std::unique_ptr<QVector<QPair<QString, QSet<trigram>>>>(trigrams);
        isTrigramsCalced[dirName] = !is_cancelled;
    }

    cnter.add();
}

void MainWindow::hideShowUI(bool hide) {
    ui->actionshowSettings->setDisabled(hide);
    ui->actionAdd_directory->setDisabled(hide);
    ui->findButton->setDisabled(hide);
}

void MainWindow::showHideCancelButton(bool show) {
    ui->cancelButton->setVisible(show);
}

void MainWindow::cancel() {
    if (findNext_in_progress) {
        emit findNext();
    }

    is_cancelled = 1;
    emit interrupt();
}

void MainWindow::recoverAfterCancel() {
    if (is_cancelled) {
        repair();
    }
}

void MainWindow::repair() {
    ui->statusLabel->setText("Work Done!");
    ui->progressBar->setValue(0);

    showHideCancelButton(false);
    hideShowUI(false);
    progressBars.clear();
    findNext_in_progress = 0;
    cant_calc_trigrams = 0;
    trigramsCalced = 0;
    inProgress = 0;
}

void MainWindow::showFounded(int x, int y, QString fileName) {
    ui->output->append(QString("x: %1 y: %2 in file: %3").arg(x).arg(y).arg(fileName));
    cnter.add();
}

void MainWindow::search() {
    if (!is_cancelled) {
        ui->statusLabel->setText("Searching");

        if (findNext_in_progress) {
            ui->output->append("\n---------------\n");
            emit findNext();
            return;
        } else {
            ui->output->clear();
        }

        trigramsCalced = 1;
        QThread *thread = new QThread();

        searcher *worker = new searcher();

        cnter.setMax(1e6);
        cnter.turnEmitter(1);
        connect(&cnter, &counter::value_changed, ui->progressBar, &QProgressBar::setValue, Qt::UniqueConnection);

        if (cant_calc_trigrams) {
            worker->set_dirs(&dirs);
            clearAllTrigrams();
        }
        worker->setTrigrams(&trigrams);
        worker->setInput(ui->lineEdit->text());
        worker->setProgressBar(ui->progressBar);

        worker->moveToThread(thread);

        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(thread, &QThread::finished, worker, &searcher::deleteLater);
        connect(thread, &QThread::started, worker, &searcher::doWork);
        connect(this, &MainWindow::findNext, worker, &searcher::findNext);
        connect(worker, &searcher::show_founded, this, &MainWindow::showFounded);
        connect(worker, &searcher::hideShowUI, this, &MainWindow::hideShowUI);
        connect(worker, &searcher::allFounded, this, &MainWindow::repair);
        connect(this, &MainWindow::interrupt, thread, &QThread::requestInterruption);
        connect(worker, &searcher::showHideProgressBar, ui->progressBar, &QProgressBar::setVisible);
        connect(worker, &searcher::setProgressBaxMax, ui->progressBar, &QProgressBar::setMaximum);

        while (!thread->isRunning()) {
            thread->start();
        }

        emit findNext();
        if (settings::getFindNext()) {
            findNext_in_progress = 1;
        }
    } else {
        ui->statusLabel->setText("Canceled");
    }
}

void MainWindow::clearTrigrams() {
    std::vector<QString> toDelete;
    for (auto &dirTrigrams : trigrams) {
        if (dirs.find(dirTrigrams.first) == dirs.end()) {
            toDelete.push_back(dirTrigrams.first);
        } else {
            if (isTrigramsCalced.find(dirTrigrams.first) == isTrigramsCalced.end()) {
                toDelete.push_back(dirTrigrams.first);
            } else if (!isTrigramsCalced.at(dirTrigrams.first)) {
                toDelete.push_back(dirTrigrams.first);
            }
        }
    }

    for (auto &dirName : toDelete) {
        trigrams.erase(dirName);
    }
}

void MainWindow::clearAllTrigrams() {
    trigrams.clear();
    isTrigramsCalced.clear();
}

void MainWindow::catch_cant_calc_trigrams() {
    cant_calc_trigrams = 1;
    trigramsCalced = 1;
    emit interrupt();
}

void MainWindow::toggleErrorsVisisbility() {
    if (ui->errors->isVisible()) {
        ui->showHideErrors->setText("Show Errors");
        ui->errors->setVisible(false);
    } else {
        ui->showHideErrors->setText("Hide Errors");
        ui->errors->setVisible(true);
    }
}

void MainWindow::addError(QString text) {
    ui->errors->append(text);
}

void MainWindow::addDirToWatcher(QString dirName) {
    QStringList list;
    list.append(dirName);

    for (QDirIterator it(dirName, QDir::AllDirs | QDir::NoDotAndDotDot,
                         QDirIterator::Subdirectories); it.hasNext();) {
        it.next();

        list.append(it.filePath());
        if(list.size() > 100){
            break;
        }
    }

    if(list.size()<100){
        emit addToWatcher(list);
    }
}

void MainWindow::removeDirFromWatcher(QString dirName) {
    QStringList list;
    list.append(dirName);

    for (QDirIterator it(dirName, QDir::AllDirs | QDir::NoDotAndDotDot,
                         QDirIterator::Subdirectories); it.hasNext();) {
        it.next();

        list.append(it.filePath());
        if(list.size() > 100){
            break;
        }
    }

    if(list.size() < 100){
        emit removeFromWatcher(list);
    }
}

void MainWindow::addDirectoryChangedMark(QString dirName) {
    auto items = ui->listWidget->findItems(dirName, Qt::MatchFixedString);

    if (items.empty()) {
        return;
    }

    if (!isTrigramsCalced[dirName]) {
        return;
    }

    auto item = items.front();

    QString oldText = item->text();
    if (!oldText.endsWith(" !!!dir changed!!!")) {
        item->setText(oldText.append(" !!!dir changed!!!"));
    }
}
