#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QJsonObject>
#include <QJsonDocument>
#include "task.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    void SetSocket(int Descriptor);
signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void TaskResult(int Number);

private:
    QTcpSocket *socket;
    bool handshake;
};
#endif // CONNECTION_H
