#include "host.h"

Host* Host::m_instance = NULL;
QMutex Host::m_instanceMutex;

Host::Host(QObject *parent) : QObject(parent){
    // Load user settings
    loadSettings();

    // Set up host path
    QString hostPathString = basePathString + QDir::toNativeSeparators("/host/");
    hostPath.setPath(hostPathString);
    if(!hostPath.exists()){
        hostPath.mkpath(hostPathString);
    }

    m_directoryManager = new DirectoryInfoManager(hostPathString);

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

    emit appendToConsole("Host started: " + hostPath.absolutePath());

    // Begin listing files for hosting
}

void Host::lookupPublicAddress(){
    emit appendToConsole("Host asked for public address");

    QNetworkRequest request(QUrl("https://api.ipify.org"));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    networkManager->get(request);
}

void Host::loadSettings(){
    QSettings settings;
    settings.beginGroup("UserSettings");
    basePathString = settings.value("hostBasePath", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).toString();
    settings.endGroup();
}

void Host::saveSettings(){
    QSettings settings;
    settings.beginGroup("UserSettings");
    settings.setValue("hostBasePath", basePathString);
    settings.endGroup();
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
        QObject::connect(newConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
        QObject::connect(newConnection, SIGNAL(disconnected()), this, SLOT(connectionDisconnected()));

        emit appendToConsole("Host accepted connnection from " + newConnection->peerAddress().toString());
    }
}

void Host::acceptError(QAbstractSocket::SocketError error){
    qDebug() << "Error accepting tcp connection " << error;
}

void Host::readyRead(){
    emit appendToConsole("Host socket ready read");
    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    QDataStream in(socket);

    in.startTransaction();
    DirectoryInfoManager clientManager;
    in >> clientManager;

    if(!in.commitTransaction()){
        // stop if we didn't get all the data
        socket->close();
        return;
    }

    emit appendToConsole("Host read: ");
    QList<DirectoryInfo> clientDirList = clientManager.getDirectoryList();
    for(int i = 0; i < clientDirList.length(); i++){
        emit appendToConsole(clientDirList[i].getName());
    }
}

void Host::connectionDisconnected(){
    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    if(socket){
        socket->deleteLater();
    }
}
