#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QSet>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <memory>
#include <vector>
#include "settings.h"
#include "trigram.h"
#include "counter.h"
#include <QDebug>
#include <QtCore/QFileSystemWatcher>
#include <QThread>
#include <QtCore/QQueue>
#include <QtCore/QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void addDirectory();

public slots:
    void addError(QString text);

    void toggleErrorsVisisbility();

    void showSettings();

    void evaluateTrigrams();

    int execAlertWindow(QString text, QMessageBox::StandardButtons buttons);

    void removeItem(QListWidgetItem *item);

    void resetProgressBar(int value, int id);

    void updateProgessBar(int value, int id);

    void showHideProgressBar(bool value, int id);

    void addNewTrigramsPortion(QString dirName, QVector<QPair<QString, QSet<trigram>>> *trigrams);

    void hideShowUI(bool hide);

    void showHideCancelButton(bool show);

    void cancel();

    void recoverAfterCancel();

    void repair();

    void showFounded(int x, int y, QString fileName);

    void search();

    void clearTrigrams();

    void clearAllTrigrams();

    void catch_cant_calc_trigrams();

    void addDirToWatcher(QString dirName);

    void removeDirFromWatcher(QString dirName);

    void addDirectoryChangedMark(QString dirName);
signals:
    void addToWatcher(QStringList paths);

    void removeFromWatcher(QStringList paths);

    void interrupt();

    void findNext();
private:
    std::unique_ptr<Ui::MainWindow> ui;

    QSet<QString> dirs;
    std::map<QString, std::unique_ptr<QVector<QPair<QString, QSet<trigram>>>>> trigrams;
    std::map<QString, bool> isTrigramsCalced;
    std::vector<std::unique_ptr<QProgressBar>> progressBars;
    counter cnter;
    QFileSystemWatcher watcher;
    QThread watcherThread;

    bool is_cancelled = 0;
    bool trigramsCalced = 0;
    bool findNext_in_progress = 0;
    bool cant_calc_trigrams = 0;
    bool inProgress = 0;
};

#endif // MAINWINDOW_H
