#include "server.h"

Server::Server(QObject *parent): QTcpServer(parent)
{

}

void Server::StartServer()
{
    const int port = 43565;
    if(listen(QHostAddress::Any, port)) {
        qDebug() << "Started image server on port " << port;
    } else {
        qDebug() << "Failed to start server. port: " << port;
    }
}

void Server::incomingConnection(qintptr handle)
{
    Connection *connection = new Connection(this);
    connection->SetSocket(handle);
}
