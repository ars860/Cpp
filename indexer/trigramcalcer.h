#ifndef TRIGRAMCALCER_H
#define TRIGRAMCALCER_H

#include <QObject>
#include <QtCore/QFile>
#include <QtCore/QVector>
#include <QtCore/QSet>
#include "trigram.h"
#include "settings.h"
#include <QDebug>

class trigramCalcer : public QObject {
Q_OBJECT
public:
    trigramCalcer();

    trigramCalcer(QString fileName);

    void setFile(QString fileName);
public slots:

    QPair<QString, QSet<trigram>> calcTrigrams();

signals:

    void addError(QString text);

    void cant_calc_trigrams();
private:
    const int buffer_size = settings::getBufferSize();
    const int binary_file_trigrams_amount = settings::getBinary_file_trigrams_amount();

    QFile file;
    QSet<trigram> trigrams;
};

#endif // TRIGRAMCALCER_H