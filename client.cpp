#include "client.h"

Client* Client::m_instance = NULL;
QMutex Client::m_instanceMutex;

Client::Client(QObject *parent) : QObject(parent){

}

Client *Client::getInstance(){
    QMutexLocker locker(&m_instanceMutex);
    if(!m_instance){
        m_instance = new Client();
    }

    return m_instance;
}

void Client::start(){
    emit appendToConsole("Client started");
}

void Client::connectToHost(const QString& host){
    qDebug() << "Client connecting to " << host;
    clientSocket = new QTcpSocket();
    QObject::connect(clientSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    QObject::connect(clientSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    clientSocket->connectToHost(QHostAddress(host), port);
}

void Client::socketConnected(){
    emit appendToConsole("Client connected to " + clientSocket->peerAddress().toString());
    clientSocket->disconnectFromHost();
}

void Client::socketDisconnected(){
    clientSocket->deleteLater();
}
