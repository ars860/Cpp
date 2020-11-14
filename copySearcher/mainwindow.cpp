#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QPointer>

main_window::main_window(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->setColumnCount(1);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, &deleter, &filedeleter::addFile);

    QCommonStyle style;
    ui->actionScan_Directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->actionExit->setIcon(style.standardIcon(QCommonStyle::SP_DialogCloseButton));
    ui->actionAbout->setIcon(style.standardIcon(QCommonStyle::SP_DialogHelpButton));
    ui->actionDelete->setIcon(style.standardIcon(QCommonStyle::SP_TrashIcon));

    connect(ui->actionScan_Directory, &QAction::triggered, this, &main_window::select_directory);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &main_window::show_about_dialog);
    connect(ui->actionDelete, &QAction::triggered, this, &main_window::deleteFiles);
    connect(this, &main_window::startDeleting, &deleter, &filedeleter::deleteFiles);

    connect(this, &main_window::start, &file_worker, &fileworker::doWork);

    connect(&collector, &filecollector::addError, ui->errorsList, &QTextEdit::append);

    connect(&distributor, &filedistributor::sorted, this, &main_window::evaluateAndMergeAllGroups);
    connect(&distributor, &filedistributor::updateStatsCounter, &stats_counter, &counter::setMaxValueForComparing);

    connect(&stats_counter, &counter::workDone, this, &main_window::workDone);
    connect(&stats_counter, &counter::updateProgressBar, ui->progressBar, &QProgressBar::setValue);
    connect(&stats_counter, &counter::updateMaximum, ui->progressBar, &QProgressBar::setMaximum);
    connect(&stats_counter, &counter::setMessage, this, &main_window::setMessage);
    connect(&stats_counter, &counter::setProgressBarHidden, this, &main_window::setProgressBarHidden);

    connect(&deleter, &filedeleter::toggleChecked, this, &main_window::paintRow);
    connect(&deleter, &filedeleter::hideRow, this, &main_window::hideRow);
    connect(&deleter, &filedeleter::filesDeleted, this, &main_window::filesDeletedUpdateStatus);
    connect(&deleter, &filedeleter::addError, ui->errorsList, &QTextEdit::append);

    connect(ui->showHideErrors, &QPushButton::pressed, this, &main_window::showHideErrors);

    ui->progressBar->hide();
    ui->statusMessage->setText("Choose directory");
    ui->treeWidget->setExpandsOnDoubleClick(false);
    ui->errorsList->setReadOnly(true);
    ui->errorsList->setHidden(true);
    ui->errorsList->viewport()->setCursor(Qt::ArrowCursor);
    ui->groupBox->setStyleSheet("QGroupBox {border: none}");

    deleter.moveToThread(&deleterThread);
    stats_counter.moveToThread(&statsCounterThread);
    deleterThread.start();
    statsCounterThread.start();

    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
    qt_ntfs_permission_lookup++;
}

main_window::~main_window() {
    deleterThread.quit();
    fileWorkerThread.quit();
    statsCounterThread.quit();

    for (auto &thread : threads) {
        if (thread != nullptr) {
            thread->quit();
        }
    }

    deleterThread.wait();
    fileWorkerThread.wait();
    statsCounterThread.wait();

    for (auto &thread : threads) {
        if (thread != nullptr) {
            thread->wait();
        }
    }
};

void main_window::repair() {
    for (int i = 0; i < threadsAmount; i++) {
        delete threads[i];
        delete bigcomparers[i];
        threads[i] = new QThread();
        bigcomparers[i] = new bigComparer();
    }

    qDeleteAll(comparers);
    comparers.clear();

    collector.reset();
    distributor.reset();
    stats_counter.reset();
    deleter.reset();
    ui->progressBar->reset();
    ui->errorsList->clear();
}

void main_window::select_directory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir != "") {
        repair();
        scan_directory(dir);
    }
}

