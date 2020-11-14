#include "file_hash.h"
#include <QDataStream>

file_hash::file_hash(QFile *file) : file(file), hash_calculated(false) {

}

file_hash::~file_hash() = default;

std::string file_hash::get_hash() {
    return hash;
}

bool file_hash::is_hash_calculated() {
    return hash_calculated;
}

bool compare_buffers(char *first_buffer, char *second_buffer, int length) {
    for (int i = 0; i < length; i++) {
        if (first_buffer[i] != second_buffer[i]) {
            return false;
        }
    }
    return true;
}

void file_hash::calc_file_hash() {
    QCryptographicHash qCryptographicHash(QCryptographicHash::Sha1);
    if (!file->isOpen()) {
        file->open(QIODevice::ReadOnly);
    }

    QDataStream in(file);
    int buffer_size = 1000;
    char buffer[1000];

    buffer_size = in.readRawData(buffer, buffer_size);
    while (buffer_size > 0) {
        qCryptographicHash.addData(buffer, buffer_size);
        buffer_size = in.readRawData(buffer, buffer_size);
    }

    hash_calculated = true;
    file->close();
    hash = qCryptographicHash.result().toStdString();
}

bool file_hash::compare_by_hashes(file_hash &other) {
    if (!is_hash_calculated()) {
        calc_file_hash();
    }
    if (!other.is_hash_calculated()) {
        other.calc_file_hash();
    }

    std::string first_file_hash = get_hash(), second_file_hash = other.get_hash();

    return first_file_hash == second_file_hash;
}

QFile *file_hash::get_file() {
    return file;
}

bool file_hash::compare_by_bytes(file_hash &other) {
    QFile first_file(get_file()->fileName());
    QFile second_file(other.get_file()->fileName());

    first_file.open(QIODevice::ReadOnly);
    second_file.open(QIODevice::ReadOnly);

    QDataStream first_in(&first_file);
    QDataStream second_in(&second_file);

    int first_buffer_size = bufferSize;
    int second_buffer_size = bufferSize;
    char first_buffer[bufferSize];
    char second_buffer[bufferSize];

    first_buffer_size = first_in.readRawData(first_buffer, first_buffer_size);
    second_buffer_size = second_in.readRawData(second_buffer, second_buffer_size);

    while (first_buffer_size == second_buffer_size && first_buffer_size > 0) {
        if (std::equal(first_buffer, second_buffer, first_buffer + first_buffer_size) != 0) {
            return false;
        }

        first_buffer_size = first_in.readRawData(first_buffer, first_buffer_size);
        second_buffer_size = second_in.readRawData(second_buffer, second_buffer_size);
    }

    return first_buffer_size == second_buffer_size;
}

bool file_hash::compare(file_hash &other) {
    return compare_by_bytes(other);
}
