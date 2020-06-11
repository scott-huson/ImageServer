#ifndef TASK_H
#define TASK_H


#include <QDebug>
#include <QObject>
#include <QRunnable>

class Task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Task();

signals:
    void Result(int Number);

protected:
    void run();
};

#endif // TASK_H
