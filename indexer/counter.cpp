#include "counter.h"
#include <QDebug>

void counter::setMax(int max) {
    this->max = max;
    curr = 0;
    emitter = 0;
    if (max == 0) {
        emit counted();
    }
}

void counter::add() {
    curr++;
    if (emitter) {
        emit value_changed(curr);
    }
    if (curr == max) {
        emit counted();
    }
}

counter::counter() : curr(0) {

}

counter::counter(int max) : curr(0), max(max) {
}

void counter::turnEmitter(bool onOff) {
    emitter = onOff;
}
