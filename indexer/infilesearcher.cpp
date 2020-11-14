#include <QtCore/QThread>
#include "infilesearcher.h"
#include "settings.h"
#include <functional>

inFileSearcher::inFileSearcher() : x(0), y(-1), pos(0) {
}

inFileSearcher::inFileSearcher(QString fileName, QString input) : x(0), y(-1), pos(0), input(input) {
    new(&file) QFile(fileName);
    file.open(QIODevice::ReadOnly);
    new(&stream) QTextStream(&file);
}

QString myReadLine(QTextStream &stream) {
    QString ans;
    QChar ch = stream.read(1)[0];
    while (ch != '\n' && ch != '\r') {
        ans.append(ch);

        if (stream.atEnd()) {
            break;
        }

        ch = stream.read(1)[0];
    }

    return ans;
}

void inFileSearcher::findNext() {
    while (!stream.atEnd()) {
        if (QThread::currentThread()->isInterruptionRequested()) {
            break;
        }

        if (pos == 0) {
            y++;
//            buffer = stream.readLine();
            buffer = myReadLine(stream);
        }

        auto it = buffer.begin() - input.size() + pos;

        while (it != buffer.end()) {
            if (it + input.size() > buffer.end()) {
                break;
            }
            it = std::search(it + input.size(), buffer.end(), input.begin(), input.end());

            if (it != buffer.end()) {
                emit founded(it - buffer.begin() + 1, y + 1, file.fileName());

                if (settings::getFindNext()) {
                    pos = it - buffer.begin() + input.size();
                    return;
                }
            }
        }

        pos = 0;
    }
    emit allFounded(file.fileName());
}

