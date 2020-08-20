#include "server.h"

Server::Server(QObject *parent): QTcpServer(parent)
{
    connectionIndex = 0;
}

bool Server::StartServer(int port, QString directory, bool envi)
{
    // These could be taken as parameters or derived from the data.
    const int width = 640;
    const int height = 480;

    // Interpret parameters for server Port
    char *port_des = (char *)"custom port";
    if (!port) {
        port = default_port;
        port_des = (char *)"default port";
    }

    // Interpret parameters for server Directory
    char *dir_des = (char *)"custom directory";
    if(directory == "") {
        directory = default_directory;
        dir_des = (char *)"default directory";
    }
    const char *file_path_string = directory.toLocal8Bit().data();

    // Interpret parameters for server file Type
    if (envi) {         // This should be a switch statement if more filetypes (like cameralink) are implemented
        // Create a camera
        Camera = new ENVICamera(width, height, height);
    } else {
        // Create a camera
        Camera = new XIOCamera(width, height, height);
    }

    // Setup Camera
    Camera->setDir(file_path_string);
    bool cam_started = Camera->start();

    if (!cam_started) {
        qDebug() << "Failure to start camera.";
        frWidth = 0;
        frHeight = 0;
        isRunning = false;    // want to make sure that we don't enter the event loop
        return false;
    } else {
        frWidth = Camera->getFrameWidth();
        frHeight = Camera->getFrameHeight();
        dataHeight = Camera->getDataHeight();
        cam_type = Camera->getCameraType();
        if (frWidth == 0 || frHeight == 0) {
            qDebug() << "Failure to start camera. Incorrect sizing (0, 0).";
            frWidth = 0;
            frHeight = 0;
            isRunning = false;    // want to make sure that we don't enter the event loop
            return false;
        } else {
            connect(Camera, &CameraModel::timeout, this, &Server::reportTimeout);
            isRunning = true;    // now set up to enter the event loop
        }
    }

    // Start listening for clients
    if(listen(QHostAddress::Any, port)) {
        qDebug() << "Started image server on" << port_des << port << ". Using" << dir_des << directory;
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
    qDebug() << "Received Timeout, server no longer running. If this happens on startup, no valid files found.";
    isRunning = false;
}
