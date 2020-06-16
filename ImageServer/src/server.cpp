#include "server.h"

Server::Server(QObject *parent): QTcpServer(parent)
{

    //    QString text = QInputDialog::getText(parent,"Path to XIO files","text");
    //    const int port = QInputDialog::getText(parent,"Port Number","int");

    // Basic constants initialization for base case
    const QString file_path = "/Users/Scott/Desktop/JPL/ImageServer/examples/ROICWARM/"; // File Path
    const char *file_path_string = file_path.toLocal8Bit().data();
    port = 1337;
    const int width = 640;
    const int height = 480;

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

void Server::StartServer()
{
    if(listen(QHostAddress::Any, port)) {
        qDebug() << "Started image server on port " << port;
    } else {
        qDebug() << "Failed to start server. port: " << port;
    }
}

void Server::incomingConnection(qintptr handle)
{
    Connection *connection = new Connection(this, Camera);
    connection->SetSocket(handle);
}

void Server::reportTimeout()
{
    qDebug() << "Received Timeout";
    isRunning = false;
}
