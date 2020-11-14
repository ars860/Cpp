#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "settings.h"
#include <memory>
#include <QSet>

namespace Ui {
    class settingsWindow;
}

class settingsWindow : public QDialog {
Q_OBJECT

public:
    explicit settingsWindow(QWidget *parent = nullptr);

    ~settingsWindow();
public slots:

    void apply_changes(int result);

private:
    std::unique_ptr<Ui::settingsWindow> ui;
};

#endif // SETTINGSWINDOW_H
