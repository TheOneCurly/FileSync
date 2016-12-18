#include "host.h"

Host* Host::m_instance = NULL;
QMutex Host::m_instanceMutex;

Host::Host(QObject *parent) : QObject(parent){
    networkManager = new QNetworkAccessManager();
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addressLookupFinished(QNetworkReply*)));
}

Host *Host::getInstance(){
    QMutexLocker locker(&m_instanceMutex);
    if(!m_instance){
        m_instance = new Host();
    }

    return m_instance;
}

QStringList Host::getLocalAddresses(){
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
    QHostAddress address;
    QStringList addressStringList;
    foreach(address, addressList){
        if(!address.isNull() && !address.isLoopback() && address.protocol() != QAbstractSocket::IPv6Protocol){ // Someday local ipv6 might be used
            addressStringList.append(address.toString());
        }
    }

    return addressStringList;
}

void Host::start(){
    tcpServer = new QTcpServer();
    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnnection()));
    QObject::connect(tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));
    tcpServer->listen(QHostAddress::Any, port);

    emit appendToConsole("Host started");
}

void Host::lookupPublicAddress(){
    emit appendToConsole("Host asked for public address");

    QNetworkRequest request(QUrl("https://api.ipify.org"));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    networkManager->get(request);
}

void Host::addressLookupFinished(QNetworkReply* reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray responseArray = reply->readAll();
        if(responseArray.length() > 0){
            emit publicAddressFound(QString(responseArray));
        }
    }else{
        qDebug() << "Error checking external ip: " << reply->errorString();
    }
}

void Host::sslErrorHandler(QList<QSslError> errors){
    QSslError error;
    foreach(error, errors){
        qDebug() << "SSL Error: " << error.errorString();
    }
}

void Host::newConnnection(){
    if(tcpServer->hasPendingConnections()){
        QTcpSocket* newConnection = tcpServer->nextPendingConnection();
        QObject::connect(newConnection, SIGNAL(disconnected()), this, SLOT(connectionDisconnected()));

        emit appendToConsole("Host accepted connnection from " + newConnection->peerAddress().toString());
    }
}

void Host::acceptError(QAbstractSocket::SocketError error){
    qDebug() << "Error accepting tcp connection " << error;
}

void Host::connectionDisconnected(){
    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    if(socket){
        socket->deleteLater();
    }
}
