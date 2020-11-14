#include <QtCore/QTextStream>
#include <QtCore/QThread>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>
#include "trigramcalcer.h"
#include "settings.h"
#include <QDebug>

trigramCalcer::trigramCalcer() {
}

trigramCalcer::trigramCalcer(QString fileName) : file(fileName) {}

QPair<QString, QSet<trigram>> trigramCalcer::calcTrigrams() {
        if (!file.open(QIODevice::ReadOnly)) {
            emit addError(QString("file: %1 is not readable").arg(file.fileName()));
            return {file.fileName(), trigrams};
        }

        if (!settings::is_supported(QFileInfo(file).suffix())) {
            return {file.fileName(), trigrams};
        }

        int buff_size = buffer_size;
        char buffer[buffer_size];

        QDataStream stream(&file);
        buff_size = stream.readRawData(buffer, buffer_size);

        if (buff_size >= 3) {
            trigram curTrigram(buffer[0], buffer[1]);

            int pos = 2;
            while (buff_size > 0) {
                if (QThread::currentThread()->isInterruptionRequested()) {
                    break;
                }

                for (; pos < buff_size; pos++) {
                    if (QThread::currentThread()->isInterruptionRequested()) {
                        break;
                    }

                    curTrigram.next(buffer[pos]);
                    trigrams.insert(curTrigram);

                    if (trigrams.size() > binary_file_trigrams_amount) {
                        break;
                    }
                }

                if (trigrams.size() > binary_file_trigrams_amount) {
                    trigrams.clear();
                    break;
                }

                buff_size = stream.readRawData(buffer, buff_size);
                pos = 0;
            }
        }

    return {file.fileName(), trigrams};
}

void trigramCalcer::setFile(QString fileName) {
    new(&file) QFile(fileName);
}
