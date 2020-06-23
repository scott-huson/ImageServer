#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>

#include "connection.h"
#include "xiocamera.h"
#include "image_type.h"
#include "cameramodel.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void StartServer();
    CameraModel *Camera;
    int frWidth, frHeight, dataHeight;
    volatile bool isRunning;
    camera_t cam_type;
    int port;
protected:
    void incomingConnection(qintptr handle) override;

signals:

public slots:
    void reportTimeout();

private:
    int connectionIndex;
};

#endif // SERVER_H
