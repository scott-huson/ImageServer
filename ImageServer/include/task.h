#ifndef TASK_H
#define TASK_H


#include <QDebug>
#include <QObject>
#include <QRunnable>

#include "cameramodel.h"

class Task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Task(CameraModel *PassedCamera);

signals:
    void Result(uint16_t *data);

protected:
    void run();

private:
    CameraModel *Camera;
};

#endif // TASK_H
