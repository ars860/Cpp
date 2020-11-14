#include "counter.h"

counter::counter() = default;

void counter::setMaxValueForComparing(int value) {
    this->maxValue = value;
    updateStep = std::max(1, maxValue / 100);
    emit updateMaximum(maxValue / updateStep + (maxValue % updateStep > 0 ? 1 : 0));
    emit setProgressBarHidden(false);
    emit setMessage("comparing files");
    if (maxValue == 0) {
        emit workDone();
    }
}

void counter::reset() {
    currentValue = 0;
    lastUpdated = 0;
    maxValue = INF;
}

void counter::add() {
    currentValue++;
    if (currentValue - updateStep == lastUpdated) {
        emit updateProgressBar(currentValue / updateStep);
        lastUpdated = currentValue;
    }
    if (currentValue == maxValue) {
        emit setMessage("Work done");
        emit workDone();
    }
}