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
    //qDebug() << connectionNumber << "---";
    //qDebug() << "1. Received Request: " << socket->readAll(); // Print out whatever we received from the socket

    if(!handshake) { // Handshake hasn't been initiated yet
        // Create the handshake object
        QJsonObject handshakeObj;
        QJsonDocument handshakeDoc;
        const int height = 480;
        const int width = 640;
        handshakeObj["height"] = height;
        handshakeObj["width"] = width;
        handshakeObj["requestType"] = "Handshake";
        handshakeDoc.setObject(handshakeObj);
        if(!socket->write(qCompress(handshakeDoc.toJson()))) {
            // Terminate the connection
            qDebug() << "Could not complete handshake. Terminating...";
        } else {
            // In this case that the socket writes sucessfully, all we need to do is wait for the next write to confirm that the handshake has started
            handshake = true;
        }
    } else {
        //qDebug() << "2. Getting frame " << socket->state();
        // Create task for getting data
        // Since we are doing this unqueued, we can assume that this means the user wants data
        Task *task = new Task(Camera);
        task->setAutoDelete(true);
        connect(task, &Task::Result, this, &Connection::TaskResult, Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(task);
    }
}

void Connection::TaskResult(uint16_t *data)
{

    size_t frameNumber = Camera->getFrameSize();
    size_t dataSize = frameNumber*2;
    qDebug() << "3. Got frame #" << connectionFrameCounter << ". Size: " << dataSize << "bytes" << data[0] << data[1] << data[2];

    char copy_data[dataSize];
    memcpy(copy_data, data, dataSize); // Should theoretically be optimized out?
    QByteArray Buffer = QByteArray::fromRawData(copy_data, dataSize);
    ///qDebug() << "Buffer 1,2,3" << data[0] << data[1] << data[2];
    //qDebug() << "4. Processed data. Waiting to write..." << socket->state();
    socket->waitForConnected();
    const int written = socket->write(Buffer);
    socket->waitForBytesWritten();
    //qDebug() << "5. Wrote data back to client" << written << "bytes.";
    connectionFrameCounter++;
}
