#include "connection.h"

Connection::Connection(QObject *parent, CameraModel *ReadingCamera, int ConnectionNumber) :
    QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(3);
    Camera = ReadingCamera;
    connectionFrameCounter = 0;
    connectionNumber = ConnectionNumber;
}

void Connection::SetSocket(int Descriptor)
{
    handshake = false; // Say that we haven't send the handshake data over yet
    socket = new QTcpSocket(this); // Create the socket value

    connect(socket, &QTcpSocket::connected, this, &Connection::connected);
    connect(socket, &QTcpSocket::disconnected, this, &Connection::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Connection::readyRead);
    connect(Camera, &CameraModel::timeout, this, &Connection::timeout);

    socket->setSocketDescriptor(Descriptor);

    qDebug() << "Connection started";
}

void Connection::connected()
{
    qDebug() << "Connection established from" << socket->peerAddress() << socket->peerPort() ;
}

void Connection::disconnected()
{
    qDebug() << "Connection closed by client";
}

void Connection::readyRead()
{
    QString message = socket->readAll();
    // Determine what to do on receiving a message
    if(!handshake && message == "ClientConnect") { // Handshake hasn't been initiated yet
        // Create the handshake object
        QJsonObject handshakeObj;
        QJsonDocument handshakeDoc;
        const int height = 480;
        const int width = 640;
        handshakeObj["height"] = height;
        handshakeObj["width"] = width;
        handshakeObj["requestType"] = "Handshake";
        handshakeDoc.setObject(handshakeObj);
        if(!socket->write(qCompress(handshakeDoc.toJson()))) { // If we write to the host, complete handshake. If not, disconnect.
            qDebug() << "Could not complete handshake. Terminating...";
            socket->disconnectFromHost();
        } else {
            handshake = true;
        }
    } else if (handshake && message == "ReadyFrame"){ // Handshake has occurred
        // Since we are doing this unqueued, we can assume that a handshaken request means the user wants data
        Task *task = new Task(Camera);
        task->setAutoDelete(true);
        connect(task, &Task::Result, this, &Connection::TaskResult, Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(task);
    }
}

void Connection::TaskResult(uint16_t *data) // Take the result and write it to the client
{
    size_t frameNumber = Camera->getFrameSize();
    size_t dataSize = frameNumber*2;
    char copy_data[dataSize];
    memcpy(copy_data, data, dataSize); // Should theoretically be optimized out by compiler?
    QByteArray Buffer = QByteArray::fromRawData(copy_data, dataSize);
    socket->waitForConnected();
    const int written = socket->write(Buffer);
    socket->waitForBytesWritten();
    qDebug() << "3. Got frame #" << connectionFrameCounter << ". Size: " << written << "bytes" << data[0] << data[1] << data[2];
    connectionFrameCounter++;
}

void Connection::timeout() // When the Cameramodel encounters a timeout.
{
    // The server could send a disconnect notice or something else in the future. Right now it doesn't do anything.
    //socket->disconnectFromHost();
    qDebug() << "Camera timed out";
}
