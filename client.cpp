#include "client.h"

Client* Client::m_instance = NULL;
QMutex Client::m_instanceMutex;

Client::Client(QObject *parent) : QObject(parent){
    loadSettings();

    // Set up client path
    QString clientPathString = basePathString + QDir::toNativeSeparators("/client/");
    clientPath.setPath(clientPathString);
    if(!clientPath.exists()){
        clientPath.mkpath(clientPathString);
    }

    m_directoryManager = new DirectoryInfoManager(clientPathString);
}

Client *Client::getInstance(){
    QMutexLocker locker(&m_instanceMutex);
    if(!m_instance){
        m_instance = new Client();
    }

    return m_instance;
}

void Client::start(){
    emit appendToConsole("Client started: " + clientPath.absolutePath());

    // track local files by subdirectory
}

void Client::connectToHost(const QString& host){
    qDebug() << "Client connecting to " << host;
    clientSocket = new QTcpSocket();
    QObject::connect(clientSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    QObject::connect(clientSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    clientSocket->connectToHost(QHostAddress(host), port);
}

void Client::loadSettings(){
    QSettings settings;
    settings.beginGroup("UserSettings");
    basePathString = (settings.value("clientBasePath", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))).toString();
    settings.endGroup();
}

void Client::saveSettings(){
    QSettings settings;
    settings.beginGroup("UserSettings");
    settings.setValue("clientBasePath", basePathString);
    settings.endGroup();
}

void Client::socketConnected(){
    emit appendToConsole("Client connected to " + clientSocket->peerAddress().toString());
    //QByteArray data;
    QDataStream out(clientSocket);
    out << (*m_directoryManager);
    qDebug() << "Wrote data to stream";

//    qint64 bytesWritten = clientSocket->write(data);
//    clientSocket->close();

//    emit appendToConsole("Client wrote " + QString::number(bytesWritten) + " bytes");
}

void Client::socketDisconnected(){
    clientSocket->deleteLater();
}
