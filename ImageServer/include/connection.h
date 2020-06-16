#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QJsonObject>
#include <QJsonDocument>

#include "task.h"
#include "cameramodel.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr, CameraModel *ReadingCamera = nullptr);
    void SetSocket(int Descriptor);
//    virtual uint16_t* getFrame();

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void TaskResult(uint16_t *data);

private:
    QTcpSocket *socket;
    bool handshake;
    CameraModel *Camera;
};
#endif // CONNECTION_H
