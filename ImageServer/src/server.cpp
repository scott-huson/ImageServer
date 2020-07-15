#include "server.h"

Server::Server(QObject *parent): QTcpServer(parent)
{

    //    QString text = QInputDialog::getText(parent,"Path to XIO files","text");
    //    const int port = QInputDialog::getText(parent,"Port Number","int");

    // Basic constants initialization for base case
    const QString file_path = "/Users/Scott/Desktop/JPL/ImageServer/examples/ROICWARM/"; // File Path
    const char *file_path_string = file_path.toLocal8Bit().data();
    const int width = 640;
    const int height = 480;
    connectionIndex = 0;

    // Create a camera
    Camera = new XIOCamera(width, height, height);
    Camera->setDir(file_path_string);
    bool cam_started = Camera->start();

    if (!cam_started) {
        // In general, software camera models will always start, but some hardware camera models can fail
        // to start if the hardware is misconfigured.'

        qDebug() << "Failure to start camera.";

        frWidth = 0;
        frHeight = 0;
        isRunning = false;    // want to make sure that we don't enter the event loop
        return;
    } else {
        frWidth = Camera->getFrameWidth();
        frHeight = Camera->getFrameHeight();
        dataHeight = Camera->getDataHeight();
        cam_type = Camera->getCameraType();

        if (frWidth == 0 || frHeight == 0) {
            // In general, software camera models will always start, but some hardware camera models can fail
            // to start if the hardware is misconfigured.
            qDebug() << "Failure to start camera. Incorrect sizing (0, 0).";

            frWidth = 0;
            frHeight = 0;
            isRunning = false;    // want to make sure that we don't enter the event loop
            return;
        } else {
            connect(Camera, &CameraModel::timeout, this, &Server::reportTimeout);
            isRunning = true;    // now set up to enter the event loop
        }
    }
}

bool Server::StartServer(int port)
{
    char *des = (char *)"custom";
    if (!port) {
        port = default_port;
        des = (char *)"default";
    }

    if(listen(QHostAddress::Any, port)) {
        qDebug() << "Started image server on" << des << "port" << port;
        return true;
    } else {
        qDebug() << "Failed to start server. port: " << port;
        return false;
    }
}

void Server::incomingConnection(qintptr handle)
{
    connectionIndex++;
    Connection *connection = new Connection(this, Camera, connectionIndex);
    connection->SetSocket(handle);
}

void Server::reportTimeout()
{
    qDebug() << "Received Timeout";
    isRunning = false;
}
