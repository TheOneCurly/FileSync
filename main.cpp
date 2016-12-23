#include "mainwindow.h"
#include <QtGlobal>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QThread>

#include "directoryinfomanager.h"
#include "host.h"
#include "client.h"

int main(int argc, char *argv[]){

    // These must be first for settings to load
    QCoreApplication::setOrganizationDomain("TheOneCurly");
    QCoreApplication::setApplicationName("FileSync");

    qRegisterMetaType<DirectoryInfo>("DirectoryInfo");
    qRegisterMetaType<DirectoryInfoManager>("DirectoryInfoManager");
    qRegisterMetaType<QList<DirectoryInfo>>("QList<DirectoryInfo>");

    qRegisterMetaTypeStreamOperators<DirectoryInfo>("DirectoryInfo");
    qRegisterMetaTypeStreamOperators<DirectoryInfoManager>("DirectoryInfoManager");
    qRegisterMetaTypeStreamOperators<QList<DirectoryInfo>>("QList<DirectoryInfo>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QThread clientThread;
    Client* m_client = Client::getInstance();
    QObject::connect(&clientThread, SIGNAL(started()), m_client, SLOT(start()));
    QObject::connect(&clientThread, SIGNAL(finished()), m_client, SLOT(deleteLater()));
    m_client->moveToThread(&clientThread);
    clientThread.start();

    QThread hostThread;
    Host* m_host = Host::getInstance();
    QObject::connect(&hostThread, SIGNAL(started()), m_host, SLOT(start()));
    QObject::connect(&hostThread, SIGNAL(finished()), m_host, SLOT(deleteLater()));
    m_host->moveToThread(&hostThread);
    hostThread.start();

    qDebug() << "Starting...";

    int status = a.exec();

    m_client->deleteLater();
    m_host->deleteLater();

    return status;
}
