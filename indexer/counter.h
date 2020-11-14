#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class counter : public QObject {
Q_OBJECT
public:
    counter();

    void turnEmitter(bool onOff);

    explicit counter(int max);
public slots:

    void setMax(int max);

    void add();

signals:

    void counted();

    void value_changed(int);
private:
    int max;
    int curr;
    bool emitter = 0;
};

#endif // COUNTER_H