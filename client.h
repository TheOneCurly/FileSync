#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

#include <QTcpSocket>
#include <QHostAddress>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    static Client* getInstance();

    static const quint16 port = 55555;

public slots:
    void start();
    void connectToHost(const QString&);

private:
    static Client* m_instance;
    static QMutex m_instanceMutex;

    QTcpSocket* clientSocket;

private slots:
    void socketConnected();
    void socketDisconnected();

signals:
    void appendToConsole(const QString&);

};

#endif // CLIENT_H
