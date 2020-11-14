#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QtCore/QThread>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTextEdit>
#include "filecollector.h"
#include "filedistributor.h"
#include "filecomparer.h"
#include "counter.h"
#include "bigcomparer.h"
#include "filedeleter.h"
#include "fileworker.h"

namespace Ui {
    class MainWindow;
}

class main_window : public QMainWindow {
Q_OBJECT

public:
    explicit main_window(QWidget *parent = 0);

    ~main_window();

private slots:

    void select_directory();

    void scan_directory(QString const &dir);

    void show_about_dialog();

    void addGroupToRes(int id);

    void evaluateAndMergeAllGroups();

    void workDone();

    void paintRow(QTreeWidgetItem *item);

    void hideRow(QTreeWidgetItem *item);

    void deleteFiles();

    void filesDeletedUpdateStatus();

    void setMessage(QString text);

    void setProgressBarHidden(bool value);

    void showHideErrors();
signals:

    void start();

    void distributorFilesReady();

    void startComparing();

    void compare(int);

    void startDeleting();

private:
    int threadsAmount = QThread::idealThreadCount();
    counter stats_counter;
    filecollector collector;
    filedistributor distributor;
    filedeleter deleter;
    fileworker file_worker;

    QThread deleterThread;
    QThread fileWorkerThread;
    QThread statsCounterThread;
    QVector<QThread*> threads = QVector<QThread*>(threadsAmount);

    QVector<QPointer<filecomparer>> comparers;
    QVector<bigComparer*> bigcomparers = QVector<bigComparer*>(threadsAmount);

    void repair();

    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_H