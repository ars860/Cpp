#ifndef TRIGRAM_H
#define TRIGRAM_H


#include <cstdint>
#include <QtCore/QChar>
#include <QtCore/QHash>

class trigram
{
public:
    trigram();

    trigram(QChar ch1, QChar ch2);

    trigram(QChar ch1, QChar ch2, QChar ch3);

    uint64_t get() const;

    void next(QChar ch);

    bool operator==(trigram const& other) const{
        return get() == other.get();
    }
private:
    uint64_t data;
};

inline uint qHash(const trigram &tr) {
    return qHash(tr.get());
}

#endif // TRIGRAM_H
