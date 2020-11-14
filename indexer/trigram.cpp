#include "trigram.h"

trigram::trigram() {

}

trigram::trigram(QChar ch1, QChar ch2) {
    data = (static_cast<uint64_t>(ch1.unicode()) << 16) + (static_cast<uint64_t>(ch2.unicode()) << 32);
}

trigram::trigram(QChar ch1, QChar ch2, QChar ch3) {
    data = ch1.unicode() + (static_cast<uint64_t>(ch2.unicode()) << 16) + ((static_cast<uint64_t>(ch3.unicode()) << 32));
}

void trigram::next(QChar ch) {
    data = (data >> 16) + ((static_cast<uint64_t>(ch.unicode()) << 32));
}

uint64_t trigram::get() const {
    return data;
}
