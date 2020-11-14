#ifndef COUNTER_H
#define COUNTER_H


#include <QtCore/QObject>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>

class counter : public QObject {
Q_OBJECT
public:
    counter();

    void reset();

public slots:

    void add();

    void setMaxValueForComparing(int maxValue);

signals:

    void updateProgressBar(int);

    void workDone();

    void countingDone();

    void updateMaximum(int);

    void setMessage(QString);

    void setProgressBarHidden(bool);

private:
    int maxValue;
    int currentValue;
    int lastUpdated;
    int updateStep;
    int static const INF = 10000;

};

#endif // COUNTER_H