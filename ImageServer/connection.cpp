#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(15);
}

void Connection::SetSocket(int Descriptor)
{
    handshake = false; // Say that we haven't send the handshake data over yet
    socket = new QTcpSocket(this); // Create the socket value

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    socket->setSocketDescriptor(Descriptor);

    qDebug() << "Connection started";
}

void Connection::connected()
{
    qDebug() << "Connection established";
}

void Connection::disconnected()
{
    qDebug() << "Connection closed by client";
}

void Connection::readyRead()
{
    qDebug() << socket->readAll();

    if(!handshake) { // Handshake hasn't been initiated yet
        const char *sizes = "100, 200";
        if(!socket->write(sizes)) {
            // Terminate the connection
            qDebug() << "Could not complete handshake. Terminating...";
        } else {
            // In this case that the socket writes sucessfully, all we need to do is wait for the next write to confirm that the handshake has started
            handshake = true;
        }
    } else {
        // Create task for getting data
        Task *task = new Task();
        task->setAutoDelete(true);
        connect(task, SIGNAL(Result(int)), SLOT(TaskResult(int)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(task);
    }
}

void Connection::TaskResult(int Number)
{
    //right here

    QByteArray Buffer;
    Buffer.append("\r\nTask Result = ");
    Buffer.append(QString::number(Number));

    socket->write(Buffer);
}
