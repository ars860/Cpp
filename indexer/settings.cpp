#include <QtCore/QStringList>
#include "settings.h"

int settings::buffer_size = 10000;

int settings::binary_file_trigrams_amount = 20000;

std::set<QString> settings::supported_formats;

QString settings::formats;

bool settings::findNext = 0;

settings::settings() {

}

void settings::setFormats(QString formats) {
    settings::formats = formats;
    supported_formats.clear();

    auto splittedFormats = formats.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    for (auto &str : splittedFormats) {
        supported_formats.insert(str);
    }
}

QString settings::getFormats() {
    return formats;
}

bool settings::is_supported(QString format) {
    if (supported_formats.empty()) {
        return true;
    }

    return supported_formats.find(format) != supported_formats.end();
}

int settings::getBufferSize() {
    return settings::buffer_size;
}

void settings::setFindNext(bool findNext) {
    settings::findNext = findNext;
}

bool settings::getFindNext() {
    return settings::findNext;
}

int settings::getBinary_file_trigrams_amount() {
    return settings::binary_file_trigrams_amount;
}
