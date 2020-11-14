#include "settingswindow.h"
#include "ui_settingswindow.h"

settingsWindow::settingsWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::settingsWindow) {
    ui->setupUi(this);

    ui->findNext->setChecked(settings::getFindNext());

    ui->formatsEdit->setText(settings::getFormats());
    ui->formatsEdit->setFocus();
    this->setFixedSize(400, 290);

    connect(this, &QDialog::finished, this, &settingsWindow::apply_changes);
}

settingsWindow::~settingsWindow() {}

void settingsWindow::apply_changes(int result) {
    if(result){
        settings::setFormats(ui->formatsEdit->text());
        settings::setFindNext(ui->findNext->isChecked());
    }
}
