#ifndef SETTINGS_H
#define SETTINGS_H

#include <set>
#include <string>
#include <QtCore/QString>

class settings {
public:
    settings();

    static void setFormats(QString formats);

    static void setFindNext(bool findNext);

    static bool getFindNext();

    static int getBinary_file_trigrams_amount();

    static QString formats;

    static QString getFormats();

    static bool is_supported(QString format);

    static int getBufferSize();
private:
    static std::set<QString> supported_formats;
    static bool findNext;
    static int binary_file_trigrams_amount;
    static int buffer_size;
};

#endif // SETTINGS_H