void main_window::scan_directory(QString const &dir) {
    ui->treeWidget->clear();
    setWindowTitle(QString("Directory Content - %1").arg(dir));

    collector.set_directory(dir);

    file_worker.setCollector(&collector);
    file_worker.setDistributor(&distributor);

    file_worker.moveToThread(&fileWorkerThread);

    ui->statusMessage->setText("collecting files...");
    ui->actionScan_Directory->setVisible(false);
    fileWorkerThread.start();


    emit start();
}

void main_window::evaluateAndMergeAllGroups() {
    QVector<QVector<QFile *>> groups = distributor.getGroups();
    comparers.resize(groups.size());
    for (int i = 0; i < comparers.size(); i++) {
        comparers[i] = new filecomparer();
    }

    for (int i = 0; i < groups.size(); i++) {
        comparers[i]->set_files(groups[i]);
        comparers[i]->setId(i);
        connect(comparers[i], &filecomparer::compared, this, &main_window::addGroupToRes);
        connect(comparers[i], &filecomparer::compared, &stats_counter, &counter::add);
        connect(comparers[i], &filecomparer::addError, ui->errorsList, &QTextEdit::append);
    }

    for (int i = 0; i < groups.size(); i++) {
        bigcomparers[i % threadsAmount]->addComparer(comparers[i]);
    }

    for (int i = 0; i < threadsAmount; i++) {
        bigcomparers[i]->moveToThread(threads[i]);
        connect(this, &main_window::startComparing, bigcomparers[i], &bigComparer::compareAll);
        threads[i]->start();
    }

    emit startComparing();
}

void setUncheckableForUser(QTreeWidgetItem *item) {
    auto currentFlags = item->flags();
    item->setFlags(currentFlags & (~Qt::ItemIsUserCheckable));
}

void main_window::addGroupToRes(int id) {
    QVector<QVector<QFile *>> groups = comparers[id]->getGroups();
    for (QVector<QFile *> group : groups) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);

        size_t cnt = 0;
        for (QFile *file : group) {
            QTreeWidgetItem *child = new QTreeWidgetItem(item);
            child->setText(0, file->fileName());
            child->setCheckState(0, Qt::Unchecked);
            setUncheckableForUser(child);
            cnt++;
        }
        item->setText(0, QString("%1 duplicates").arg(cnt));
        item->setCheckState(0, Qt::Unchecked);
        setUncheckableForUser(item);

        ui->treeWidget->addTopLevelItem(item);
    }
}

void main_window::workDone() {
    ui->actionScan_Directory->setVisible(true);
    ui->statusMessage->setText("work done");
    ui->progressBar->hide();
}

void main_window::paintRow(QTreeWidgetItem *item) {
    if (item->checkState(0) == Qt::Checked) {
        item->setCheckState(0, Qt::Unchecked);
    } else {
        item->setCheckState(0, Qt::Checked);
    }
}

void main_window::filesDeletedUpdateStatus() {
    ui->statusMessage->setText("files successfully deleted!");
}

void ::main_window::hideRow(QTreeWidgetItem *item) {
    item->setHidden(true);

    if (item->parent() != nullptr) {
        item->parent()->text(0);
        QString duplicatesAmountStr = item->parent()->text(0);
        duplicatesAmountStr.chop(QString(" duplicates").size());
        int dupAmountInt = std::stoi(duplicatesAmountStr.toStdString());
        dupAmountInt--;
        if (dupAmountInt != 0) {
            item->parent()->setText(0, QString("%1 duplicates").arg(dupAmountInt));
        } else {
            item->parent()->setHidden(true);
        }
    }
}

void main_window::deleteFiles() {
    ui->statusMessage->setText("deleting files...");
    emit startDeleting();
}

void main_window::show_about_dialog() {
    QMessageBox::aboutQt(this);
}

void main_window::setMessage(QString text) {
    ui->statusMessage->setText(text);
}

void main_window::setProgressBarHidden(bool value) {
    ui->progressBar->setHidden(value);
}

void main_window::showHideErrors() {
    if (ui->errorsList->isHidden()) {
        ui->showHideErrors->setText("Hide Errors");
        ui->errorsList->setHidden(false);
    } else {
        ui->showHideErrors->setText("Show Errors");
        ui->errorsList->setHidden(true);
    }
}
