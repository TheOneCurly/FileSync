#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QDir>

#include <QTcpSocket>
#include <QHostAddress>

#include "directoryinfomanager.h"

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
    QString basePathString;
    QDir clientPath;

    DirectoryInfoManager* m_directoryManager;

    void loadSettings();
    void saveSettings();

private slots:
    void socketConnected();
    void socketDisconnected();

signals:
    void appendToConsole(const QString&);

};

#endif // CLIENT_H
