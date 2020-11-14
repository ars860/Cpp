#ifndef FILECOMPARER_UTILS_H
#define FILECOMPARER_UTILS_H

#include <QCryptographicHash>
#include <QFile>

class file_hash {
public:
    file_hash() = default;

    ~file_hash();

    explicit file_hash(QFile *file);

    std::string get_hash();

    bool compare(file_hash & other);

    bool compare_by_hashes(file_hash & other);

    bool compare_by_bytes(file_hash & other);

    void calc_file_hash();

    bool is_hash_calculated();

    QFile* get_file();
private:
    QFile *file;
    std::string hash;
    bool hash_calculated;
    static const int bufferSize = 1000;
};

#endif // FILECOMPARER_UTILS_H
