#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>

#include "connection.h"
#include "xiocamera.h"
#include "envicamera.h"
#include "image_type.h"
#include "cameramodel.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    bool StartServer(int port, QString directory, bool envi);
    CameraModel *Camera;
    int frWidth, frHeight, dataHeight;
    volatile bool isRunning;
    camera_t cam_type;
    int default_port = 1337;
    const QString default_directory = "/Users/Scott/Desktop/JPL/ImageServer/examples/ROICWARM/";

protected:
    void incomingConnection(qintptr handle) override;

public slots:
    void reportTimeout();

private:
    int connectionIndex;
};

#endif // SERVER_H
