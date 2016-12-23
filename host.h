#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QDir>

#include <QNetworkInterface>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>

#include <QTcpServer>
#include <QDataStream>

#include "directoryinfomanager.h"

class Host : public QObject
{
    Q_OBJECT
public:
    explicit Host(QObject *parent = 0);
    static Host* getInstance();
    static QStringList getLocalAddresses();

    static const quint16 port = 55555;

public slots:
    void start();
    void lookupPublicAddress();

private:
    static Host* m_instance;
    static QMutex m_instanceMutex;

    QNetworkAccessManager* networkManager;
    QTcpServer* tcpServer;
    QString basePathString;
    QDir hostPath;

    DirectoryInfoManager* m_directoryManager;

    void loadSettings();
    void saveSettings();

private slots:
    void addressLookupFinished(QNetworkReply *);
    void sslErrorHandler(QList<QSslError>);
    void newConnnection();
    void acceptError(QAbstractSocket::SocketError);
    void connectionDisconnected();
    void readyRead();

signals:
    void publicAddressFound(const QString&);
    void appendToConsole(const QString&);
};

#endif // HOST_H
