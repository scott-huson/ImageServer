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
    explicit Connection(QObject *parent = nullptr, CameraModel *ReadingCamera = nullptr, int ConnectionNumber = 0);
    void SetSocket(int Descriptor);
//    virtual uint16_t* getFrame();

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void TaskResult(uint16_t *data);

private:
    QTcpSocket *socket; // TCP Socket of connection
    bool handshake; // Used to determine if client has accepted transmitted data sizes
    CameraModel *Camera; // Camera class for data aquisition
    int connectionFrameCounter; // Used to determine number of frames being sent to each connection
    int connectionNumber; // Used for identifying connections from each other
};
#endif // CONNECTION_H
